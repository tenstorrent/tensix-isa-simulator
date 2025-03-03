// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

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
using core::p_unpacr_nop;
using core::p_setadc;

using core::MopTemplate0;
using core::DataFormat;

//
//    Unpack
//

void Unpack::unpack_untilize_hw_configure_disaggregated(
        DataFormat unpa_src_format, DataFormat unpa_dst_format) {
    configure_unpack_AB(
        unpa_src_format, 
        unpa_dst_format, 
        unpa_src_format, 
        unpa_dst_format, 
        1,
        16,
        false,
        false,
        false);
}

void Unpack::unpack_untilize_init(DataFormat unpa_src_format, DataFormat unpa_dst_format) {
    unpack_untilize_mop_config();

    // SKIPPED: wait_for_idle

    // Override default settings

#if 0 // TODO: Revise this (used for strides only)
    uint32_t srca_face_height = 1;
    uint32_t srcb_face_height = 16;
#endif

    // SKIPPED: Strides

    set_tile_x_dim(FACE_HEIGHT);

    DataFormat in_data_format_unp0 = unpa_src_format; // unchanged
    uint32_t x_dim = 16;
    uint32_t y_dim = FACE_HEIGHT;
    uint32_t z_dim = 4; // unchanged
    set_tile_descriptor(0, in_data_format_unp0, x_dim, y_dim, z_dim);

    // SKIPPED: Set UNP0_ADDR_BASE_REG_0_Base_ADDR32
    //     (base address skips halo rows in srcA (ch1))

#if 0 // TODO
    uint32_t addr_offset = THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    setgpr(p_gpr_unpack::TILE_SIZE, get_l1_tile_size(unpa_src_format));
    setgpr(p_gpr_unpack::TILE_OFFSET, 0);
    SETDMAREG(0, 0, 0, LO_16(p_gpr_unpack::TILE_OFFSET));
    REG2FLOP(1, 0, 0, 0, addr_offset, p_gpr::ZERO);
#else // temporary solution
    m_tile_size = get_l1_tile_size(unpa_src_format);
    set_offset_addr(0, 0, 0);
#endif
}

void Unpack::unpack_untilize(uint32_t base_addr, uint32_t block_c_tiles) {
    unpack_untilize_pass(true, base_addr, block_c_tiles);
    unpack_untilize_pass(false, base_addr, block_c_tiles); 
}

void Unpack::unpack_untilize_uninit(DataFormat unpa_src_format, DataFormat unpa_dst_format) {
    // SKIPPED: wait_for_idle

    configure_unpack_AB(
        unpa_src_format, 
        unpa_dst_format,
        unpa_src_format, 
        unpa_dst_format,
        16, 
        16, 
        false, 
        true,    // transpose_xy_srca_en - WHY?
        false); 
}

void Unpack::unpack_untilize_mop_config() {
    auto unpack_srca =
        TT_OP(UNPACR(SrcA, 0b01000001, 0, 0, 0, 0, 0, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));

    auto unpack_addcr = TT_OP(ADDRCRZW(0b001, 0, 0, 0, 0, 0b0001));

#if 0 // TODO
    uint32_t TILE_OFFSET = p_gpr_unpack::TILE_OFFSET;
    uint32_t TILE_SIZE = p_gpr_unpack::TILE_SIZE;
    uint32_t addr_offset = THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    auto unpack_addr_offset = TT_OP(ADDDMAREG(0, TILE_OFFSET, TILE_OFFSET, TILE_SIZE));
    auto unpack_wr_addr_offset = TT_OP(REG2FLOP(1, 0, 0, 0, addr_offset, TILE_OFFSET);
#else // temporary solution
    auto unpack_addr_offset = TT_OP(incr_offset_addr(0, 0, m_tile_size));
    auto unpack_wr_addr_offset = TT_OP(NOP());
#endif

    MopTemplate0 m;
    m.unpack_b = true;
    m.unpack_halo = true;
    m.a0_instr = unpack_srca;
    m.a1_instr = unpack_srca;
    m.a2_instr = unpack_addr_offset;
    m.a3_instr = unpack_wr_addr_offset;
    m.b_instr = unpack_addcr;
    set_mop_templ_0(m);
}

void Unpack::unpack_untilize_pass(
        bool first_pass, 
        uint32_t base_addr, 
        uint32_t block_c_tiles) {
    uint32_t rem_blocks_in_row = block_c_tiles;

    // Program srcA base address

    // Clear l1 addr y cnt
    SETADCXY(0b001, 0, 0, 0, 0, 0b0010);
    if (first_pass) {
        // Select bootom faces in the 2nd pass
        SETADC(p_setadc::UNP0, p_setadc::CH_0, p_setadc::SET_Z, 0);
    } else {
        // Select bottom faces in the 2nd pass
        SETADC(p_setadc::UNP0, p_setadc::CH_0, p_setadc::SET_Z, 2);
    }

    start_unpack_A_untilize(base_addr);

    uint32_t face_2xr_cnt = 0;
    for (uint32_t r = 0; r < FACE_HEIGHT; r++) {
        // reset remaining blocks in row
        rem_blocks_in_row = block_c_tiles;

        do {
            if (face_2xr_cnt + rem_blocks_in_row >= FACE_HEIGHT / 2) {
                // Run MOP
                MOP(0, 8 - face_2xr_cnt - 1, 0);
                UNPACR(SrcA, 0b0, 0, 0, 0, 0, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                // borrowed from 0.33
                UNPACR_NOP(SrcB, p_unpacr_nop::UNP_ZEROSRC);
                UNPACR_NOP(SrcB, p_unpacr_nop::UNP_SET_DVALID);
                SETADCXY(0b001, 0, 0, 0, 0, 0b1000);  // Clear srcA addr y cnt
                rem_blocks_in_row -= (8 - face_2xr_cnt);
                face_2xr_cnt = 0;
            } else {
                MOP(0, rem_blocks_in_row - 1, 0);
                face_2xr_cnt += rem_blocks_in_row;
                rem_blocks_in_row = 0;
            }
        } while (rem_blocks_in_row > 0);

#if 0 // TODO
        uint32_t addr_offset = THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        // Clear offset pointer
        SETDMAREG(0, 0, 0, LO_16(p_gpr_unpack::TILE_OFFSET));  
        // Clear offset register
        REG2FLOP(1, 0, 0, 0, addr_offset, p_gpr::ZERO);
#else // temporary solution
        set_offset_addr(0, 0, 0);
#endif
        // inc l1 addr y cnt
        INCADCXY(0b001, 0, 0, 1, 0);
    }

    end_unpack_A_untilize();
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

