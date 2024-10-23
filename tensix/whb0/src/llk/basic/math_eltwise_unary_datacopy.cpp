
#include <cstdint>
#include <cassert>

#include "core/addr_mod.hpp"
#include "core/template.hpp"
#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/math.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::AddrMod;
using core::MopTemplate1;

using core::p_setrwc;
using core::p_mova2d;
using core::p_movb2d;
using core::p_stall;
using core::p_elwise;

//
//    Math
//

void Math::math_eltwise_unary_datacopy_init(
        DataCopyType type, 
        BroadcastType src_b_bcast_type, 
        bool transpose_xy,
        uint32_t within_face_16x16_transpose) {
    // within_face_16x16_transpose is used by unpacker, math does not transpose

    eltwise_unary_configure_addrmod(type, src_b_bcast_type);

    if (type == DataCopyType::A2D) {
        // p_mova2d::MOV_8_ROWS is currently not used by eltwise_unary_configure_mop
        eltwise_unary_configure_mop(
            type, src_b_bcast_type, transpose_xy, p_mova2d::MOV_8_ROWS, 16);
    } else if (type == DataCopyType::B2D) {
        eltwise_unary_configure_mop(
            type, src_b_bcast_type, false, p_movb2d::MOV_4_ROWS, 16);
    } else {
        assert(false);
    }

    reset_counters(p_setrwc::SET_ABD_F); 
}

void Math::math_eltwise_unary_datacopy(
        DataCopyType type, 
        BroadcastType src_b_bcast_type, 
        DstSync Dst, 
        bool is_fp32_dest_acc_en,
        uint32_t dst_index) {
    static constexpr uint32_t 
        CLR_B = p_setrwc::CLR_B,
        CLR_AB = p_setrwc::CLR_AB,
        SRCA_VLD = p_stall::SRCA_VLD,
        STALL_MATH = p_stall::STALL_MATH;

    if (Dst == DstSync::SyncTile16 || Dst == DstSync::SyncTile2) {
        set_dst_write_addr(
            DstTileLayout::Default, 
            DstTileShape::Tile32x32, 
            get_math_sync_tile_dst_index());
    } else {
        set_dst_write_addr(
            DstTileLayout::Default, 
            DstTileShape::Tile32x32, 
            dst_index);
    }

    if (type == DataCopyType::A2D) {
        MOP(1, 0, 0);
    } else if (type == DataCopyType::B2D) {
        // B2D FP32 dest unsupported until TF32 input support is added
        assert(!is_fp32_dest_acc_en);
        if (src_b_bcast_type == BroadcastType::SCALAR) {
            // Manually clear B once mop is done
            MOP(1, 0, 0);
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
        } else if (src_b_bcast_type == BroadcastType::COL) {
            // Mop for col broadcast only does 2 outerloops.
            // Needs to clear B manually and call twice
            STALLWAIT(STALL_MATH, SRCA_VLD);
            ZEROSRC(0, 1, 0, 1); // Zero out SrcA current read bank
            MOP(1, 0, 0);
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
            MOP(1, 0, 0);
            SETRWC(CLR_AB, 0, 0, 0, 0, 0);
        } else {
            MOP(1, 0, 0);
        }
    } else {
        assert(false);
    }

    clear_dst_reg_addr(); 
}

void Math::eltwise_unary_configure_addrmod(DataCopyType type, BroadcastType bcast_type) {
    // Use srcA for data movement
    if (type == DataCopyType::A2D) {
        AddrMod a0;
        a0.srca.incr = 1;
        a0.srcb.incr = 0;
        a0.dest.incr = 1;
        set_addr_mod(0, a0);

        // Just unpack into A and move to Dest
        AddrMod a2;
        a2.srca.incr = 8;
        a2.srcb.incr = 0;
        a2.dest.incr = 8;
        set_addr_mod(2, a2);
    } else {
        if (bcast_type == BroadcastType::ROW || bcast_type == BroadcastType::SCALAR) {
            AddrMod a0;
            a0.srca.incr = 0;
            a0.srcb.incr = 0;
            a0.dest.incr = 1;
            set_addr_mod(0, a0);

            // Just unpack into B and move to Dest
            AddrMod a2;
            a2.srca.incr = 0;
            a2.srcb.incr = 0;
            a2.dest.incr = 8;
            set_addr_mod(2, a2);
        } else {
            AddrMod a0;
            a0.srca.incr = 0;
            a0.srcb.incr = 1;
            a0.dest.incr = 1;
            set_addr_mod(0, a0);

            // Just unpack into B and move to Dest
            AddrMod a2;
            a2.srca.incr = 0;
            a2.srcb.incr = 8;
            a2.dest.incr = 8;
            set_addr_mod(2, a2);
        }
    } 
}

