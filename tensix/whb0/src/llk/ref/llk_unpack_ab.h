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

template <BroadcastType BType = BroadcastType::NONE>
inline void llk_unpack_AB_mop_config(bool transpose_of_faces = false, uint32_t operand_id = 0) {
    static constexpr auto unpack_srca =
        TT_OP_UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    static constexpr auto unpack_srcb =
        TT_OP_UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
    // if narrow tile read face 0 twice for row broadcast
    // or read face 0 and 1 for col broadcast
    uint32_t num_faces = get_num_faces(operand_id);
    bool narrow_tile = get_narrow_tile(operand_id); 

    if constexpr (BType == BroadcastType::COL) {
        static constexpr auto unpack_srcb_set_z = TT_OP_SETADCZW(0b010, 0, 0, 0, 2, 0b0001);
        uint32_t outerloop = (num_faces < 4) ? 1 : 2;
        uint32_t innerloop = (num_faces < 2) ? 1 : 2;
        ckernel_template tmp(outerloop, innerloop, unpack_srca);
        tmp.set_start_op(unpack_srcb);
        if (narrow_tile) {
            tmp.set_end_op(unpack_srcb); // Read face 1
        } else {
            tmp.set_end_op(unpack_srcb_set_z);
        }
        tmp.program(instrn_buffer);
    } else if constexpr (BType == BroadcastType::ROW) {
        static constexpr auto unpack_srcb_clear_z  = TT_OP_SETADCZW(0b010, 0, 0, 0, 0, 0b0001);
        static constexpr auto unpack_srcb_no_z_inc = 
            TT_OP_UNPACR(SrcB, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1);
        uint32_t outerloop = (num_faces < 4) ? 1 : 2;
        uint32_t innerloop = (num_faces < 2) ? 1 : 2;
        ckernel_template tmp(
            outerloop, 
            innerloop, 
            narrow_tile ? unpack_srcb_no_z_inc : unpack_srcb, 
            unpack_srca);
        tmp.set_end_op(unpack_srcb_clear_z);
        tmp.program(instrn_buffer);
    } else if constexpr (BType == BroadcastType::SCALAR) {
        uint32_t outerloop = 1;
        uint32_t innerloop = num_faces;
        ckernel_template tmp(outerloop, innerloop, unpack_srca);
        tmp.set_start_op(unpack_srcb);
        tmp.program(instrn_buffer);
    } else {
        if (transpose_of_faces) {
            // set z to 1
            static constexpr auto srca_set_z = TT_OP_SETADCZW(0b001, 0, 0, 0, 1, 0b0001); 
            // inc z by 2
            static constexpr auto unpack_srca_skip_z =
                TT_OP_UNPACR(SrcA, 0b10, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1); 
            uint32_t outerloop = (num_faces < 4) ? 1 : 2;
            uint32_t innerloop = (num_faces < 2) ? 1 : 2;
            ckernel_template tmp(
                outerloop, 
                innerloop, 
                (num_faces < 4) ? unpack_srca : unpack_srca_skip_z, 
                unpack_srcb);
            tmp.set_end_op(srca_set_z);
            tmp.program(instrn_buffer);
        } else {
            uint32_t outerloop = 1;
            uint32_t innerloop = num_faces;
            ckernel_template tmp(outerloop, innerloop, unpack_srca, unpack_srcb);
            tmp.program(instrn_buffer);
        }
    }
}

template <bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_AB_hw_configure(
        const llk_unpack_AB_params_t *unpack_AB_params, 
        int within_face_16x16_transpose = 0) {
    constexpr bool is_row_pool = false;
    // In0 -> unpA
    // In1 -> unpB
    uint32_t unpA_operand_id = get_operand_id(unpack_AB_params->unpA_operand);
    uint32_t unpB_operand_id = get_operand_id(unpack_AB_params->unpB_operand);

    // unpA -> srcA
    // unpB -> srcB
    // num faces in unpA and unpB are the same
    uint32_t num_faces = get_num_faces(unpA_operand_id);

    // face r dim in unpA and unpB are the same
    uint32_t face_r_dim = get_face_r_dim(unpA_operand_id);

    configure_unpack_AB(
        unpA_operand_id, 
        unpB_operand_id,
        face_r_dim, 
        face_r_dim, 
        is_row_pool, 
        within_face_16x16_transpose, 
        is_fp32_dest_acc_en, 
        srnd_fpu_en, 
        num_faces, 
        num_faces);
}

