// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/template.hpp"
#include "core/instr_params.hpp"
#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/unpack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_unpacr;

using core::MopTemplate0;
using core::DataFormat;

//
//    Unpack
//

void Unpack::unpack_AB_hw_configure_disaggregated(
        BroadcastType BType,
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format) {
    bool is_row_pool = false;
    bool transpose_xy_srca = false;
    uint32_t srca_height = 16;
    uint32_t srcb_height =
        (BType == BroadcastType::SCALAR || BType == BroadcastType::ROW) ? 1 : 16;
    configure_unpack_AB(
        unpa_src_format,
        unpa_dst_format,
        unpb_src_format,
        unpb_dst_format,
        srca_height, 
        srcb_height, 
        is_row_pool, 
        transpose_xy_srca, 
        is_fp32_dest_acc_en);
}

void Unpack::unpack_AB_init(BroadcastType BType) {
    unpack_AB_mop_config(BType);
} 

void Unpack::unpack_AB(
        BroadcastType BType,
        uint32_t address_a,
        uint32_t address_b) {
    // Clear z/w start counters
    // (all unpackers and channels)
    SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    start_unpack_AB(address_a, address_b);

    // Run MOP
    if (BType == BroadcastType::ROW || BType == BroadcastType::COL) {
        mop_run(2);
    } else if (BType == BroadcastType::SCALAR) {
        mop_run(1);
    } else {
        mop_run(4);
    }

    end_unpack_AB();
} 

void Unpack::unpack_AB_mop_config(BroadcastType BType) {
    //    unpack_block_selection = SrcA / SrcB
    //    addr_mode              = 0b1 (CH0_Z_INC)
    //    cfg_context_cnt_inc    = 0
    //    cfg_context_id         = 0
    //    addr_cnt_context_id    = 0
    //    ovrd_thread_id         = 1
    //    set_dat_valid          = 1
    //    rareb_en               = RAREFYB_DISABLE
    //    zero_write2            = 0
    //    auto_inc_context_id    = 0
    //    row_search             = 0
    //    search_cache_flush     = 0
    //    last                   = 1
    auto unpack_srca =
        TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
    auto unpack_srcb =
        TT_OP(UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));

    if (BType == BroadcastType::COL) {
        // cnt_set_mask = unpacker1 (SrcB), ch0_z = 2, bit_mask = ch0_z (ch0 = src)
        auto unpack_srcb_set_z = TT_OP(SETADCZW(0b010, 0, 0, 0, 2, 0b0001));
        // 1 x unpack_srcb, 2 x unpack_srca, set SrcB input to tile [2]
        MopTemplate0 m;
        m.unpack_b = false;
        m.unpack_halo = true;
        m.a0_instr = unpack_srcb;
        m.a1_instr = unpack_srca;
        m.a2_instr = unpack_srca;
        m.a3_instr = unpack_srcb_set_z;
        set_mop_templ_0(m);
    } else if (BType == BroadcastType::ROW) {
        // cnt_set_mask = unpacker1, ch0_z = 0, bit_mask = ch0_z (ch0 = src)
        auto unpack_srcb_clear_z = TT_OP(SETADCZW(0b010, 0, 0, 0, 0, 0b0001));
        // 2 x (unpack_srca, unpack_srcb), set SrcB input to tile [0]
        MopTemplate0 m;
        m.unpack_b = true;
        m.unpack_halo = true;
        m.a0_instr = unpack_srcb;
        m.a1_instr = unpack_srca;
        m.a2_instr = unpack_srcb;
        m.a3_instr = unpack_srca;
        m.b_instr = unpack_srcb_clear_z;
        set_mop_templ_0(m);
    } else if (BType == BroadcastType::SCALAR) {
        // 4 x unpack_srca, 1 x unpack_srb
        MopTemplate0 m;
        m.unpack_b = true;
        m.unpack_halo = true;
#if 0 // TODO: Revise this
        m.a0_instr = unpack_srca;
        m.a1_instr = unpack_srca;
        m.a2_instr = unpack_srca;
        m.a3_instr = unpack_srca;
        m.b_instr = unpack_srcb;
#else
        // unpack SrcB first to avoid deadlocks
        m.a0_instr = unpack_srcb;
        m.a1_instr = unpack_srca;
        m.a2_instr = unpack_srca;
        m.a3_instr = unpack_srca;
        m.b_instr = unpack_srca;
#endif
        set_mop_templ_0(m);
    } else {    // BType == BroadcastType::NONE
        // 1 x unpack_srca, 1 x unpack_srcb
        MopTemplate0 m;
        m.unpack_b = true;
        m.unpack_halo = false;
        m.a0_instr = unpack_srca;
        m.b_instr = unpack_srcb;
        set_mop_templ_0(m);
    }
} 

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

