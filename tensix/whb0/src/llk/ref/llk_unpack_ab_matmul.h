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

inline void llk_unpack_AB_matmul_mop_config(
        bool transpose, 
        uint32_t ct_dim, 
        uint32_t rt_dim, 
        uint32_t kt_dim, 
        bool partial_face) {
    // transpose is unused, math is adjusted to take into account
    // srca face layout when transpose = true

    // in0 - loaded to SrcB
    // in1 - loaded to SrcA

    bool reuse_a = ct_dim >= rt_dim;
    uint32_t replay_buf_prog_len = (!reuse_a && partial_face) ? 14 : 10;
    uint32_t replay_buf_run_len  = replay_buf_prog_len / 2;

    constexpr uint32_t RAREFYB_DISABLE = p_unpacr::RAREFYB_DISABLE;

    if (reuse_a) {
        constexpr uint32_t flop_index_cntx0 = 
            THCON_SEC0_REG3_Base_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        constexpr uint32_t flop_index_cntx1 = 
            THCON_SEC0_REG3_Base_cntx1_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        TT_REPLAY(0, replay_buf_prog_len, 0, 1);
        // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
        TTI_UNPACR(SrcA, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
        TTI_RDCFG(p_gpr_unpack::TMP0, THCON_SEC0_REG3_Base_address_ADDR32);
        TTI_ADDDMAREG(0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP0, p_gpr_unpack::TILE_SIZE_A);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index_cntx0, p_gpr_unpack::TMP0);
        TTI_NOP;
        // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
        TTI_UNPACR(SrcA, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
        TTI_RDCFG(p_gpr_unpack::TMP0, THCON_SEC0_REG3_Base_cntx1_address_ADDR32);
        TTI_ADDDMAREG(0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP0, p_gpr_unpack::TILE_SIZE_A);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index_cntx1, p_gpr_unpack::TMP0);
        TTI_NOP;
    } else {
        constexpr uint32_t flop_index_cntx0 = 
            THCON_SEC1_REG3_Base_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        constexpr uint32_t flop_index_cntx1 = 
            THCON_SEC1_REG3_Base_cntx1_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
        TT_REPLAY(0, replay_buf_prog_len, 0, 1);
        if (partial_face) {
            // ovrd_thread_id = 1, set_dat_valid = 0, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            // Set ch0_z=0, ch1_z=0
            TTI_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 0, 0b0101); 
        } else {
            // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
        }
        TTI_RDCFG(p_gpr_unpack::TMP0, THCON_SEC1_REG3_Base_address_ADDR32);
        TTI_ADDDMAREG(0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP_LO);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index_cntx0, p_gpr_unpack::TMP0);
        TTI_NOP;
        if (partial_face) {
            // ovrd_thread_id = 1, set_dat_valid = 0, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            // Set ch0_z=0, ch1_z=0
            TTI_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 0, 0b0101);
        } else {
            // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
            TTI_UNPACR(SrcB, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
        }
        TTI_RDCFG(p_gpr_unpack::TMP0, THCON_SEC1_REG3_Base_cntx1_address_ADDR32);
        TTI_ADDDMAREG(0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP0, p_gpr_unpack::TMP_LO);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index_cntx1, p_gpr_unpack::TMP0);
        TTI_NOP;
    }

    ckernel_unpack_template tmp = ckernel_unpack_template(
        false,
        false,
        // runs when context is 0
        TT_OP_REPLAY(0, replay_buf_run_len, 0, 0),
        NULL_OP,
        NULL_OP,
        NULL_OP,
        // runs when context is 1
        TT_OP_REPLAY(replay_buf_run_len, replay_buf_run_len, 0, 0), 
        NULL_OP,
        NULL_OP);

    tmp.program(instrn_buffer);
}

