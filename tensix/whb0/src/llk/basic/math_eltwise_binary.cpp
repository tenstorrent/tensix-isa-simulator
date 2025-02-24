// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

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

using core::p_setrwc;
using core::p_stall;
using core::p_zeroacc;
using core::p_elwise;

using core::AddrMod;
using core::MopTemplate1;

//
//    Math
//

void Math::math_eltwise_binary_init(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType src_b_bcast_type, 
        uint32_t NUM_FIDELITY_PHASES, 
        bool acc_to_dest) {
    assert(
        eltwise_binary_type == EltwiseBinaryType::ELWADD || 
        eltwise_binary_type == EltwiseBinaryType::ELWSUB || 
        eltwise_binary_type == EltwiseBinaryType::ELWMUL);
    assert(
        src_b_bcast_type == BroadcastType::NONE ||
        src_b_bcast_type == BroadcastType::COL ||
        src_b_bcast_type == BroadcastType::ROW ||
        src_b_bcast_type == BroadcastType::SCALAR);
    assert(!(acc_to_dest && (src_b_bcast_type != BroadcastType::NONE)));

    math_eltwise_binary_configure_addrmod(eltwise_binary_type, src_b_bcast_type);
    math_eltwise_binary_configure_mop(
        eltwise_binary_type, 
        src_b_bcast_type, 
        NUM_FIDELITY_PHASES, 
        acc_to_dest);

    reset_counters(p_setrwc::SET_ABD_F); 
}

void Math::math_eltwise_binary(
        EltwiseBinaryType eltwise_binary_type,
        BroadcastType src_b_bcast_type,
        DstSync Dst,
        int NUM_FIDELITY_PHASES,
        bool acc_to_dest,
        bool is_fp32_dest_acc_en,
        uint32_t dst_index, 
        bool clear_dest_acc) {
    static constexpr uint32_t 
        CLR_B = p_setrwc::CLR_B,
        SET_D = p_setrwc::SET_D;

    assert(
        eltwise_binary_type == EltwiseBinaryType::ELWADD || 
        eltwise_binary_type == EltwiseBinaryType::ELWSUB || 
        eltwise_binary_type == EltwiseBinaryType::ELWMUL);
    assert(
        src_b_bcast_type == BroadcastType::NONE ||
        src_b_bcast_type == BroadcastType::COL ||
        src_b_bcast_type == BroadcastType::ROW ||
        src_b_bcast_type == BroadcastType::SCALAR);
    assert(!(acc_to_dest && src_b_bcast_type != BroadcastType::NONE));

    bool high_fidelity = (NUM_FIDELITY_PHASES > 0);

    if (Dst == DstSync::SyncTile16 || Dst == DstSync::SyncTile2) {
        uint32_t math_sync_tile_dst_index = get_math_sync_tile_dst_index();

        set_dst_write_addr(
            DstTileLayout::Default, 
            DstTileShape::Tile32x32, 
            math_sync_tile_dst_index);

        if (Dst == DstSync::SyncTile16) {
            uint32_t ZERO_ACC_MODE = 
                is_fp32_dest_acc_en ? p_zeroacc::CLR_16_32B : p_zeroacc::CLR_16;

            if (eltwise_binary_type == EltwiseBinaryType::ELWMUL) {
                ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 0);
                ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 1);
                ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 2);
                ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 3);
            } else if (acc_to_dest) {
                if (clear_dest_acc) {
                    ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 0);
                    ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 1);
                    ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 2);
                    ZEROACC(ZERO_ACC_MODE, 1, (math_sync_tile_dst_index << 2) + 3);
                }
            }
        }

    } else {
        set_dst_write_addr(DstTileLayout::Default, DstTileShape::Tile32x32, dst_index);
    }

    if (acc_to_dest) {
        assert(eltwise_binary_type == EltwiseBinaryType::ELWADD || 
            eltwise_binary_type == EltwiseBinaryType::ELWSUB);
        // accumulate to dest where srcA <- 0 and dest += (srcA +/- srcB)
        MOP(1, 0, 0);
    } else if (eltwise_binary_type == EltwiseBinaryType::ELWADD || 
            eltwise_binary_type == EltwiseBinaryType::ELWSUB) {
        if (src_b_bcast_type == BroadcastType::SCALAR) {
            // Manually clear B once mop is done
            MOP(1, 0, 0);
            SETRWC(CLR_B, 0, 0, 0, 0, SET_D);
        } else if (src_b_bcast_type == BroadcastType::COL) {
            // Mop for col broadcast only does 2 outerloops.
            // Needs to clear B manually and call twice
            MOP(1, 0, 0);
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
            MOP(1, 0, 0);
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
        } else {
            // Row and no broadcasted behaves similarly
            MOP(1, 0, 0);
        }
    } else if (eltwise_binary_type == EltwiseBinaryType::ELWMUL) {
        if (src_b_bcast_type == BroadcastType::SCALAR) {
            // Manually clear B once mop is done
            if (high_fidelity) {
                for (uint32_t n = 0; n < 4; n++) {  // N-num faces
                    MOP(1, 0, 0);
                }
            } else {
                MOP(1, 0, 0);
            }
            SETRWC(CLR_B, 0, 0, 0, 0, SET_D);
        } else if (src_b_bcast_type == BroadcastType::COL) {
            // Mop for col broadcast only does 2 outerloops.
            // Needs to clear B manually and call twice
            if (high_fidelity) {
                MOP(1, 0, 0);
                MOP(1, 0, 0);
            } else {
                MOP(1, 0, 0);
            }
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
            if (high_fidelity) {
                MOP(1, 0, 0);
                MOP(1, 0, 0);
            } else {
                MOP(1, 0, 0);
            }
            SETRWC(CLR_B, 0, 0, 0, 0, 0);
        } else {
            // Row and no broadcasted behaves similarly
            if (high_fidelity) {
                for (uint32_t n = 0; n < 4; n++) {  // N-num faces
                    MOP(1, 0, 0);
                }
            } else {
                MOP(1, 0, 0);
            }
        }
    } else {
        assert(false);
    }

    clear_dst_reg_addr();
}

