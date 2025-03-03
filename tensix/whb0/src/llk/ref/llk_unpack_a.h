// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

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

template <
    BroadcastType BType = BroadcastType::NONE, 
    bool acc_to_dest = false, 
    EltwiseBinaryReuseDestType binary_reuse_dest = EltwiseBinaryReuseDestType::NONE>
inline void llk_unpack_A_mop_config(bool transpose_of_faces, uint32_t operand_id) {
    static_assert(
        !(BType != BroadcastType::NONE && 
            acc_to_dest && 
            binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCB), 
        "Not supported configuration!");

    uint32_t num_faces = get_num_faces(operand_id);
    // in TT_OP_UNPACR: ovrd_thread_id = 1, set_dat_valid = 1
    static constexpr auto unpack_srca = 
        TT_OP_UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    static constexpr auto unpack_srca_set_dvalid = 
        TT_OP_UNPACR_NOP(SrcA, p_unpacr_nop::UNP_ZEROSRC_SET_DVALID);
    static constexpr auto unpack_srcb = 
        TT_OP_UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    static constexpr auto unpack_srcb_inc_z_0 = 
        TT_OP_UNPACR(SrcB, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    static constexpr auto unpack_srcb_zerosrc = 
        TT_OP_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_ZEROSRC);
    // WA for https://yyz-gitlab.local.tenstorrent.com/tenstorrent/budabackend/-/issues/1230
    static constexpr auto unpack_srcb_set_dvalid = 
        TT_OP_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_SET_DVALID);
    // set srcA ch0_z = 1
    static constexpr auto srca_set_z_1 = 
        TT_OP_SETADCZW(p_setadc::UNP_A, 0, 0, 0, 1, 0b0001); 
    // set srcB ch0_z = 2
    static constexpr auto srcb_set_z_2 = 
        TT_OP_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 2, 0b0001); 
    // set srcB ch0_z = 0
    static constexpr auto srcb_clear_z = 
        TT_OP_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 0, 0b0001); 

    if constexpr (BType == BroadcastType::COL) {
        if constexpr (acc_to_dest) {
            // TODO: add support for num_faces, add support for dest to srcB
            constexpr uint32_t innerloop = 1;
            constexpr uint32_t outerloop = 2; 
            ckernel_template tmp(
                outerloop, innerloop, unpack_srca_set_dvalid, unpack_srca_set_dvalid);
            tmp.set_start_op(unpack_srcb);
            tmp.set_end_op(srcb_set_z_2);
            tmp.program(instrn_buffer);
        } else {
            // TODO: add support for num_faces, add support for dest to srcB
            constexpr uint32_t innerloop = 1;
            constexpr uint32_t outerloop = 1;
            ckernel_template tmp(outerloop, innerloop, srcb_set_z_2, unpack_srcb);
            tmp.set_start_op(unpack_srcb);
            tmp.set_end_op(unpack_srca_set_dvalid);
            tmp.program(instrn_buffer);
        }
    } else if constexpr (BType == BroadcastType::ROW) {
        // TODO: add support for num_faces
        constexpr uint32_t innerloop = 2;
        constexpr uint32_t outerloop = 2;
        if constexpr (acc_to_dest) {
            ckernel_template tmp(outerloop, innerloop, unpack_srcb, unpack_srca_set_dvalid);
            tmp.set_end_op(srcb_clear_z);
            tmp.program(instrn_buffer);
        } else {
            ckernel_template tmp(outerloop, innerloop, unpack_srcb);
            tmp.set_end_op(srcb_clear_z);
            tmp.program(instrn_buffer);
        }
    } else if constexpr (BType == BroadcastType::SCALAR) {
        static_assert(!acc_to_dest && 
            "accumulate into dest with broadcast scaler is not supported!");
        uint32_t outerloop = num_faces;
        constexpr uint32_t innerloop = 1;
        ckernel_template tmp(outerloop, innerloop, unpack_srcb_inc_z_0);
        tmp.program(instrn_buffer);
    } else {
        if (transpose_of_faces) {
            constexpr uint32_t replay_buf_len = 3;
            TTI_REPLAY(0, replay_buf_len, 0, 1);
            TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_ZEROSRC);
            TTI_UNPACR_NOP(SrcB, p_unpacr_nop::UNP_SET_DVALID);
            if (num_faces > 2) {
                // inc srcA ch0_z+=2
                TTI_UNPACR(SrcA, 0b10, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            } else {
                // inc srcA ch0_z+=1
                TTI_UNPACR(SrcA, 0b01, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            }
            uint32_t outerloop = (num_faces < 4) ? 1 : 2;
            uint32_t innerloop = (num_faces < 2) ? 1 : 2;
            // Unpack faces 0/2 && 1/3 to srcA or 0/1 for 2 face tile
            ckernel_template tmp(outerloop, innerloop, TT_OP_REPLAY(0, replay_buf_len, 0, 0));
            if (num_faces > 2) {
                tmp.set_end_op(srca_set_z_1);
            }
            tmp.program(instrn_buffer);
        } else {
            if constexpr (acc_to_dest) {
                static constexpr auto unpack_srca_reuse = 
                    (binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCA) ?
                        unpack_srca_set_dvalid : unpack_srca;
                static constexpr auto unpack_srcb_reuse = 
                    (binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCB) ?
                        unpack_srcb_zerosrc  : unpack_srcb;
                uint32_t outerloop = num_faces;
                constexpr uint32_t innerloop = 1;
                ckernel_template tmp(outerloop, innerloop, unpack_srca_reuse, unpack_srcb_reuse);
                if constexpr (binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCB) {
                    tmp.set_end_op(unpack_srcb_set_dvalid);
                }
                tmp.program(instrn_buffer);
            } else {
                uint32_t outerloop = num_faces;
                constexpr uint32_t innerloop = 1;
                ckernel_template tmp(
                    outerloop, innerloop, unpack_srcb_zerosrc, unpack_srcb_set_dvalid);
                tmp.set_start_op(unpack_srca);
                tmp.program(instrn_buffer);
            }
        }
    }
}