template<bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_AB_matmul_hw_configure(
        const llk_unpack_AB_matmul_params_t *unpack_AB_params) {
    constexpr bool is_row_pool = false;
    bool transpose_xy_srca = unpack_AB_params->transpose_xy_srca;

    // In0 -> unpB
    // In1 -> unpA
    uint32_t unpA_operand_id = get_operand_id(unpack_AB_params->unpB_operand);
    uint32_t unpB_operand_id = get_operand_id(unpack_AB_params->unpA_operand);

    // unpA -> srcA
    // unpB -> srcB
    uint32_t unpA_num_faces = get_num_faces(unpA_operand_id);
    uint32_t unpB_num_faces = get_num_faces(unpB_operand_id);

    uint32_t unpA_face_r_dim = get_face_r_dim(unpA_operand_id);
    uint32_t unpB_face_r_dim = get_face_r_dim(unpB_operand_id);

    configure_unpack_AB(
        unpA_operand_id, 
        unpB_operand_id,
        unpA_face_r_dim, 
        unpB_face_r_dim, 
        is_row_pool, 
        transpose_xy_srca,
        is_fp32_dest_acc_en, 
        srnd_fpu_en, 
        unpA_num_faces, 
        unpB_num_faces);

    // Configure tile size in datums
    uint32_t unpA_x_end = unpA_num_faces * unpA_face_r_dim * FACE_C_DIM - 1;
    uint32_t unpB_x_end = unpB_num_faces * unpB_face_r_dim * FACE_C_DIM - 1;
    TT_SETADCXX(p_setadc::UNP_A, unpA_x_end, 0x0);
    TT_SETADCXX(p_setadc::UNP_B, unpB_x_end, 0x0);

    uint32_t inputA = get_operand_id(unpack_AB_params->unpB_operand);
    uint32_t inputB = get_operand_id(unpack_AB_params->unpA_operand);
    regfile[p_gpr_unpack::TILE_SIZE_A] = get_cb_interface(inputA).fifo_page_size;
    regfile[p_gpr_unpack::TILE_SIZE_B] = get_cb_interface(inputB).fifo_page_size;
    sync_regfile_write(p_gpr_unpack::TILE_SIZE_B);
}

template<bool is_fp32_dest_acc_en = false, bool srnd_fpu_en = false>
inline void llk_unpack_AB_matmul_hw_configure_disaggregated(
        uint32_t unpA_operand, 
        uint32_t unpB_operand, 
        uint32_t transpose_xy_srca = 0) {
    llk_unpack_AB_matmul_params_t unpack_AB_matmul_params = {
        .unpA_operand = unpA_operand, 
        .unpB_operand = unpB_operand, 
        .transpose_xy_srca = transpose_xy_srca 
    };
    llk_unpack_AB_matmul_hw_configure<is_fp32_dest_acc_en, srnd_fpu_en>(&unpack_AB_matmul_params);
}