template <bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_AB_hw_configure_disaggregated(
        uint32_t unpA_operand, 
        uint32_t unpB_operand, 
        int within_face_16x16_transpose = 0) {
    llk_unpack_AB_params_t unpack_AB_params = {
        .unpA_operand = unpA_operand, 
        .unpB_operand = unpB_operand
    };
    llk_unpack_AB_hw_configure<is_fp32_dest_acc_en, srnd_fpu_en>(
        &unpack_AB_params, within_face_16x16_transpose);
}

template <BroadcastType BType = BroadcastType::NONE>
inline void llk_unpack_AB_init(
        uint32_t unpA_operand, 
        uint32_t unpB_operand, 
        uint32_t transpose = 0, 
        uint32_t acc_to_dest = 0) {
    uint32_t unpA_operand_id = get_operand_id(unpA_operand);

    // Need to be able to configure tranpose srca for fused ops
    // transpose within the face
    cfg_reg_rmw_tensix<THCON_SEC0_REG2_Haloize_mode_RMW>(transpose);

    // face r dim in unpA and unpB are the same
    uint32_t face_r_dim = get_face_r_dim(unpA_operand_id); 

    constexpr uint32_t UNP_SEL = p_setadc::UNP_AB;
    config_face_dim<false, UNP_SEL>(face_r_dim);

    // transpose of faces 0, 2, 1, 3
    llk_unpack_AB_mop_config<BType>((transpose > 0), unpA_operand_id); 
}

template <BroadcastType BType = BroadcastType::NONE>
inline void llk_unpack_AB(
        uint32_t operandA, 
        uint32_t operandB, 
        uint32_t tile_index_a, 
        uint32_t tile_index_b, 
        bool transpose_of_faces = 0 /*not used*/) {
    uint32_t inputA = get_operand_id(operandA);
    uint32_t inputB = get_operand_id(operandB);
    CBInterface &cb_a = get_cb_interface(inputA);
    CBInterface &cb_b = get_cb_interface(inputB);
    uint32_t base_address_a = cb_a.fifo_rd_ptr;
    uint32_t offset_address_a = cb_a.fifo_page_size * tile_index_a;
    uint32_t base_address_b = cb_b.fifo_rd_ptr;
    uint32_t offset_address_b = cb_b.fifo_page_size * tile_index_b;

    // note: unpacker is programmed to automatically skip the tile header (+1)
    // since there is no tile header, we need to -1 the address (in terms of 16B words),
    // to offet unpacker's automatic +1
    uint32_t address_a = base_address_a + offset_address_a - 1;
    uint32_t address_b = base_address_b + offset_address_b - 1;

    // reset counters
    TTI_SETADCZW(0b011, 0, 0, 0, 0, 0b1111); 

    // get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    // Wait for free context
    wait_for_next_context(2);

    // Program srcA and srcB base addresses
    if (unp_cfg_context == 0) {
        cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address_a;
        cfg[THCON_SEC1_REG3_Base_address_ADDR32] = address_b;
    } else {
        cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address_a;
        cfg[THCON_SEC1_REG3_Base_cntx1_address_ADDR32] = address_b;
    }

    // Trisc::SEMPOST for context acquire
    semaphore_post(semaphore::UNPACK_SYNC);

    // Run MOP
    ckernel::ckernel_template::run(instrn_buffer);

    // T6::SEMGET for context release
    t6_semaphore_get(semaphore::UNPACK_SYNC);

    // Switch unpacker config context
    switch_config_context();
}

END_NS_LLK

