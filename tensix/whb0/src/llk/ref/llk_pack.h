// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "cfg_defines.h"

#include "ckernel_core.h"
#include "ckernel_addrmod.h"
#include "ckernel_instr_params.h"
#include "ckernel_defs.h"
#include "ckernel_mem_ptr.h"
#include "ckernel.h"
#include "cpack_common.h"

#include "llk_defs.h"
#include "llk_param_structs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::packer;

template <
    bool untilize = false, 
    bool zero_output = false, 
    DstTileFaceLayout FaceLayout = DstTileFaceLayout::RowMajor>
inline void llk_pack_mop_config(uint32_t output_id) {
    uint32_t num_faces = get_num_faces(output_id);
    uint32_t face_r_dim = get_face_r_dim(output_id);
    bool partial_face = 
        (get_partial_face(output_id) && IS_BFP_FORMAT((uint32_t)get_pack_dst_format(output_id)));

    addr_mod_pack_t {
        // 4-bit value so max is 15. INCADCXY will increment it by 1
        .y_src = {.incr = untilize ? 0 : 15}, 
        .y_dst = {.incr = 1},
    }
    .set(ADDR_MOD_0);

    if constexpr (untilize) {
       addr_mod_pack_t {
           .y_src = {.incr = 1, .clr = 0, .cr = 1},
           .y_dst = {.incr = 1, .clr = 0, .cr = 0},
       }
       .set(ADDR_MOD_1);
    } else {
       addr_mod_pack_t {
           .y_src = {.incr = 0, .clr = 1, .cr = 0},
           .y_dst = {.incr = 0, .clr = 1, .cr = 0},
           .z_src = {.incr = 0, .clr = 0},
           .z_dst = {.incr = 0, .clr = 0},
       }
       .set(ADDR_MOD_1);
    }

    addr_mod_pack_t {
        .y_src = {.incr = 0, .clr = 0, .cr = 0},
        .y_dst = {.incr = 0, .clr = 0, .cr = 0},
    }
    .set(ADDR_MOD_2);

    // ACHTUNG: Does TT-Metal use headers ???

    uint32_t PACKCNT = partial_face ? 1 : num_faces;
    uint32_t MEGAROW = 1;
    constexpr uint32_t ZERO_OUTPUT_FLAG = 
        zero_output ? p_pacr::P_ZERO_OUTPUT_ENABLED : p_pacr::P_ZERO_OUTPUT_DISABLED;

    // Write header to l1
    if constexpr (!untilize) {
        uint32_t MOP_INNER_LOOP = 1;
        uint32_t MOP_OUTER_LOOP = 1;

        ckernel::ckernel_template tmp(
            MOP_OUTER_LOOP, 
            MOP_INNER_LOOP, 
            TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1));

        if (partial_face) {
            // Don't close the tile, point to the next face
            tmp.set_start_op(
                TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
            // Inc ch0_y += 1 (addr_mod_0 will increment by 15)
            tmp.set_loop_op0(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 1, 0)); 
            // Close the tile
            tmp.set_loop_op1(
                TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1)); 
        }

        tmp.program(instrn_buffer);
    } else {
        bool narrow_tile = get_narrow_tile(output_id);
        uint32_t MOP_UNTILIZE_INNER_LOOP = 
            narrow_tile ? 1 : (FaceLayout == DstTileFaceLayout::ColMajor ? 8 : 4);
        uint32_t MOP_UNTILIZE_OUTER_LOOP = 
            (face_r_dim == 1 || narrow_tile) ? 1 : face_r_dim / 2;

        ckernel::ckernel_template tmp(
            MOP_UNTILIZE_OUTER_LOOP, 
            MOP_UNTILIZE_INNER_LOOP, 
            TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
        if (narrow_tile) {
            // Close the tile and clear the counters
            tmp.set_last_inner_loop_instr(
                TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1)); 
        } else {
            tmp.set_start_op(
                TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
            if (face_r_dim > 1) {
                // If it's narrow tile (32x16) pack rows back to back otherwise jump between faces
                tmp.set_loop_op0(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0)); 
                tmp.set_end_op(
                    TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
            }
            tmp.set_last_inner_loop_instr(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
            tmp.set_last_outer_loop_instr(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
        }
        tmp.program(instrn_buffer);
    }
}

template <bool untilize = false, bool is_fp32_dest_acc_en = false>
inline void llk_pack_hw_configure(const llk_pack_params_t *pack_params) {
    configure_pack<is_fp32_dest_acc_en, untilize>(
        get_output_id(pack_params->pack_output), pack_params->relu_config.val);
}

template <
    bool untilize = false, 
    bool is_fp32_dest_acc_en = false, 
    ReluType relu_type = ReluType::NO_RELU, 
    uint32_t relu_threshold = 0>
inline void llk_pack_hw_configure_disaggregated(uint32_t pack_output) {
    llk_pack_params_t llk_pack_params = {
        .pack_output = pack_output, 
        .relu_config = {
            .f = {
                .ApplyRelu = (uint32_t)relu_type, 
                .Threshold = relu_threshold
            }
        }
    };
    llk_pack_hw_configure<untilize, is_fp32_dest_acc_en>(&llk_pack_params);
}

// FIXME: Remove once edge mask spec is defined
template <bool untilize = false, PoolType type, ReduceDim dim, bool is_fp32_dest_acc_en = false>
inline void llk_pack_reduce_hw_configure(const llk_pack_params_t *pack_params) {
    configure_pack<is_fp32_dest_acc_en, untilize>(
        get_output_id(pack_params->pack_output), pack_params->relu_config.val);

    MEM_PTR_T cfg = get_cfg_pointer();

    pck_edge_offset_u pack_edge_offset;
    pack_edge_offset.val = 0;
    pack_edge_offset.f.mask = 0x0;

    if constexpr (dim == ReduceDim::REDUCE_ROW) {
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = 0x0001;
        if constexpr (untilize) {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack1 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            pack_edge_offset.f.tile_row_set_select_pack3 = 1;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x11111111;
        } else {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x55555555;
        }
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
    } else if constexpr (dim == ReduceDim::REDUCE_SCALAR) {
        pack_edge_offset.f.tile_row_set_select_pack0 = 1;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 1] = 0x0001;
        cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
    } else {
        pack_edge_offset.f.tile_row_set_select_pack0 = 1;
        pack_edge_offset.f.tile_row_set_select_pack1 = 1;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 1] = 0xffff;
        if constexpr (untilize) {
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000005;
        } else {
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
        }
    }
}

