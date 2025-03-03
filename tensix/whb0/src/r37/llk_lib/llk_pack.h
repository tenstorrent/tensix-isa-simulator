// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_io_pack.h"
#include "llk_defs.h"
#include "llk_param_structs.h"

#include "ckernel.h"
#include "ckernel_template.h"
#include "llk_pack_common.h"
#include "ckernel_globals.h"

using namespace ckernel;
using namespace ckernel::packer;

template <bool untilize = false, bool zero_output = false, DstTileFaceLayout FaceLayout = DstTileFaceLayout::RowMajor>
inline void llk_pack_mop_config(const uint32_t output_id) {

    const uint num_faces = get_num_faces(output_id);
    const uint face_r_dim = get_face_r_dim(output_id);
    const bool partial_face = get_partial_face(output_id) && IS_BFP_FORMAT((uint)pack_dst_format[output_id]);

    addr_mod_pack_t{
        .y_src = {.incr = untilize ? 0 : 15}, // 4-bit value so max is 15. incadcxy will increment it by 1
        .y_dst = {.incr = 1},
    }
        .set(ADDR_MOD_0);

    if constexpr (untilize) {
       addr_mod_pack_t{
           .y_src = { .incr = 1, .clr = 0, .cr = 1  },
           .y_dst = { .incr = 1, .clr = 0, .cr = 0  },
       }.set(ADDR_MOD_1);
    } else {
       addr_mod_pack_t{
           .y_src = {.incr = 0, .clr = 1, .cr = 0},
           .y_dst = {.incr = 0, .clr = 1, .cr = 0},
           .z_src = {.incr = 0, .clr = 0},
           .z_dst = {.incr = 0, .clr = 0},
       }
       .set(ADDR_MOD_1);
    }

    addr_mod_pack_t{
        .y_src = { .incr = 0, .clr = 0, .cr = 0  },
        .y_dst = { .incr = 0, .clr = 0, .cr = 0  },
    }.set(ADDR_MOD_2);

    const uint PACKCNT = partial_face ? 1 : num_faces;
    const uint MEGAROW = 1;
    constexpr uint ZERO_OUTPUT_FLAG = zero_output ? p_pacr::P_ZERO_OUTPUT_ENABLED : p_pacr::P_ZERO_OUTPUT_DISABLED;


    // Write header to l1
    if constexpr (!untilize) {
        const uint MOP_INNER_LOOP = 1;
        const uint MOP_OUTER_LOOP = 1;

        ckernel::ckernel_template tmp(MOP_OUTER_LOOP, MOP_INNER_LOOP, TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1));

        if (partial_face) {
            tmp.set_start_op(TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0)); // Don't close the tile, point to the next face
            tmp.set_loop_op0(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 1, 0)); // Inc ch0_y+=1 (addr_mod_0 will increment by 15)
            tmp.set_loop_op1(TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1)); // Close the tile
        }


        tmp.program(instrn_buffer);
    } else {
        const bool narrow_tile = get_narrow_tile(output_id);
        const uint MOP_UNTILIZE_INNER_LOOP = narrow_tile ? 1 : (FaceLayout == DstTileFaceLayout::ColMajor ? 8 : 4);
        const uint MOP_UNTILIZE_OUTER_LOOP = ((face_r_dim == 1) || narrow_tile) ? 1 : face_r_dim / 2;

        ckernel::ckernel_template tmp(MOP_UNTILIZE_OUTER_LOOP, MOP_UNTILIZE_INNER_LOOP, TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
        if (narrow_tile) {
            tmp.set_last_inner_loop_instr(TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 1)); // Close the tile and clear the counters
        } else {
            tmp.set_start_op(TT_OP_PACR(ADDR_MOD_0, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
            if (face_r_dim>1) {
                tmp.set_loop_op0(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0)); // If it's narrow tile (32x16) pack rows back to back otherwise jump between faces
                tmp.set_end_op(TT_OP_PACR(ADDR_MOD_1, ZERO_OUTPUT_FLAG, PACK_SEL(PACKCNT), 0, MEGAROW, 0, 0));
            }
            tmp.set_last_inner_loop_instr(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
            tmp.set_last_outer_loop_instr(TT_OP_INCADCXY(p_setadc::PAC, 0, 0, 4, 0));
        }
        tmp.program(instrn_buffer);
    }

}

template <bool untilize = false, bool is_fp32_dest_acc_en = false>
inline void llk_pack_hw_configure(const llk_pack_params_t *pack_params) {
    configure_pack<is_fp32_dest_acc_en, untilize>(get_output_id(pack_params->pack_output), pack_params->relu_config.val);
}

template <bool untilize = false, bool is_fp32_dest_acc_en = false, ReluType relu_type = ReluType::NO_RELU, std::uint32_t relu_threshold = 0>
inline void llk_pack_hw_configure_disaggregated(std::uint32_t pack_output) {
    llk_pack_params_t llk_pack_params = {
        .pack_output = pack_output, .relu_config = {.f = {.ApplyRelu = (std::uint32_t)relu_type, .Threshold = relu_threshold,}}};
    llk_pack_hw_configure<untilize, is_fp32_dest_acc_en>(&llk_pack_params);
}

// FIXME: Remove once edge mask spec is defined
template <bool untilize = false, PoolType type, ReduceDim dim, bool is_fp32_dest_acc_en = false>
inline void llk_pack_reduce_hw_configure(const llk_pack_params_t *pack_params) {
    configure_pack<is_fp32_dest_acc_en, untilize>(get_output_id(pack_params->pack_output), pack_params->relu_config.val);
#if 0 // [-ISS]
    volatile uint tt_reg_ptr *cfg = get_cfg_pointer();
#endif
    MEM_PTR_T cfg = get_cfg_pointer();

    ckernel::packer::pck_edge_offset_u pack_edge_offset = {.val = 0};
    pack_edge_offset.f.mask = 0x0;
    if constexpr (dim == ReduceDim::REDUCE_ROW) {
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = 0x0001;
        if constexpr (untilize) {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack1 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            pack_edge_offset.f.tile_row_set_select_pack3 = 1;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x11111111; // each packer packs 1x32 row
        } else {
            pack_edge_offset.f.tile_row_set_select_pack0 = 1;
            pack_edge_offset.f.tile_row_set_select_pack2 = 1;
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x55555555; // each packer packs 1x16 row
        }
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0] = pack_edge_offset.val;
    } else if constexpr (dim == ReduceDim::REDUCE_SCALAR) {
        pack_edge_offset.f.tile_row_set_select_pack0 = 1;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0] = pack_edge_offset.val;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = 0x0001;
        cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
    } else {
        pack_edge_offset.f.tile_row_set_select_pack0 = 1;
        pack_edge_offset.f.tile_row_set_select_pack1 = 1;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0] = pack_edge_offset.val;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = 0xffff;

        if constexpr (untilize) {
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000005;// Each packer packs 1x32 row
        } else {
            cfg[TILE_ROW_SET_MAPPING_1_row_set_mapping_0_ADDR32] = 0x00000001;
        }
    }
}