void Math::math_eltwise_binary_configure_addrmod(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType bcast_type) {
    // Use srcA for data movement
    if (bcast_type == BroadcastType::NONE || bcast_type == BroadcastType::COL) {
        AddrMod a0;
        a0.srca.incr = 8;
        a0.srcb.incr = 8;
        a0.dest.incr = 8,
        set_addr_mod(0, a0);
    } else {
        // bcast_type == BroadcastType::ROW || bcast_type == BroadcastType::SCALAR
        AddrMod a0;
        a0.srca.incr = 8;
        a0.srcb.incr = 0;
        a0.dest.incr = 8;
        set_addr_mod(0, a0);
    }

    AddrMod a1;
    a1.srca.incr = 0;
    a1.srcb.incr = 0;
    a1.dest.incr = 0;
    set_addr_mod(1, a1);

    AddrMod a2;
    a2.srca.incr = 0;
    a2.srca.clr = 1;
    a2.srcb.incr = 0;
    a2.srcb.clr = 1;
    a2.dest.incr = 0;
    a2.dest.clr = 0;
    a2.dest.cr = 1;
    a2.fidelity.incr = 1;
    set_addr_mod(2, a2);

    AddrMod a3;
    a3.srca.incr = 0;
    a3.srca.clr = 1;
    a3.srcb.incr = 0;
    a3.srcb.clr = 1;
    a3.dest.incr = 8;
    a3.dest.clr = 0;
    a3.dest.cr = 0;
    a3.dest.c_to_cr = 1;
    a3.fidelity.incr = 0;
    a3.fidelity.clr = 1;
    set_addr_mod(3, a3);
} 