template <
    bool untilize = false, 
    PoolType type, 
    ReduceDim dim, 
    bool is_fp32_dest_acc_en = false, 
    ReluType relu_type = ReluType::NO_RELU, 
    uint32_t relu_threshold = 0>
inline void llk_pack_reduce_hw_configure_disaggregated(uint32_t pack_output) {
    llk_pack_params_t llk_pack_params = {
        .pack_output = pack_output, 
        .relu_config = {
            .f = {
                .ApplyRelu = (uint32_t)relu_type, 
                .Threshold = relu_threshold
            }
        }
    };
    llk_pack_reduce_hw_configure<untilize, type, dim, is_fp32_dest_acc_en>(&llk_pack_params);
}

template <
    bool untilize = false, 
    bool zero_output = false, 
    DstTileFaceLayout FaceLayout = DstTileFaceLayout::RowMajor>
inline void llk_pack_init(uint32_t pack_output = 0) {
    uint32_t output_id = get_output_id(pack_output);
    llk_pack_mop_config<untilize, zero_output, FaceLayout>(output_id);
}

template <bool out_of_order_output, bool untilize>
inline uint32_t get_output_tile_address(uint32_t output_id, uint32_t output_tile_index) {
    uint32_t pack_tile_addr;
    CBInterface &cb = get_cb_interface(output_id);
    if constexpr (out_of_order_output) {
        pack_tile_addr = cb.fifo_wr_ptr + cb.fifo_page_size * output_tile_index - 1;
    } else {
        if constexpr (untilize) {
            // FIXME: Do we need support for pack untilize?
        } else {
            pack_tile_addr = cb.fifo_wr_ptr + cb.fifo_wr_tile_ptr - 1;
            cb.fifo_wr_tile_ptr += cb.fifo_page_size;
        }
    }
    return pack_tile_addr;
}

