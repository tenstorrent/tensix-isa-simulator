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
#include "cunpack_common.h"

#include "llk_defs.h"
#include "llk_param_structs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::unpacker;

template <PoolType type, ReduceDim dim>
inline void llk_unpack_reduce_mop_config() {
    static constexpr auto unpack_srca =
        TT_OP_UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    static constexpr auto unpack_zerosrca = 
        TT_OP_UNPACR_NOP(p_unpacr_nop::UNP0, p_unpacr_nop::UNP_ZEROSRC);
    static constexpr auto unpack_srcb =
        TT_OP_UNPACR(SrcB, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    ckernel_unpack_template tmp = ckernel_unpack_template(
        true,  // src B
        true,  // halo - just used for 4 unpacks
        unpack_zerosrca,
        unpack_srca,
        TT_OP_NOP,
        TT_OP_NOP,
        NULL_OP,
        unpack_srcb,
        NULL_OP);
    tmp.program(instrn_buffer);
}

template <
    PoolType type, 
    ReduceDim dim, 
    bool is_fp32_dest_acc_en = false, 
    bool srnd_fpu_en = false>
inline void llk_unpack_reduce_hw_configure(
    const llk_unpack_reduce_params_t *unpack_reduce_params, float const_mult) {

    constexpr uint32_t srca_height = 16;
    constexpr uint32_t srcb_height = 16;
    constexpr bool is_row_pool = true;
    constexpr bool transpose_xy_per_face = (ReduceDim::REDUCE_ROW == dim);

    configure_unpack_AB(
        get_operand_id(unpack_reduce_params->unpA_operand),
        get_operand_id(unpack_reduce_params->unpA_operand),
        srca_height,
        srcb_height,
        is_row_pool,
        transpose_xy_per_face,
        is_fp32_dest_acc_en,
        srnd_fpu_en);

    if constexpr (type != PoolType::MAX) {
        union {
            float f;
            uint32_t u;
        } f2u = {.f = const_mult};

        // Load const into L1 buffer
        uint32_t *l1_buffer = get_l1_buffer_ptr();
        for (int i = 0; i < 16; i++) {
            l1_buffer[i] = f2u.u;
        }
    }
}

template <
    PoolType type, 
    ReduceDim dim, 
    bool is_fp32_dest_acc_en = false, 
    bool srnd_fpu_en = false>
inline void llk_unpack_reduce_hw_configure_disaggregated(uint32_t unpA_operand, float mult) {
    llk_unpack_reduce_params_t unpack_reduce_params = {.unpA_operand = unpA_operand};
    llk_unpack_reduce_hw_configure<type, dim, is_fp32_dest_acc_en, srnd_fpu_en>(
        &unpack_reduce_params, mult);
}

template <PoolType type, ReduceDim dim>
inline void llk_unpack_reduce_init(uint32_t within_face_16x16_transpose = 0) {
    llk_unpack_reduce_mop_config<type, dim>();

    uint32_t unpack_src_df = (uint32_t)DataFormat::Float32;
    uint32_t unpack_dst_df = 
        ((((uint32_t)get_unpack_dst_format(0) >> 2) & 0x1) != 0) ? 
             (uint32_t)DataFormat::Float16_b : (uint32_t)DataFormat::Float16;

    cfg_reg_rmw_tensix<ALU_FORMAT_SPEC_REG1_SrcB_RMW>(unpack_dst_df);

    // REDUCE_ROW requires transpose itself; additionaly, 
    // within_face_16x16_transpose flag could require transpose;
    // if we have the flag set with REDUCE_ROW, we don't need to do anything
    cfg_reg_rmw_tensix<THCON_SEC0_REG2_Haloize_mode_RMW>(
        (dim == ReduceDim::REDUCE_ROW) ? 
            !within_face_16x16_transpose : within_face_16x16_transpose);

    TTI_SETADCXX(0b11, FACE_WIDTH * FACE_HEIGHT - 1, 0x0);

    // TODO: Implement support for these registers, sync with unpack_config
    cfg_reg_rmw_tensix<THCON_SEC1_REG0_TileDescriptor_ADDR32, 0, 0xf>(unpack_src_df);
    cfg_reg_rmw_tensix<THCON_SEC1_REG2_Out_data_format_RMW>(unpack_dst_df);

    TTI_WRCFG(
        p_gpr_unpack::L1_BUFFER_ADDR, 
        p_cfg::WRCFG_32b, 
        THCON_SEC1_REG3_Base_address_ADDR32);
    TTI_WRCFG(
        p_gpr_unpack::L1_BUFFER_ADDR, 
        p_cfg::WRCFG_32b, 
        THCON_SEC1_REG3_Base_cntx1_address_ADDR32);
    TTI_NOP; 
    TTI_NOP;
}

template <PoolType type, ReduceDim dim>
inline void llk_unpack_reduce(uint32_t operand, uint32_t tile_index) {
    uint32_t input = get_operand_id(operand);
    CBInterface &cb = get_cb_interface(input);
    uint32_t base_address = cb.fifo_rd_ptr;
    uint32_t offset_address = cb.fifo_page_size * tile_index;
    // note: unpacker is programmed to automatically skip the tile header (+1)
    // since there is no tile header, we need to -1 the address (in terms of 16B words),
    // to offet unpacker's automatic +1
    uint32_t address = base_address + offset_address - 1;

    // Clear z/w start counters
    TTI_SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    // get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    // Wait for free context
    wait_for_next_context(2);

    // Load only 16 datums into srcB
    TTI_SETADCXX(p_setadc::UNP1, DATUMS_PER_ROW - 1, 0x0);

    // Trisc::SEMPOST for context acquire
    semaphore_post(semaphore::UNPACK_SYNC);

    // Program srcA base address
    if (unp_cfg_context == 0) {
        cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address;
    } else {
        cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address;
    }

    // Run MOP
    mop_run(0, 4);

    // Restore face height
    TTI_SETADCXX(p_setadc::UNP1, FACE_HEIGHT * 16 - 1, 0x0);

    // T6::SEMGET for context release
    t6_semaphore_get(semaphore::UNPACK_SYNC);

    // Switch unpacker config context
    switch_config_context();
}

END_NS_LLK

