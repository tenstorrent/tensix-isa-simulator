// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "ckernel.h"
#include "ckernel_defs.h"
#include "debug/fw_debug.h"
#include "cpack_common.h"
#include "llk_defs.h"
#include "llk_param_structs.h"

#include "hostdevcommon/common_runtime_address_map.h"


using namespace ckernel;
using namespace ckernel::packer;


// wait until math is done and has produced something to pack
inline void llk_packer_wait_for_math_done() {
    TTI_SEMWAIT(p_stall::STALL_TDMA, semaphore::t6_sem(semaphore::MATH_PACK), p_stall::STALL_ON_ZERO);
}

// Tell math that it can write again
template <uint WaitRes = p_stall::NONE>
inline void llk_packer_set_math_semaphore() {
    t6_semaphore_get<WaitRes>(semaphore::MATH_PACK);  // Indicate that packer is done and header is written into L1
}

// Wait for all writes to complete in L1 (header + data)
// Tell math it can write again
// Clear dest
template <DstSync Dst, bool is_fp32_dest_acc_en = false>
inline void llk_pack_dest_section_done() {
    constexpr bool clear_dest = (Dst != DstSync::SyncTile16);

    if constexpr (clear_dest){
        TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::PACK);  // wait for pack to finish

        if constexpr (Dst == DstSync::SyncFull) {
            constexpr uint32_t CLEAR_MODE = is_fp32_dest_acc_en ? p_zeroacc::CLR_ALL_32B : p_zeroacc::CLR_ALL;
            TT_ZEROACC(CLEAR_MODE, ADDR_MOD_1, 0);
        } else {
            static_assert((Dst == DstSync::SyncHalf) || (Dst == DstSync::SyncTile2));
            constexpr uint32_t CLEAR_MODE = is_fp32_dest_acc_en ? p_zeroacc::CLR_HALF_32B : p_zeroacc::CLR_HALF;
            TT_ZEROACC(CLEAR_MODE, ADDR_MOD_1, (dest_offset_id) % 2);
        }
    }

    //Note: we should have already stalled math in non-tile dest modes due to clearing
    constexpr uint32_t WaitRes = (Dst == DstSync::SyncTile16) ? (p_stall::PACK) : (p_stall::NONE);

    // Tell math that it can write again
    llk_packer_set_math_semaphore<WaitRes>();

    constexpr bool flip_dest = ((Dst == DstSync::SyncHalf) || (Dst == DstSync::SyncTile2));

    if constexpr (flip_dest){
        flip_packer_dest_offset_id();
        select_packer_dest_registers<Dst>();
    }
}

template <DstSync Dst, DstTileFaceLayout FaceLayout = RowMajor, bool untilize = false, bool is_fp32_dest_acc_en = false>
inline void llk_pack_dest_init() {
    tensix_sync();
    reset_dest_offset_id();
    init_packer_dest_offset_registers<FaceLayout, untilize, is_fp32_dest_acc_en>();
    select_packer_dest_registers<Dst>();
    packer_addr_counter_init();
    pack_sync_tile_dst_ptr = 0;
}