template <
    bool out_of_order_output = false, 
    DstSync Dst = SyncFull, 
    bool untilize = false, 
    bool is_fp32_dest_acc_en = false>
inline void llk_pack(uint32_t tile_index, uint32_t output, uint32_t output_tile_index = 0) {
    uint32_t output_id = get_output_id(output);

    static_assert(!(untilize && out_of_order_output) && 
        "untilize out of order packing is not supported!");

    uint32_t pack_tile_addr = 
        get_output_tile_address<out_of_order_output, untilize>(output_id, output_tile_index);

    constexpr uint32_t DEST_NUM_TILES_SHIFT = is_fp32_dest_acc_en ? 1 : 0;
    constexpr uint32_t DEST_NUM_TILES = DEST_NUM_TILES_FP16 >> DEST_NUM_TILES_SHIFT;

    if constexpr (Dst == DstSync::SyncTile16) {
        // W-counter points to the next tile in dest
        TT_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, pack_sync_tile_dst_ptr);
        pack_sync_tile_dst_ptr++;
        pack_sync_tile_dst_ptr = pack_sync_tile_dst_ptr & (DEST_NUM_TILES - 1);
    } else if constexpr (Dst == DstSync::SyncTile2) {
        TT_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, pack_sync_tile_dst_ptr);
        pack_sync_tile_dst_ptr = 0;
    } else {
        TT_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, tile_index);
    }

    program_packer_destination(pack_tile_addr, output_id);

    mop_run(1, 1);

    if constexpr (untilize) {
        // close tile
        TTI_PACR(ADDR_MOD_2, 0, 0xf, 0, 0, 1, 1); 
        TTI_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_Y, 0);
        TTI_INCADCZW(p_setadc::PAC, 0, 0, 1, 0);
    }
}

template <
    ReduceDim dim, 
    bool at_kernel_start = false, 
    bool revert = false, 
    bool is_fp32_dest_acc_en = false>
inline void llk_pack_reduce_config_v2(uint32_t icb_out) {
    // ACHTUNG: This is provisional version 
    //     It is different from original code
    if constexpr (at_kernel_start)
        configure_pack<is_fp32_dest_acc_en, false>(get_output_id(icb_out), false);
    else {
        TTI_STALLWAIT(p_stall::STALL_PACK, p_stall::PACK);
        tensix_sync();
    }

    MEM_PTR_T cfg = get_cfg_pointer();
    if (revert) {
        if (!at_kernel_start) {
            // same as in 'configure_pack'
            pck_edge_offset_u pck_edge_offset;
            pck_edge_offset.val = 0;
            pck_edge_offset.f.mask = 0xffff;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32] = pck_edge_offset.val;
            cfg[TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32] = 0x0; 
        }
    } else {
        // same as in 'pack_reduce_hw_configure': unify?
        pck_edge_offset_u pack_edge_offset;
        pack_edge_offset.val = 0;
        pack_edge_offset.f.mask = 0x0;
        if constexpr (dim == ReduceDim::REDUCE_ROW) {
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = 0x0001;
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x55555555;
        } else if constexpr (dim == ReduceDim::REDUCE_SCALAR) {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 1] = 0x0001;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
        } else {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack1 = 1;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 0] = pack_edge_offset.val;
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + 1] = 0xffff;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
        }
    }
}

END_NS_LLK

