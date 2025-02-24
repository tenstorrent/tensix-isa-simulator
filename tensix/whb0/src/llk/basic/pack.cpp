// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/addr_mod.hpp"
#include "core/template.hpp"
#include "core/instr_params.hpp"
#include "core/gpr_map.hpp"
#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/pack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_setadc;
using core::p_pacr;
using core::p_ind;

using core::p_gpr_pack;

using core::AddrModPack;
using core::MopTemplate1;
using core::DataFormat;

//
//    Pack
//

void Pack::pack_hw_configure_disaggregated(
        bool untilize, 
        bool is_fp32_dest_acc_en, 
        ReluType relu_type, 
        uint32_t relu_threshold,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
#if 0 // TODO: Revise this
    configure_pack(
        is_fp32_dest_acc_en,
        out_src_format,
        out_dst_format, 
        relu_type, 
        relu_threshold);
#endif
    configure_pack(
        is_fp32_dest_acc_en,
        untilize,
        out_src_format,
        out_dst_format, 
        relu_type, 
        relu_threshold);
    // SKIPPED: Store value for num_msg_received register when untilize is true
}

void Pack::pack_init(bool untilize, bool zero_output, DstTileFaceLayout FaceLayout) {
    pack_mop_config(untilize, zero_output, FaceLayout); 
}

void Pack::pack(
        DstSync Dst, 
        bool untilize, 
        bool is_fp32_dest_acc_en, 
        bool pack_l1_acc_en,
        uint32_t tile_index, 
        uint32_t output_addr, 
        bool pack_l1_acc) {
    uint32_t DEST_NUM_TILES_SHIFT = is_fp32_dest_acc_en ? 1 : 0;
    uint32_t DEST_NUM_TILES = DEST_NUM_TILES_FP16 >> DEST_NUM_TILES_SHIFT; 

    if (Dst == DstSync::SyncTile16) {
        // W-counter points to the next tile in dest
        SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, get_pack_sync_tile_dst_ptr());
        incr_pack_sync_tile_dst_ptr(1, DEST_NUM_TILES);
    } else if (Dst == DstSync::SyncTile2) {
        SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, get_pack_sync_tile_dst_ptr());
        set_pack_sync_tile_dst_ptr(0);
    } else {
        SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, tile_index);
    }

    // Packer is programmed to automatically skip the tile header
    // Since output_addr points to tile content, we need to -1 pack address
    // (in terms of 16B words) to offset packer's +1 
    // ACHTUNG: In original code this is done for GS but not for WHB0.
    //     Double check whether this logic applies to WHB0.
    output_addr--;

    program_packer_destination(PackSelMask::PACK_ALL, output_addr);

    if (pack_l1_acc_en) {
        // ACHTUNG: Since this is conditional, register must be cleared at init
        program_packer_l1_acc(pack_l1_acc);
    }

    MOP(1, 0, 0);

    if (untilize) {
        PACR(2, 0, 0xf, 0, 0, 1, 1); // close tile
        SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_Y, 0);
        INCADCZW(p_setadc::PAC, 0, 0, 1, 0);
    } 
}