inline void llk_unpack_AB_matmul_init(
        uint32_t unpA_operand, 
        uint32_t unpB_operand, 
        uint32_t transpose = 0, 
        uint32_t ct_dim = 1, 
        uint32_t rt_dim = 1, 
        uint32_t kt_dim = 1) {
    // In0 -> srcB (supports partial face)
    // In1 -> srcA
    uint32_t unpA_operand_id = get_operand_id(unpB_operand);
    uint32_t unpB_operand_id = get_operand_id(unpA_operand);

    uint32_t unpA_face_r_dim = get_face_r_dim(unpA_operand_id);
    uint32_t unpB_face_r_dim = get_face_r_dim(unpB_operand_id);

    bool reuse_a = ct_dim >= rt_dim;
    bool partial_face = get_partial_face(unpB_operand_id);

    uint32_t unpA_num_faces = get_num_faces(unpA_operand_id);
    // if partial face -> unpack face by face
    uint32_t unpB_num_faces = partial_face ? 1 : get_num_faces(unpB_operand_id); 

    // also turn on within_face_16x16_transpose if it was turned off by datacopy at runtime
    // on WH, the unpacker performs both transpose of faces as well as transpose each face.
    // the former is configured in mop, the latter is configured in cfg register in hw_configure
    // in large matmul, datacopy will disable the transpose of faces,
    // so we need it turn it back on for matmul.
    cfg_reg_rmw_tensix<THCON_SEC0_REG2_Haloize_mode_RMW>(transpose);

    TTI_SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    // Loading partial face into srcA is not supported
    uint32_t unpA_x_end = unpA_num_faces * unpA_face_r_dim * FACE_C_DIM - 1;
    TT_SETADCXX(p_setadc::UNP_A, unpA_x_end, 0x0);

    if (partial_face) {
        // Do face by face unpacking. Need to program correct face dim
        // to compute address of the next face
        config_face_dim<false, p_setadc::UNP_B>(unpB_face_r_dim);
    } else {
        // Do full tile unpacking. No need to program face dim
        // as address counter pointing to the face is not incremented
        uint32_t unpB_x_end = unpB_num_faces * unpB_face_r_dim * FACE_C_DIM - 1;
        TT_SETADCXX(p_setadc::UNP_B, unpB_x_end, 0x0);
    }

    // store kt_dim to gpr for scaling tile size
    TT_SETDMAREG(0, LOWER_HALFWORD(kt_dim), 0, LO_16(p_gpr_unpack::KT_DIM)); 

    llk_unpack_AB_matmul_mop_config((transpose != 0), ct_dim, rt_dim, kt_dim, partial_face);
}