void Math::math_eltwise_binary_configure_mop(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType bcast_type, 
        uint32_t NUM_FIDELITY_PHASES, 
        bool acc_to_dest) {
    static constexpr uint32_t 
        CLR_A = p_setrwc::CLR_A,
        CLR_AB = p_setrwc::CLR_AB,
        CR_AB = p_setrwc::CR_AB,
        SET_AB = p_setrwc::SET_AB,
        SRCA_VLD = p_stall::SRCA_VLD,
        SRCB_VLD = p_stall::SRCB_VLD,
        STALL_MATH = p_stall::STALL_MATH,
        SRCB_NO_BCAST = p_elwise::SRCB_NO_BCAST,
        SRCB_BCAST_COL = p_elwise::SRCB_BCAST_COL,
        SRCB_BCAST_ROW = p_elwise::SRCB_BCAST_ROW,
        SRCB_BCAST_ALL = p_elwise::SRCB_BCAST_ALL,
        DEST_ACCUM_EN = p_elwise::DEST_ACCUM_EN,
        DEST_ACCUM_DIS = p_elwise::DEST_ACCUM_DIS;

    bool high_fidelity = (NUM_FIDELITY_PHASES > 0);

    uint32_t innerloop = 16 >> 3;  // 8 rows per eltwise op at a time.
    // The mop only runs for 2 outer loops and mop is called twice for col broadcast
    uint32_t outerloop = (bcast_type == BroadcastType::COL) ? 2 : 4;
    uint32_t broadcast_type = SRCB_NO_BCAST;
    if (bcast_type == BroadcastType::COL) {
        broadcast_type = SRCB_BCAST_COL;
    } else if (bcast_type == BroadcastType::ROW) {
        broadcast_type = SRCB_BCAST_ROW;
    } else if (bcast_type == BroadcastType::SCALAR) {
        broadcast_type = SRCB_BCAST_ALL;
    }

    // Scalar and Col broadcast should not Clear B within a mop.
    // This is controlled outside of MOP.
    if (bcast_type == BroadcastType::COL || bcast_type == BroadcastType::SCALAR) {
        if (eltwise_binary_type == EltwiseBinaryType::ELWADD) {
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(ELWADD(0, 0, broadcast_type, 0, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_A, CR_AB, 0, 0, 0, SET_AB));
            set_mop_templ_1(m);
        } else if (eltwise_binary_type == EltwiseBinaryType::ELWSUB) {
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(ELWSUB(0, 0, broadcast_type, 0, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_A, CR_AB, 0, 0, 0, SET_AB));
            set_mop_templ_1(m);
        } else if (eltwise_binary_type == EltwiseBinaryType::ELWMUL) {
            MopTemplate1 m;
            if (high_fidelity) {
                m.loop_outer = NUM_FIDELITY_PHASES;
                m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
                m.loop_inner = innerloop;
                m.loop_op0 = TT_OP(ELWMUL(0, 0, broadcast_type, 0, 0));
                // Incr fidelity last inst of inner loop
                m.last_inner_op = TT_OP(ELWMUL(0, 0, broadcast_type, 2, 0));
                m.last_outer_op = TT_OP(ELWMUL(CLR_A, 0, broadcast_type, 3, 0));
            } else {
                m.loop_outer = outerloop;
                m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
                m.loop_inner = innerloop;
                m.loop_op0 = TT_OP(ELWMUL(0, 0, broadcast_type, 0, 0));
                m.end_op0 = TT_OP(SETRWC(CLR_A, CR_AB, 0, 0, 0, SET_AB));
            }
            set_mop_templ_1(m);
        }
    } else {
        // bcast_type == BroadcastType::NONE || bcast_type == BroadcastType::ROW
        if (eltwise_binary_type == EltwiseBinaryType::ELWADD) {
            uint32_t ACC_MODE = acc_to_dest ? DEST_ACCUM_EN : DEST_ACCUM_DIS;
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(ELWADD(0, ACC_MODE, broadcast_type, 0, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_AB, CR_AB, 0, 0, 0, SET_AB));
            set_mop_templ_1(m);
        } else if (eltwise_binary_type == EltwiseBinaryType::ELWSUB) {
            uint32_t ACC_MODE = acc_to_dest ? DEST_ACCUM_EN : DEST_ACCUM_DIS;
            MopTemplate1 m;
            m.loop_outer = outerloop;
            m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
            m.loop_inner = innerloop;
            m.loop_op0 = TT_OP(ELWSUB(0, ACC_MODE, broadcast_type, 0, 0));
            m.end_op0 = TT_OP(SETRWC(CLR_AB, CR_AB, 0, 0, 0, SET_AB));
            set_mop_templ_1(m);
        } else if (eltwise_binary_type == EltwiseBinaryType::ELWMUL) {
            MopTemplate1 m;
            if (high_fidelity) {
                m.loop_outer = NUM_FIDELITY_PHASES;
                m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
                m.loop_inner = innerloop;
                m.loop_op0 = TT_OP(ELWMUL(0, 0, broadcast_type, 0, 0));
                // Incr fidelity last inst of inner loop
                m.last_inner_op = TT_OP(ELWMUL(0, 0, broadcast_type, 2, 0));
                m.last_outer_op = TT_OP(ELWMUL(CLR_AB, 0, broadcast_type, 3, 0));
            } else {
                m.loop_outer = outerloop;
                m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
                m.loop_inner = innerloop;
                m.loop_op0 = TT_OP(ELWMUL(0, 0, broadcast_type, 0, 0));
                m.end_op0 = TT_OP(SETRWC(CLR_AB, CR_AB, 0, 0, 0, SET_AB));
            }
            set_mop_templ_1(m);
        }
    }
} 

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