#if 0 // TODO: Revise this
void Pack::pack_mop_config(bool untilize, bool zero_output, DstTileFaceLayout FaceLayout) {
    AddrModPack a0;
    a0.y_src.incr = untilize ? 0 : 1;
    a0.y_dst.incr = 1;
    set_addr_mod_pack(0, a0);

    if (untilize) {
        AddrModPack a1;
        a1.y_src.incr = 1;
        a1.y_src.clr = 0;
        a1.y_src.cr = 1;
        a1.y_dst.incr = 1;
        a1.y_dst.clr = 0;
        a1.y_dst.cr = 0;
        set_addr_mod_pack(1, a1);
    } else {
        AddrModPack a1;
        a1.y_src.incr = 0;
        a1.y_src.clr = 1;
        a1.y_src.cr = 0;
        a1.y_dst.incr = 0;
        a1.y_dst.clr = 1;
        a1.y_dst.cr = 0;
        a1.z_src.incr = 0;
        a1.z_src.clr = 0;
        a1.z_dst.incr = 0;
        a1.z_dst.clr = 0;
        set_addr_mod_pack(1, a1);
    }

    AddrModPack a2;
    a2.y_src.incr = 0;
    a2.y_src.clr = 0;
    a2.y_src.cr = 0;
    a2.y_dst.incr = 0;
    a2.y_dst.clr = 0;
    a2.y_dst.cr = 0;
    set_addr_mod_pack(2, a2);

    uint32_t MOP_INNER_LOOP = 16;
    uint32_t MOP_UNTILIZE_INNER_LOOP = (FaceLayout == DstTileFaceLayout::ColMajor) ? 8 : 4;
    uint32_t MOP_OUTER_LOOP = 1;
    uint32_t MOP_UNTILIZE_OUTER_LOOP = 8;
    int32_t PACKCNT = 4;
    uint32_t MEGAROW = 1;
    uint32_t ZERO_OUTPUT_FLAG = 
        zero_output ? p_pacr::P_ZERO_OUTPUT_ENABLED : p_pacr::P_ZERO_OUTPUT_DISABLED;

    // addr_mode      = 0
    // zero_write     = ZERO_OUTPUT_FLAG
    // pack_sel       = PACK_SEL(PACKCNT)
    // ovrd_thread_id = 0
    // concat         = MEGAROW
    // flush          = 0
    // last           = 0
    auto pack = TT_OP(PACR(0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));

    MopTemplate1 m;
    if (!untilize) {
        //
        //    set offsets for packers: 0, 32, 16, 48 (ColMajor) or 0, 16, 32, 48 (RowMajor)
        //    loop 16
        //        PACR at (offset[packer] + ch0_y) with 4 packers, one row per packer
        //            then auto ch0_y += 1 for all except last iterations
        //
        m.loop_outer = MOP_OUTER_LOOP;
        m.loop_inner = MOP_INNER_LOOP;
        m.loop_op0 = pack;
        m.last_inner_op = TT_OP(PACR(1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, 0, 0, 1));
        m.last_outer_op = TT_OP(PACR(1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, 0, 0, 1));
#if 0 // SKIPPED: No tile headers (always?)
        // Write header to l1
        // mem_hier_sel   = 1 (L1)
        // size_sel       = 0 (16B)
        // reg_size_sel   = p_ind::LD_16B
        // offset_index   = LO_16(0)
        // auto_inc_spec  = p_ind::INC_NONE
        // data_reg_index = p_gpr_pack::TILE_HEADER
        // addr_reg_index = p_gpr_pack::OUTPUT_ADDR
        m.end_op0 = 
            TT_OP(STOREIND(
                1, 
                0, 
                p_ind::LD_16B, 
                LO_16(0), 
                p_ind::INC_NONE, 
                p_gpr_pack::TILE_HEADER, 
                p_gpr_pack::OUTPUT_ADDR));
#endif
    } else {
        //
        //    set offsets for packers: 0, 8, 16, 24 (ColMajor) or 0, 8, 32, 40 (RowMajor)
        //    loop 8
        //        PACR at (offset[packer] + ch0_y) with 4 packers, one row per packer
        //        # advance ch0_y by 32 or 16 rows
        //        loop 8 (ColMajor) or 4 (RowMajor)
        //            ch0_y += 4    
        //        PACR with new ch0_y, then auto ch0_y_cr += 1, ch0_y = ch0_y_cr
        //
        m.loop_outer = MOP_UNTILIZE_OUTER_LOOP;
        m.start_op = TT_OP(PACR(0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
        m.loop_inner = MOP_UNTILIZE_INNER_LOOP;
// overridden in original code 
//        m.loop_op0 = pack;
        m.loop_op0 = TT_OP(INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
// redundant in original code
//        m.last_inner_op = TT_OP(INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
//        m.last_outer_op = TT_OP(INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
        m.end_op0 = TT_OP(PACR(1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
    }
    set_mop_templ_1(m);
}
#endif

// Upgraded to 0.33 (pack by 256-item rows unless untilize)

void Pack::pack_mop_config(bool untilize, bool zero_output, DstTileFaceLayout FaceLayout) {
    AddrModPack a0;
    a0.y_src.incr = untilize ? 0 : 15;
    a0.y_dst.incr = 1;
    set_addr_mod_pack(0, a0);

    if (untilize) {
        AddrModPack a1;
        a1.y_src.incr = 1;
        a1.y_src.clr = 0;
        a1.y_src.cr = 1;
        a1.y_dst.incr = 1;
        a1.y_dst.clr = 0;
        a1.y_dst.cr = 0;
        set_addr_mod_pack(1, a1);
    } else {
        AddrModPack a1;
        a1.y_src.incr = 0;
        a1.y_src.clr = 1;
        a1.y_src.cr = 0;
        a1.y_dst.incr = 0;
        a1.y_dst.clr = 1;
        a1.y_dst.cr = 0;
        a1.z_src.incr = 0;
        a1.z_src.clr = 0;
        a1.z_dst.incr = 0;
        a1.z_dst.clr = 0;
        set_addr_mod_pack(1, a1);
    }

    AddrModPack a2;
    a2.y_src.incr = 0;
    a2.y_src.clr = 0;
    a2.y_src.cr = 0;
    a2.y_dst.incr = 0;
    a2.y_dst.clr = 0;
    a2.y_dst.cr = 0;
    set_addr_mod_pack(2, a2);

    int32_t PACKCNT = 4;
    uint32_t MEGAROW = 1;
    uint32_t ZERO_OUTPUT_FLAG = 
        zero_output ? p_pacr::P_ZERO_OUTPUT_ENABLED : p_pacr::P_ZERO_OUTPUT_DISABLED;

    auto pack = TT_OP(PACR(0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));

    MopTemplate1 m;
    if (!untilize) {
        uint32_t MOP_INNER_LOOP = 1;
        uint32_t MOP_OUTER_LOOP = 1;
        //
        //    set offsets for packers: 0, 32, 16, 48 (ColMajor) or 0, 16, 32, 48 (RowMajor)
        //    PACR at (offset[packer] + ch0_y) with 4 packers, one 256-item row per packer
        //
        m.loop_outer = MOP_OUTER_LOOP;
        m.loop_inner = MOP_INNER_LOOP;
        m.loop_op0 = TT_OP(PACR(1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1));
    } else {
        uint32_t MOP_UNTILIZE_INNER_LOOP = (FaceLayout == DstTileFaceLayout::ColMajor) ? 8 : 4;
        uint32_t MOP_UNTILIZE_OUTER_LOOP = 8;
        //
        //    set offsets for packers: 0, 8, 16, 24 (ColMajor) or 0, 8, 32, 40 (RowMajor)
        //    loop 8
        //        PACR at (offset[packer] + ch0_y) with 4 packers, one row per packer
        //        # advance ch0_y by 32 or 16 rows
        //        loop 8 (ColMajor) or 4 (RowMajor)
        //            ch0_y += 4    
        //        PACR with new ch0_y, then auto ch0_y_cr += 1, ch0_y = ch0_y_cr
        //
        m.loop_outer = MOP_UNTILIZE_OUTER_LOOP;
        m.start_op = TT_OP(PACR(0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
        m.loop_inner = MOP_UNTILIZE_INNER_LOOP;
        m.loop_op0 = TT_OP(INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
        m.end_op0 = TT_OP(PACR(1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
    }
    set_mop_templ_1(m);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