inline void llk_unpack_AB_matmul(
        uint32_t operandA, 
        uint32_t operandB, 
        uint32_t tile_index_a, 
        uint32_t tile_index_b, 
        uint32_t ct_dim = 1, 
        uint32_t rt_dim = 1, 
        uint32_t kt_dim = 1) {
    // In0/InA -> srcB (supports partial face)
    // In1/InB -> srcA

    uint32_t inputA = get_operand_id(operandA);
    uint32_t inputB = get_operand_id(operandB);
    CBInterface &cb_a = get_cb_interface(inputA);
    CBInterface &cb_b = get_cb_interface(inputB);
    uint32_t base_address_a = cb_a.fifo_rd_ptr;
    uint32_t base_address_b = cb_b.fifo_rd_ptr;

    // get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    // In1/InB -> srcA
    // In0/InA -> srcB
    uint32_t unpA_face_r_dim = get_face_r_dim(inputB); 
    uint32_t unpB_face_r_dim = get_face_r_dim(inputA);

    bool reuse_a = (ct_dim >= rt_dim);
    uint32_t t_dim = reuse_a ? rt_dim : ct_dim;

    bool partial_face = get_partial_face(inputA);

    if (!reuse_a) {
        TTI_MULDMAREG(0, p_gpr_unpack::TMP_LO, p_gpr_unpack::TILE_SIZE_B, p_gpr_unpack::KT_DIM);
    }

    for (uint32_t t = 0; t < t_dim; t++) {
        uint32_t cur_tile_index_a = tile_index_a + (reuse_a ? (t * kt_dim) : 0);
        uint32_t cur_tile_index_b = tile_index_b + (reuse_a ? 0 : t);
        uint32_t next_tile_index_a = tile_index_a + (reuse_a ? ((t + 1) * kt_dim) : 0);
        uint32_t next_tile_index_b = tile_index_b + (reuse_a ? 0 : (t + 1));
        uint32_t offset_address_a = cb_a.fifo_page_size * cur_tile_index_a;
        uint32_t offset_address_b = cb_b.fifo_page_size * cur_tile_index_b;
        uint32_t next_offset_address_a = cb_a.fifo_page_size * next_tile_index_a;
        uint32_t next_offset_address_b = cb_b.fifo_page_size * next_tile_index_b;
        // note: unpacker is programmed to automatically skip the tile header (+1)
        // since there is no tile header, we need to -1 the address (in terms of 16B words),
        // to offet unpacker's automatic +1
        uint32_t address_a = base_address_a + offset_address_a - 1;
        uint32_t address_b = base_address_b + offset_address_b - 1;
        uint32_t next_address_a = base_address_a + next_offset_address_a - 1;
        uint32_t next_address_b = base_address_b + next_offset_address_b - 1;

        // Wait for free context
        wait_for_next_context(2);

        // Trisc::SEMPOST for context acquire
        semaphore_post(semaphore::UNPACK_SYNC);

        // Program srcA and srcB base addresses
        // note inverted addess_a and address_b
        if (unp_cfg_context == 0) {
            cfg[THCON_SEC0_REG3_Base_address_ADDR32] = address_b;
            cfg[THCON_SEC1_REG3_Base_address_ADDR32] = address_a;
        } else {
            cfg[THCON_SEC0_REG3_Base_cntx1_address_ADDR32] = address_b;
            cfg[THCON_SEC1_REG3_Base_cntx1_address_ADDR32] = address_a;
        }

        constexpr uint32_t RAREFYB_DISABLE = p_unpacr::RAREFYB_DISABLE;

        if (reuse_a) {
            if (partial_face) {
                // Do face by face unpacking
                // ovrd_thread_id = 1, set_dat_valid = 0, auto_inc_context_id = 0
                TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
                TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                TTI_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 0, 0b0101); // Set ch0_z=0, ch1_z=0
            } else {
                // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
                TTI_UNPACR(SrcB, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            }
            if (t + 1 < t_dim) {
                // Let's load one more tile into srcB
                TT_SETDMAREG(0, LOWER_HALFWORD(next_address_a), 0, LO_16(p_gpr_unpack::TMP0));
                TT_SETDMAREG(0, UPPER_HALFWORD(next_address_a), 0, HI_16(p_gpr_unpack::TMP0));
                uint32_t flop_index = 
                    (unp_cfg_context == 0) ?
                        THCON_SEC1_REG3_Base_address_ADDR32 - THCON_CFGREG_BASE_ADDR32 :
                        THCON_SEC1_REG3_Base_cntx1_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
                TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::TMP0);
                TTI_DMANOP;
                if (partial_face) {
                    // Do face by face unpacking
                    // ovrd_thread_id = 1, set_dat_valid = 0, auto_inc_context_id = 0
                    TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                    // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
                    TTI_UNPACR(SrcB, 0b00010001, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                    // Set ch0_z=0, ch1_z=0
                    TTI_SETADCZW(p_setadc::UNP_B, 0, 0, 0, 0, 0b0101);
                } else {
                    // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
                    TTI_UNPACR(SrcB, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                }
                t++;
            }
        } else {
            // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
            TTI_UNPACR(SrcA, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
            if (t + 1 < t_dim) {
                // Let's load one more tile into srcB
                TT_SETDMAREG(0, LOWER_HALFWORD(next_address_b), 0, LO_16(p_gpr_unpack::TMP0));
                TT_SETDMAREG(0, UPPER_HALFWORD(next_address_b), 0, HI_16(p_gpr_unpack::TMP0));
                uint32_t flop_index = 
                    (unp_cfg_context == 0) ?
                        THCON_SEC0_REG3_Base_address_ADDR32 - THCON_CFGREG_BASE_ADDR32 :
                        THCON_SEC0_REG3_Base_cntx1_address_ADDR32 - THCON_CFGREG_BASE_ADDR32;
                TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::TMP0);
                TTI_DMANOP;
                // ovrd_thread_id = 1, set_dat_valid = 1, auto_inc_context_id = 0
                TTI_UNPACR(SrcA, 0, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1);
                t++;
            }
        }

        TT_MOP(0, (reuse_a ? ct_dim : rt_dim) - 1, (unp_cfg_context == 0) ? 0 : 0xff);

        // T6::SEMGET for context release
        t6_semaphore_get(semaphore::UNPACK_SYNC);

        // Switch unpacker config context
        switch_config_context();
    }
}

END_NS_LLK