void Math::eltwise_unary_configure_mop(
        DataCopyType type, 
        BroadcastType bcast_type, 
        bool transpose_xy,
        uint32_t rows_per_inst, 
        uint32_t total_rows) {
    // always move 32x32 tile, packed as 16x16x4

    static constexpr uint32_t 
        CLR_B = p_setrwc::CLR_B,
        CLR_AB = p_setrwc::CLR_AB,
        MOV_1_ROW = p_movb2d::MOV_1_ROW,
        MOV_8_ROW_BRCST = p_movb2d::MOV_8_ROW_BRCST,
        MOV_8_ROW_BRCST_D0_BRCST = p_movb2d::MOV_8_ROW_BRCST_D0_BRCST,
        CR_B = p_setrwc::CR_B,
        SET_A = p_setrwc::SET_A,
        SET_B = p_setrwc::SET_B,
        SRCA_VLD = p_stall::SRCA_VLD,
        SRCB_VLD = p_stall::SRCB_VLD,
        STALL_MATH = p_stall::STALL_MATH,
        SRCB_NO_BCAST = p_elwise::SRCB_NO_BCAST,
        SRCB_BCAST_COL = p_elwise::SRCB_BCAST_COL;

    if (type == DataCopyType::A2D) {
        // In B0, SFPU with fp16 SyncTile16 is supported by HW,
        // but requires src A/B both to have data valid set
        // otherwise data mismatch is seen due to packer not stalling properly
        // We implement datacopy as elwadd with 0 that has been zeroed out by unpacker
        // This assumes no kernel fusion where another op might use src B
        // Additional benefit of supporting fp32 dest (SyncTile2)
        // regardless of unpack src a/b format

        uint32_t innerloop = total_rows >> 3;
        uint32_t outerloop = 4;
        MopTemplate1 m;
        m.loop_outer = outerloop;
        m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
        m.loop_inner = innerloop;
        m.loop_op0 = TT_OP(ELWADD(0, 0, SRCB_NO_BCAST, 2, 0));
        m.end_op0 = TT_OP(SETRWC(CLR_AB, 0, 0, 0, 0, SET_A));
        set_mop_templ_1(m);
    } else if (type == DataCopyType::B2D) {
        // 0: dest_rwc += 1, 2: dest_rwc += 8
        uint32_t addr_mod = (rows_per_inst == MOV_1_ROW) ? 0 : 2;

        if (bcast_type == BroadcastType::SCALAR) {
            uint32_t outerloop = 4;
            uint32_t innerloop = total_rows >> 3;
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(MOVB2D(0, 0, addr_mod, MOV_8_ROW_BRCST_D0_BRCST, 0));
            m.end_op0 = TT_OP(SETRWC(0, CR_B, 0, 0, 0, SET_B));
            set_mop_templ_1(m);
        } else if (bcast_type == BroadcastType::COL) {
            // The mop only runs for 2 outer loops and mop is called twice for col broadcast
            uint32_t outerloop = 2;
            uint32_t innerloop = 16 >> 3; // elwadd produces 8 rows per op
            // broadcast_type = p_movb2d::MOV_8_ROW_BRCST_D0_BRCST;
            // MOVB2D with column broadcast doesn't work due to the bug in FPU tile
            // which masks dest write enable signals when instrn_mode[1:0] == 2'b01
            // ELWADD with zeros will be used as a workaround
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(ELWADD(0, 0, SRCB_BCAST_COL, addr_mod, 0));
            m.end_op0 = TT_OP(SETRWC(0, CR_B, 0, 0, 0, SET_B));
            set_mop_templ_1(m);
        } else if (bcast_type == BroadcastType::ROW) {
            uint32_t outerloop = 4;
            uint32_t innerloop = total_rows >> 3;
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(MOVB2D(0, 0, addr_mod, MOV_8_ROW_BRCST, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_B, CR_B, 0, 0, 0, SET_B));
            set_mop_templ_1(m);
        } else {
            // rows_per_inst may be p_movb2d::MOV_1_ROW or p_movb2d::MOV_4_ROWS
            // currently only p_movb2d::MOV_4_ROWS is used
            uint32_t innerloop = 
                (rows_per_inst == MOV_1_ROW) ? total_rows : (total_rows >> 2);
            uint32_t outerloop = 4;
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(MOVB2D(0, 0, addr_mod, rows_per_inst, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_B, CR_B, 0, 0, 0, SET_B));
            set_mop_templ_1(m);
        }
    } 
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namepace ronin

