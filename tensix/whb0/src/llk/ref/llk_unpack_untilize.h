// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "cfg_defines.h"

#include "ckernel_core.h"
#include "ckernel_instr_params.h"
#include "ckernel_gpr_map.h"
#include "ckernel_mem_ptr.h"
#include "ckernel.h"
#include "cunpack_common.h"

#include "llk_defs.h"
#include "llk_param_structs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::unpacker;

inline void llk_unpack_untilize_mop_config() {
    constexpr uint32_t replay_buf_len = 5;
    constexpr uint32_t flop_index = 
        THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    TTI_REPLAY(0, replay_buf_len, 0, 1);
    TTI_UNPACR(SrcA, 0b01000001, 0, 0, 0, 1, 0, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    TTI_UNPACR(SrcA, 0b01000001, 0, 0, 0, 1, 0, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    TTI_ADDDMAREG(0, p_gpr_unpack::TILE_OFFSET, p_gpr_unpack::TILE_OFFSET, p_gpr_unpack::TILE_SIZE);
    TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::TILE_OFFSET);
    TTI_ADDRCRZW(0b001, 0, 0, 0, 0, 0b0001);
    ckernel_unpack_template tmp = ckernel_unpack_template(
        false,
        false,
        TT_OP_REPLAY(0, replay_buf_len, 0, 0),
        NULL_OP,
        NULL_OP,
        NULL_OP,
        NULL_OP,
        NULL_OP,
        NULL_OP);
    tmp.program(instrn_buffer);
}

inline void llk_unpack_untilize_hw_configure(
        const llk_unpack_untilize_params_t *unpack_untilize_params) {
    configure_unpack_AB(
        get_operand_id(unpack_untilize_params->unpA_operand), 
        get_operand_id(unpack_untilize_params->unpA_operand), 
        1);
}

inline void llk_unpack_untilize_hw_configure_disaggregated(uint32_t unpA_operand) {
    llk_unpack_untilize_params_t unpack_untilize_params = {
        .unpA_operand = unpA_operand,
    };
    llk_unpack_untilize_hw_configure(&unpack_untilize_params);
}

inline void llk_unpack_untilize_init(uint32_t operand) {
    llk_unpack_untilize_mop_config();

    wait_for_idle();

    // Override default settings
    uint32_t input = get_operand_id(operand);

    DataFormat src_df = get_unpack_src_format(input);
    DataFormat dst_df = get_unpack_dst_format(input);

    uint32_t unpA_ch1_x_stride = get_df_item_size(dst_df);
    uint32_t unpA_ch1_y_stride = 16 * unpA_ch1_x_stride;
    cfg_reg_rmw_tensix<
            UNP0_ADDR_CTRL_XY_REG_1_Ystride_ADDR32, 
            UNP0_ADDR_CTRL_XY_REG_0_Ystride_SHAMT, 
            UNP0_ADDR_CTRL_XY_REG_1_Ystride_MASK>(
        unpA_ch1_y_stride);
    cfg_reg_rmw_tensix<
            THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32, 0, 0xFFFF>(
        FACE_WIDTH | (FACE_WIDTH << 16));
    cfg_reg_rmw_tensix<THCON_SEC0_REG0_TileDescriptor_ADDR32, 16, 0xFFFF00>(FACE_WIDTH);
    cfg_reg_rmw_tensix<THCON_SEC0_REG0_TileDescriptor_ADDR32 + 1, 0, 0xFFFF>(FACE_HEIGHT);

    constexpr uint32_t flop_index = 
        THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    regfile[p_gpr_unpack::TILE_SIZE] = get_cb_interface(input).fifo_page_size;
    regfile[p_gpr_unpack::TILE_OFFSET] = 0;
    sync_regfile_write(p_gpr_unpack::TILE_OFFSET);
    TTI_SETDMAREG(0, 0, 0, LO_16(p_gpr_unpack::TILE_OFFSET));
    TTI_SETDMAREG(0, 0, 0, HI_16(p_gpr_unpack::TILE_OFFSET));
    TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr::ZERO);
}

template <bool first_pass = true>
inline void llk_unpack_untilize_(uint32_t operand, uint32_t block_tile_cols) {
    uint32_t input = get_operand_id(operand);
    uint32_t base_address = get_cb_interface(input).fifo_rd_ptr - 1;
    uint32_t rem_blocks_in_row = block_tile_cols;

    // get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    // Clear l1 addr y cnt
    TTI_SETADCXY(0b001, 0, 0, 0, 0, 0b0010);
    if constexpr (first_pass) {
        // Select bootom faces in the 2nd pass
        TT_SETADC(p_setadc::UNP0, p_setadc::CH_0, p_setadc::SET_Z, 0);
    } else {
        // Select bootom faces in the 2nd pass
        TT_SETADC(p_setadc::UNP0, p_setadc::CH_0, p_setadc::SET_Z, 2);
    }

    // Wait for free context
    wait_for_next_context(1);

    // Trisc::SEMPOST for context acquire
    semaphore_post(semaphore::UNPACK_SYNC);

    cfg[THCON_SEC0_REG3_Base_address_ADDR32] = base_address;

    uint32_t face_2xr_cnt = 0;
    for (uint32_t r = 0; r < FACE_HEIGHT; r++) {
        // reset remaining blocks in row
        rem_blocks_in_row = block_tile_cols;

        do {
            if (face_2xr_cnt + rem_blocks_in_row >= FACE_HEIGHT / 2) {
                TT_MOP(0, 8 - face_2xr_cnt - 1, 0);
                // set data valid
                TTI_UNPACR(SrcA, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);

                TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_ZEROSRC);
                TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_SET_DVALID);
                // Clear srcA addr y cnt
                TTI_SETADCXY(0b001, 0, 0, 0, 0, 0b1000);
                rem_blocks_in_row -= (8 - face_2xr_cnt);
                face_2xr_cnt = 0;
            } else {
                TT_MOP(0, rem_blocks_in_row - 1, 0);
                face_2xr_cnt += rem_blocks_in_row;
                rem_blocks_in_row = 0;
            }
        } while (rem_blocks_in_row > 0);

        // Clear offset pointer
        constexpr uint32_t flop_index = 
            THCON_SEC0_REG7_Offset_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        TTI_SETDMAREG(0, 0, 0, LO_16(p_gpr_unpack::TILE_OFFSET));
        TTI_SETDMAREG(0, 0, 0, HI_16(p_gpr_unpack::TILE_OFFSET));
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr::ZERO);
        // inc l1 addr y cnt
        TTI_INCADCXY(0b001, 0, 0, 1, 0);
    }

    // T6::SEMGET for context release
    t6_semaphore_get(semaphore::UNPACK_SYNC);

}

inline void llk_unpack_untilize(uint32_t operand, uint32_t block_c_tiles) {
    llk_unpack_untilize_<true>(operand, block_c_tiles);
    llk_unpack_untilize_<false>(operand, block_c_tiles);
}

inline void llk_unpack_untilize_uninit(uint32_t operand) {
    wait_for_idle();

    configure_unpack_AB(operand, operand, 16, 16, false, false);
}

END_NS_LLK