template <DstSync Dst, DstTileFaceLayout FaceLayout, bool untilize = false>
inline void llk_init_packer_dest_offset_registers(const std::uint32_t pack_output = 0) {
    TTI_STALLWAIT(p_stall::STALL_TDMA|p_stall::STALL_THCON, p_stall::PACK);  // wait for pack to finish
    if constexpr (untilize) {
       if constexpr (FaceLayout == ColMajor) {
          // Packer0 :  0,32,  1,33 ...  7, 39
          // Packer1 :  8,40,  9,41 ... 15, 47
          // Packer2 : 16,48, 17,49 ... 23, 55
          // Packer3 : 23,56, 24,57 ... 31, 63
          TT_SETDMAREG(0, 0x000 + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 0));
          TT_SETDMAREG(0, 0x000 + 0x08, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 1));
          TT_SETDMAREG(0, 0x000 + 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 2));
          TT_SETDMAREG(0, 0x000 + 0x18, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 3));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 0));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x08, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 1));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 2));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x18, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 3));
       } else {
          // Packer0 :  0,16,  1,17 ...  7, 23
          // Packer1 :  8,24,  9,25 ... 15, 31
          // Packer2 : 32,48, 33,49 ... 39, 55
          // Packer3 : 40,56, 41,57 ... 47, 63
          TT_SETDMAREG(0, 0x000 + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 0));
          TT_SETDMAREG(0, 0x000 + 0x08, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 1));
          TT_SETDMAREG(0, 0x000 + 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 2));
          TT_SETDMAREG(0, 0x000 + 0x28, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 3));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 0));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x08, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 1));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 2));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x28, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 3));
       }
    } else {
       if constexpr (FaceLayout == ColMajor) {
           TT_SETDMAREG(0, 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 0));
           TT_SETDMAREG(0, 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 1));
           TT_SETDMAREG(0, 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 2));
           TT_SETDMAREG(0, 0x30, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 3));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 0));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 1));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 2));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x30, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 3));
       } else {  // Default to row major layout
           TT_SETDMAREG(0, 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 0));
           TT_SETDMAREG(0, 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 1));
           TT_SETDMAREG(0, 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 2));
           TT_SETDMAREG(0, 0x30, 0, LO_16(p_gpr_pack::DEST_OFFSET_LO + 3));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 0));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 1));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 2));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x30, 0, LO_16(p_gpr_pack::DEST_OFFSET_HI + 3));
       }
    }
    select_packer_dest_registers<Dst>();
}

inline void llk_pack_debug_dump(std::uint8_t *data, std::uint32_t byte_size) {
    debug_dump(data, byte_size);
}

inline void llk_pack_debug_dump_seek(std::uint8_t offset) {
    debug_dump_seek(offset);
}

template <bool is_fp32_dest_acc_en = false>
inline void llk_pack_reconfig_data_format(const std::uint32_t new_operand) {
    reconfig_packer_data_format<is_fp32_dest_acc_en>(get_output_id(new_operand));
}

template <bool is_fp32_dest_acc_en = false>
inline void llk_pack_reconfig_data_format(const std::uint32_t old_operand, const std::uint32_t new_operand) {
    std::uint32_t old_operand_id = get_output_id(old_operand);
    std::uint32_t new_operand_id = get_output_id(new_operand);

    if((pack_dst_format[old_operand_id] != pack_dst_format[new_operand_id])
       && (pack_dst_format[old_operand_id] != (uint)DataFormat::Invalid)
       && (pack_dst_format[new_operand_id] != (uint)DataFormat::Invalid)) {
        reconfig_packer_data_format<is_fp32_dest_acc_en>(new_operand_id);
    }
}

inline void llk_pack_relu_config(std::uint32_t config) {
    ReluType mode = (config&0xf) == 0 ? ReluType::NO_RELU : ((config&0xf) == 3 ? ReluType::MAX_THRESHOLD_RELU : ReluType::MIN_THRESHOLD_RELU);
    uint32_t val = ((config>>16) << STACC_RELU_ReluThreshold_SHAMT) | (((uint32_t)mode) << STACC_RELU_ApplyRelu_SHAMT);
    TTI_SETDMAREG(0, val&0xffff, 0, LO_16(p_gpr_pack::TMP0));
    TTI_SETDMAREG(0, val>>16, 0, HI_16(p_gpr_pack::TMP0));
	TTI_STALLWAIT(p_stall::STALL_CFG, p_stall::PACK);
    TTI_WRCFG(p_gpr_pack::TMP0,  p_cfg::WRCFG_32b, STACC_RELU_ApplyRelu_ADDR32);
    TTI_NOP; TTI_NOP;
}

inline void llk_pack_reconfig_l1_acc(const std::uint32_t enable)
{
    reconfigure_packer_l1_acc(enable);
}

