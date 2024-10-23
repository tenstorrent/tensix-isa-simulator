#pragma once

#include <cstdint>

#include "cfg_defines.h"

#include "ckernel_core.h"
#include "ckernel_instr_params.h"
#include "ckernel_mem_ptr.h"
#include "cunpack_common.h"

#include "llk_defs.h"
#include "llk_param_structs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::unpacker;

inline void llk_unpack_tilize_mop_config() {
    constexpr uint32_t replay_buf_len = 3;

    TTI_REPLAY(0, replay_buf_len, 0, 1);

    TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_ZEROSRC);
    TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_SET_DVALID);
    // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
    TTI_UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);

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

template <bool is_fp32_dest_acc_en = false>
inline void llk_unpack_tilize_hw_configure(
        const llk_unpack_tilize_params_t *unpack_tilize_params) {
    constexpr bool is_row_pool = false;
    constexpr uint32_t srca_height = 16;
    constexpr uint32_t srcb_height = 16;
    constexpr bool transpose_xy_srca = false;
    uint32_t srca_operand = get_operand_id(unpack_tilize_params->unpA_operand);
    configure_unpack_AB(
        srca_operand, 
        srca_operand, 
        srca_height, 
        srcb_height, 
        is_row_pool, 
        transpose_xy_srca, 
        is_fp32_dest_acc_en);
}

template <bool is_fp32_dest_acc_en = false>
inline void llk_unpack_tilize_hw_configure_disaggregated(
        uint32_t unpA_operand, uint32_t unpA_block_ct_dim) {
    llk_unpack_tilize_params_t unpack_tilize_params = {
        .unpA_operand = unpA_operand,
        .unpA_block_c_dim = unpA_block_ct_dim * TILE_WIDTH,
    };
    llk_unpack_tilize_hw_configure<is_fp32_dest_acc_en>(&unpack_tilize_params);
}

inline void llk_unpack_tilize_init(uint32_t srca_operand, uint32_t unpA_block_c_tiles) {
    llk_unpack_tilize_mop_config();

    wait_for_idle();

    // Override default settings
    MEM_PTR_T cfg = get_cfg_pointer();

    DataFormat src_df = get_unpack_src_format(srca_operand);
    DataFormat dst_df = get_unpack_dst_format(srca_operand);

    unpack_config_u config = {0};

    config.f.out_data_format = (uint32_t)dst_df;
    config.f.throttle_mode = 2;
    config.f.tileize_mode = 1;
    config.f.shift_amount = 
        (SCALE_DATUM_SIZE((uint32_t)src_df, unpA_block_c_tiles * TILE_WIDTH)) >> 4;

    cfg[THCON_SEC0_REG2_Out_data_format_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32] = 16 | (16 << 16);
}

inline void llk_unpack_tilize(uint32_t operand, uint32_t tile_index, uint32_t block_ct_dim) {
    uint32_t input = get_operand_id(operand);
    // Remove header size added by descriptor
    uint32_t base_address = get_cb_interface(input).fifo_rd_ptr - 1;  
    DataFormat src_df = get_unpack_src_format(input);
    // Each iteration unpacks 2 16 x 16 faces (1st 0,1 2nd 2,3)
    // Offset address is in 16B words
    // Datum count = tile_index * 16 ( / 16 to get word count)
    uint32_t top_face_offset_address = SCALE_DATUM_SIZE((uint32_t)src_df, tile_index) << 1;

    // * 16 rows / 16 to get 16B word aligned address
    uint32_t bot_face_offset_address = 
        SCALE_DATUM_SIZE((uint32_t)src_df, block_ct_dim * TILE_WIDTH);

    // get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    for (uint32_t n = 0; n < 2; n++) {
        uint32_t address = 
            base_address + top_face_offset_address + ((n == 1) ? bot_face_offset_address : 0);

        // Clear z/w start counters
        TTI_SETADCZW(0b001, 0, 0, 0, 0, 0b1111);

        // Wait for free context
        wait_for_next_context(2);

        // Trisc::SEMPOST for context acquire
        semaphore_post(semaphore::UNPACK_SYNC);

        if (unp_cfg_context == 0) {
            cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address;
        } else {
            cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address;
        }

        // Run MOP
        mop_run(0, 2);

        // T6::SEMGET for context release
        t6_semaphore_get(semaphore::UNPACK_SYNC);

        // Switch unpacker config context
        switch_config_context();
    }
}

inline void llk_unpack_tilize_block(uint32_t operand, uint32_t block_c_tiles) {
    uint32_t input = get_operand_id(operand);
    for (uint32_t tile_index = 0; tile_index < block_c_tiles; tile_index++) {
        llk_unpack_tilize(operand, tile_index, block_c_tiles);
    }
}

inline void llk_unpack_tilize_uninit() {
    // Undo tilize if added
    wait_for_idle();

    MEM_PTR_T cfg = get_cfg_pointer();
    unpack_config_u config;
    config.val[0] = cfg[THCON_SEC0_REG2_Out_data_format_ADDR32 + 0];
    config.f.tileize_mode = 0;
    config.f.shift_amount = 0;
    cfg[THCON_SEC0_REG2_Out_data_format_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32] = 256 | (256 << 16);
}

END_NS_LLK