template <bool untilize = false, PoolType type, ReduceDim dim, bool is_fp32_dest_acc_en = false, ReluType relu_type = ReluType::NO_RELU, std::uint32_t relu_threshold = 0>
inline void llk_pack_reduce_hw_configure_disaggregated(std::uint32_t pack_output) {
    llk_pack_params_t llk_pack_params = {
        .pack_output = pack_output, .relu_config = {.f = {.ApplyRelu = (std::uint32_t)relu_type, .Threshold = relu_threshold}}};
    llk_pack_reduce_hw_configure<untilize, type, dim, is_fp32_dest_acc_en>(&llk_pack_params);
}

template <bool untilize = false, bool zero_output = false, DstTileFaceLayout FaceLayout = DstTileFaceLayout::RowMajor>
inline void llk_pack_init(const std::uint32_t pack_output = 0) {
    const std::uint32_t output_id = get_output_id(pack_output);
    llk_pack_mop_config<untilize, zero_output, FaceLayout>(output_id);
}

template <bool out_of_order_output, bool untilize>
inline std::uint32_t get_output_tile_address(std::uint8_t output_id, std::uint32_t output_tile_index) {

    std::uint32_t pack_tile_addr;
    if constexpr (out_of_order_output) {
        pack_tile_addr = cb_interface[output_id].fifo_wr_ptr +
                        (std::uint32_t)(cb_interface[output_id].fifo_page_size)*output_tile_index - 1;
    } else {
        if constexpr (untilize) {
            // FIXME: Do we need support for pack untilize?
            // std::uint16_t out_tile_index = (cb_interface[output_id].ublock_tile_cnt/cb_interface[output_id].ublock_ct)*cb_interface[output_id].row_tile_dim +
            //                                 cb_interface[output_id].ublock_tile_cnt%cb_interface[output_id].ublock_ct; //FIXME: optimize perf
            // pack_tile_addr = cb_interface[output_id].fifo_wr_ptr + cb_interface[output_id].fifo_wr_tile_ptr - 1;
            // pack_tile_addr += out_tile_index*(std::uint32_t)(cb_interface[output_id].fifo_page_size);

            // cb_interface[output_id].ublock_tile_cnt++;

            // if (cb_interface[output_id].ublock_tile_cnt == cb_interface[output_id].ublock_tile_dim) {
            //    cb_interface[output_id].ublock_tile_cnt=0;
            //    cb_interface[output_id].fifo_wr_tile_ptr += (std::uint32_t)(cb_interface[output_id].fifo_page_size)*cb_interface[output_id].ublock_ct;
            // }
        } else {
            pack_tile_addr = cb_interface[output_id].fifo_wr_ptr + cb_interface[output_id].fifo_wr_tile_ptr - 1;
            cb_interface[output_id].fifo_wr_tile_ptr += cb_interface[output_id].fifo_page_size;
        }
    }
    return pack_tile_addr;
}