template <bool untilize = false, ReduceDim dim>
inline void llk_pack_reduce_mask_config() {
    ckernel::packer::pck_edge_offset_u pack_edge_offset = {.val = 0};

    // We initialize PCK_EDGE_OFFSET_SEC0 mask to clear out all the datums in the row
    pack_edge_offset.f.mask = 0x0;
    uint32_t row_set_mapping_1;
    uint32_t edge_offset_sec1_mask;

    if constexpr (dim == ReduceDim::REDUCE_ROW) {
        // PCK_EDGE_OFFSET_SEC1 mask will clear out all the datums in the row except the first one
        edge_offset_sec1_mask = 0x0001;
        if constexpr (untilize) {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack1 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            pack_edge_offset.f.tile_row_set_select_pack3 = 1;
            row_set_mapping_1 = 0x11111111; // each packer packs 1x32 row
        } else {
            // Packer 0 and 2 will use TILE_ROW_SET_MAPPING_1, while packer 1 and 3 will keep using
            // TILE_ROW_SET_MAPPING_0 configuration which is the default one
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;

            // TILE_ROW_SET_MAPPING_1 configuration sets all rows to use PCK_EDGE_OFFSET_SEC1 mask
            row_set_mapping_1 = 0x55555555; // each packer packs 1x16 row
        }
    } else if constexpr (dim == ReduceDim::REDUCE_COL) {
        // PCK_EDGE_OFFSET_SEC1 mask will pass through all the datums in the row as they are
        edge_offset_sec1_mask = 0xffff;

        // Packer 0 and 1 will use TILE_ROW_SET_MAPPING_1, while packer 2 and 3 will keep using
        // TILE_ROW_SET_MAPPING_0 configuration which is the default one
        pack_edge_offset.f.tile_row_set_select_pack0 = 1;
        pack_edge_offset.f.tile_row_set_select_pack1 = 1;

        if constexpr (untilize) {
            row_set_mapping_1 = 0x00000005; // each packer packs 1x32 row
        } else {
            // TILE_ROW_SET_MAPPING_1 configuration sets only first row to use PCK_EDGE_OFFSET_SEC1 mask
            row_set_mapping_1 = 0x00000001; // each packer packs 1x16 row
        }
    }

    // Initialize TMP registers with values we need to write in CFG registers
    TTI_SETDMAREG(0, LOWER_HALFWORD(pack_edge_offset.val), 0, LO_16(p_gpr_pack::TMP0));
    TTI_SETDMAREG(0, UPPER_HALFWORD(pack_edge_offset.val), 0, HI_16(p_gpr_pack::TMP0));
    TTI_SETDMAREG(0, LOWER_HALFWORD(edge_offset_sec1_mask), 0, LO_16(p_gpr_pack::TMP_LO));
    TTI_SETDMAREG(0, LOWER_HALFWORD(row_set_mapping_1), 0, LO_16(p_gpr_pack::TMP1));
    TTI_SETDMAREG(0, UPPER_HALFWORD(row_set_mapping_1), 0, HI_16(p_gpr_pack::TMP1));

    // Wait for packer to finish to avoid breaking its current configuration
    TTI_STALLWAIT(p_stall::STALL_CFG, p_stall::PACK);

    // Configure packer
    TTI_WRCFG(p_gpr_pack::TMP0,  p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32);
    TTI_WRCFG(p_gpr_pack::TMP_LO,  p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC1_mask_ADDR32);
    TTI_WRCFG(p_gpr_pack::TMP1,  p_cfg::WRCFG_32b, TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32);

    TTI_NOP; TTI_NOP;
}

inline void llk_pack_reduce_mask_clear() {
    // By default, all packers are set to use TILE_ROW_SET_MAPPING_0 and
    // mask is configured to pass through all the datums
    pck_edge_offset_u pack_edge_offset = {.val = 0};
    pack_edge_offset.f.mask = 0xffff;

    // Initialize TMP registers with values we need to write in CFG registers
    TTI_SETDMAREG(0, LOWER_HALFWORD(pack_edge_offset.val), 0, LO_16(p_gpr_pack::TMP0));
    TTI_SETDMAREG(0, UPPER_HALFWORD(pack_edge_offset.val), 0, HI_16(p_gpr_pack::TMP0));

    // Wait for packer to finish to avoid breaking its current configuration
    TTI_STALLWAIT(p_stall::STALL_CFG, p_stall::PACK);

    // Clear out packer configuration for reduce
    TTI_WRCFG(p_gpr_pack::TMP0,  p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32);
    TTI_WRCFG(p_gpr_pack::TMP0,  p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC1_mask_ADDR32);

    // All mappings point to PCK_EDGE_OFFSET_SEC0_mask_ADDR32
    TTI_WRCFG(p_gpr::ZERO,  p_cfg::WRCFG_32b, TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32);
    TTI_WRCFG(p_gpr::ZERO,  p_cfg::WRCFG_32b, TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32);

    TTI_NOP; TTI_NOP;
}