template <bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_A_hw_configure(
        const llk_unpack_A_params_t *unpack_A_params, int within_face_16x16_transpose = 0) {
    constexpr bool is_row_pool = false;
    uint32_t unpA_operand_id = get_operand_id(unpack_A_params->unpA_operand);
    uint32_t unpA_num_faces = get_num_faces(unpA_operand_id);
    uint32_t unpA_face_r_dim = get_face_r_dim(unpA_operand_id);
    configure_unpack_AB(
        unpA_operand_id, 
        unpA_operand_id,
        unpA_face_r_dim, 
        unpA_face_r_dim, 
        is_row_pool, 
        within_face_16x16_transpose, 
        is_fp32_dest_acc_en, 
        srnd_fpu_en, 
        unpA_num_faces, 
        unpA_num_faces);
}

template <bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_A_hw_configure_disaggregated(
        uint32_t unpA_operand, int within_face_16x16_transpose = 0) {
    llk_unpack_A_params_t unpack_A_params = {
        .unpA_operand = unpA_operand
    };
    llk_unpack_A_hw_configure<is_fp32_dest_acc_en, srnd_fpu_en>(
        &unpack_A_params, within_face_16x16_transpose);
}

template <
    BroadcastType BType = BroadcastType::NONE, 
    bool acc_to_dest = false, 
    EltwiseBinaryReuseDestType binary_reuse_dest = EltwiseBinaryReuseDestType::NONE>
inline void llk_unpack_A_init(
        uint32_t transpose_of_faces = 0, 
        uint32_t within_face_16x16_transpose = 0, 
        uint32_t operand = 0) {
    cfg_reg_rmw_tensix<THCON_SEC0_REG2_Haloize_mode_RMW>(within_face_16x16_transpose);

    uint32_t operand_id = get_operand_id(operand);
    uint32_t face_r_dim = get_face_r_dim(operand_id);

    constexpr uint32_t UNP_SEL = 
        (BType == BroadcastType::NONE) ? p_setadc::UNP_A : p_setadc::UNP_B;
    config_face_dim<false, UNP_SEL>(face_r_dim);
    llk_unpack_A_mop_config<BType, acc_to_dest, binary_reuse_dest>(
        (transpose_of_faces > 0), operand_id);
}

template <
    BroadcastType BType = BroadcastType::NONE, 
    bool acc_to_dest = false, 
    EltwiseBinaryReuseDestType binary_reuse_dest = EltwiseBinaryReuseDestType::NONE>
inline void llk_unpack_A(
        uint32_t operand, 
        uint32_t tile_index, 
        bool transpose_of_faces = 0 /*not used*/) {
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

    // Trisc::SEMPOST for context acquire
    semaphore_post(semaphore::UNPACK_SYNC);

    // Program srcA and srcB base addresses
    if (unp_cfg_context == 0) {
        if constexpr (BType == BroadcastType::NONE && !acc_to_dest) {
            cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address;
        } else {
            if constexpr(binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCB) {
                cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address;
            }
            cfg[THCON_SEC1_REG3_Base_address_ADDR32] = address;
        }
    } else {
        if constexpr (BType == BroadcastType::NONE && !acc_to_dest) {
            cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address;
        } else {
            if constexpr(binary_reuse_dest == EltwiseBinaryReuseDestType::DEST_TO_SRCB) {
                cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address;
            }
            cfg[THCON_SEC1_REG3_Base_cntx1_address_ADDR32] = address;
        }
    }

    // Run MOP
    ckernel::ckernel_template::run(instrn_buffer);

    // T6::SEMGET for context release
    t6_semaphore_get(semaphore::UNPACK_SYNC);

    // Switch unpacker config context
    switch_config_context();
}

END_NS_LLK