#if defined(PERF_DUMP) && MATH_PACK_DECOUPLE
template <bool out_of_order_output = false, DstSync Dst = SyncFull, bool untilize = false, bool is_fp32_dest_acc_en = false, bool pack_l1_acc_en = false>
inline void llk_pack_decouple(std::uint32_t tile_index, std::uint32_t output, std::uint32_t output_tile_index = 0, bool pack_l1_acc = false) {

    std::uint8_t output_id = get_output_id(output);

    static_assert((!(untilize && out_of_order_output)) && "untilize out of order packing is not supported!");

    std::uint32_t pack_tile_addr = get_output_tile_address<out_of_order_output, untilize>(output_id, output_tile_index);

    if (operand_is_intermediate(output)) {
        return;
    }

    if constexpr (!untilize) {
        uint32_t tile_header[4];
        uint32_t* l1_dest = reinterpret_cast<uint32_t*>(pack_tile_addr << 4);
        for (int i = 0; i < 4; i++) {
            tile_header[i] = regfile[p_gpr_pack::TILE_HEADER + i];
            l1_dest[i] = tile_header[i];
        }
    }
}
#endif

template <bool out_of_order_output = false, DstSync Dst = SyncFull, bool untilize = false, bool is_fp32_dest_acc_en = false>
inline void llk_pack(std::uint32_t tile_index, std::uint32_t output, std::uint32_t output_tile_index = 0) {

    std::uint8_t output_id = get_output_id(output);

    // Access tile dims using the following logic:
    // pack_tile_dims[output_id]

    static_assert((!(untilize && out_of_order_output)) && "untilize out of order packing is not supported!");

    std::uint32_t pack_tile_addr = get_output_tile_address<out_of_order_output, untilize>(output_id, output_tile_index);

    constexpr uint32_t DEST_NUM_TILES_SHIFT = is_fp32_dest_acc_en ? (1) : (0);
    constexpr uint32_t DEST_NUM_TILES = DEST_NUM_TILES_FP16 >> DEST_NUM_TILES_SHIFT;

    if constexpr (Dst == DstSync::SyncTile16) {
        // W-counter points to the next tile in dest
        TT_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_W, pack_sync_tile_dst_ptr);
        pack_sync_tile_dst_ptr += 1;
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
        TTI_PACR(ADDR_MOD_2, 0, 0xf, 0, 0, 1, 1); // close tile
        TTI_SETADC(p_setadc::PAC, p_setadc::CH_0, p_setadc::SET_Y, 0);
        TTI_INCADCZW(p_setadc::PAC, 0, 0, 1, 0);
    }

}
// FIXME-WH-UPLIFT
template <ReduceDim dim, bool at_kernel_start = false, bool revert=false, bool is_fp32_dest_acc_en = false>
inline void llk_pack_reduce_config_v2(uint32_t icb_out) {

    if constexpr (at_kernel_start)
        configure_pack<is_fp32_dest_acc_en, false>(get_output_id(icb_out), false);
    else {
        TTI_STALLWAIT(p_stall::STALL_PACK, p_stall::PACK);
        tensix_sync();
    }

#if 0 // [-ISS]
    volatile uint *cfg = get_cfg_pointer();
#endif
    MEM_PTR_T cfg = get_cfg_pointer();
    if constexpr (dim == ReduceDim::REDUCE_ROW) {
        for (uint i = 0; i < 4; i++)
            //TTI_WRCFG(revert ? 0xFFFFffff : 0x1, p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32+i);
            cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32 + i] = revert ? 0xFFFFffff : 0x1;
    } else if constexpr (dim == ReduceDim::REDUCE_SCALAR) {
        //TTI_WRCFG(revert ? 0xFFFFffff : 0x0, p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0);
        //TTI_WRCFG(revert ? 0xFFFFffff : 0x1, p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1);
        //TTI_WRCFG(revert ? 0xFFFFffff : 0x1, p_cfg::WRCFG_32b, TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32);
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0] = revert ? 0xFFFFffff : 0x0;
        cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = revert ? 0xFFFFffff : 0x1;
        cfg[TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32] = revert ? 0xF : 0x1;
    } else {
        //TTI_WRCFG(revert ? 0xFFFFffff : 0x0,    p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0);
        //TTI_WRCFG(revert ? 0xFFFFffff : 0xFFFF, p_cfg::WRCFG_32b, PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1);
        //cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+0] = revert ? 0xFFFFffff : 0x0;
        //cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32+1] = revert ? 0xFFFFffff : 0x0000ffff;
    }
}
