// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cfg_defines.h"

#include "fw_debug.h"
#include "ckernel_core.h"
#include "ckernel_addrmod.h"
#include "ckernel_instr_params.h"
#include "cmath_common.h"

#include "llk_defs.h"

BEGIN_NS_LLK

using namespace ckernel;

template <
    DataCopyType type, 
    BroadcastType src_b_bcast_type = BroadcastType::NONE, 
    DstSync Dst = DstSync::SyncFull, 
    bool is_fp32_dest_acc_en = false>
inline void llk_math_eltwise_unary_datacopy(uint32_t dst_index) {
    if constexpr ((Dst == DstSync::SyncTile16) || (Dst == DstSync::SyncTile2)) {
        math::set_dst_write_addr<
            DstTileLayout::Default, DstTileShape::Tile32x32>(
                math_sync_tile_dst_index);
    } else {
        math::set_dst_write_addr<DstTileLayout::Default, DstTileShape::Tile32x32>(dst_index);
    }

    if constexpr (type == A2D) {
        ckernel_template::run(instrn_buffer);
    } else if constexpr (type == B2D) {
        if constexpr (src_b_bcast_type == BroadcastType::SCALAR) {
            // Manually clear B once mop is done
            ckernel_template::run(instrn_buffer);
            TTI_SETRWC(p_setrwc::CLR_B, 0, 0, 0, 0, 0);
        } else if constexpr (src_b_bcast_type == BroadcastType::COL) {
            // Mop for col broadcast only does 2 outerloops.
            // Needs to clear B manually and call twice
            ckernel_template::run(instrn_buffer);
            TTI_SETRWC(p_setrwc::CLR_B, 0, 0, 0, 0, 0);
            ckernel_template::run(instrn_buffer);
            TTI_SETRWC(p_setrwc::CLR_AB, 0, 0, 0, 0, 0);
        } else {
            ckernel_template::run(instrn_buffer);
        }
    } else {
        FWASSERT("Unsupported op!", false);
    }

    math::clear_dst_reg_addr();
}

template <DataCopyType type, BroadcastType bcast_type = BroadcastType::NONE>
inline void eltwise_unary_configure_addrmod() {
    // Use srcA for data movement
    if constexpr (type == A2D) {
        addr_mod_t {
            .srca = {.incr = 1},
            .srcb = {.incr = 0},
            .dest = {.incr = 1},
        }
        .set(ADDR_MOD_0);

        // Just unpack into A and move to Dest
        addr_mod_t {
            .srca = {.incr = 8},
            .srcb = {.incr = 0},
            .dest = {.incr = 8},
        }
        .set(ADDR_MOD_2);
    } else {
        if constexpr (bcast_type == BroadcastType::ROW || bcast_type == BroadcastType::SCALAR) {
            addr_mod_t {
                .srca = {.incr = 0},
                .srcb = {.incr = 0},
                .dest = {.incr = 1},
            }
            .set(ADDR_MOD_0);

            // Just unpack into B and move to Dest
            addr_mod_t {
                .srca = {.incr = 0},
                .srcb = {.incr = 0},
                .dest = {.incr = 8},
            }
            .set(ADDR_MOD_2);
        } else {
            addr_mod_t {
                .srca = {.incr = 0},
                .srcb = {.incr = 1},
                .dest = {.incr = 1},
            }
            .set(ADDR_MOD_0);

            // Just unpack into B and move to Dest
            addr_mod_t {
                .srca = {.incr = 0},
                .srcb = {.incr = 8},
                .dest = {.incr = 8},
            }
            .set(ADDR_MOD_2);
        }
    }
}

template <DataCopyType type, BroadcastType bcast_type = BroadcastType::NONE>
inline void eltwise_unary_configure_mop(
        uint32_t rows_per_inst, 
        uint32_t total_rows, 
        uint32_t operand_id) {
    // always move 32x32 tile, packed as 16x16x4

    if constexpr (type == A2D) {
        uint32_t num_faces = get_num_faces(operand_id);
        uint32_t addr_mod = 
            (rows_per_inst == p_mova2d::MOV_1_ROW) ? ADDR_MOD_0 : ADDR_MOD_2;
        uint32_t innerloop = 
            (rows_per_inst == p_mova2d::MOV_1_ROW) ? total_rows : (total_rows >> 3);
        uint32_t outerloop = num_faces;

        // use elwadd to handle unpacking data into src A as fp16, but dest is in fp32 mode
        ckernel_template tmp(
            outerloop, 
            innerloop, 
            TT_OP_ELWADD(0, 0, p_elwise::SRCB_NO_BCAST, ADDR_MOD_2, 0));
        tmp.set_end_op(TT_OP_SETRWC(p_setrwc::CLR_AB, 0, 0, 0, 0, p_setrwc::SET_AB));
        tmp.program(instrn_buffer);
    } else if constexpr (type == B2D) {
        uint32_t addr_mod = 
            (rows_per_inst == p_movb2d::MOV_1_ROW) ? ADDR_MOD_0 : ADDR_MOD_2;
        uint32_t innerloop = 
            (rows_per_inst == p_movb2d::MOV_1_ROW) ? total_rows : (total_rows >> 2);
        uint32_t outerloop = 4;
        auto broadcast_type = p_movb2d::MOV_1_ROW;  // No broadcast;

        if constexpr (bcast_type == BroadcastType::COL) {
            innerloop = 16 >> 3; //elwadd produces 8 rows per op
            // The mop only runs for 2 outer loops and mop is called twice for col broadcast
            outerloop = 2;
            // broadcast_type = p_movb2d::MOV_8_ROW_BRCST_D0_BRCST;
            // MOVB2D with column broadcast doesn't work due to the bug in FPU tile
            // which masks dest write enable signals when instrn_mode[1:0] == 2'b01
            // ELTWADD with zeros will be used as a workaround
            broadcast_type = p_elwise::SRCB_BCAST_COL;
        } else if constexpr (bcast_type == BroadcastType::ROW) {
            innerloop = (total_rows >> 3);
            broadcast_type = p_movb2d::MOV_8_ROW_BRCST;
        } else if constexpr (bcast_type == BroadcastType::SCALAR) {
            innerloop = (total_rows >> 3);
            broadcast_type = p_movb2d::MOV_8_ROW_BRCST_D0_BRCST;
        }

        if constexpr (bcast_type == BroadcastType::SCALAR) {
            ckernel_template tmp(
                outerloop, 
                innerloop, 
                TT_OP_MOVB2D(0, 0, addr_mod, broadcast_type, 0));
            tmp.set_end_op(TT_OP_SETRWC(0, p_setrwc::CR_B, 0, 0, 0, p_setrwc::SET_B));
            tmp.program(instrn_buffer);
        } else if constexpr (bcast_type == BroadcastType::COL) {
            ckernel_template tmp(
                outerloop, 
                innerloop, 
                TT_OP_ELWADD(0, 0, broadcast_type, addr_mod, 0));
            tmp.set_end_op(TT_OP_SETRWC(0, p_setrwc::CR_B, 0, 0, 0, p_setrwc::SET_B));
            tmp.program(instrn_buffer);
        } else if constexpr (bcast_type == BroadcastType::ROW) {
            ckernel_template tmp(
                outerloop, 
                innerloop, 
                TT_OP_MOVB2D(0, 0, addr_mod, broadcast_type, 0));
            tmp.set_end_op(
                TT_OP_SETRWC(p_setrwc::CLR_B, p_setrwc::CR_B, 0, 0, 0, p_setrwc::SET_B));
            tmp.program(instrn_buffer);
        } else {
            ckernel_template tmp(
                outerloop, 
                innerloop, 
                TT_OP_MOVB2D(0, 0, addr_mod, rows_per_inst, 0));
            tmp.set_end_op(
                TT_OP_SETRWC(p_setrwc::CLR_B, p_setrwc::CR_B, 0, 0, 0, p_setrwc::SET_B));
            tmp.program(instrn_buffer);
        }
    }
}

template <DataCopyType type, BroadcastType src_b_bcast_type = BroadcastType::NONE>
inline void llk_math_eltwise_unary_datacopy_init(
        uint32_t transpose_of_faces = 0 /*unused*/, 
        uint32_t within_face_16x16_transpose = 0 /* unused */, 
        uint32_t operand = 0) {
    // within_face_16x16_transpose is used by unpacker, math does not transpose
    uint32_t operand_id = get_operand_id(operand);

    eltwise_unary_configure_addrmod<type, src_b_bcast_type>();

    if constexpr (type == A2D) {
        eltwise_unary_configure_mop<type, src_b_bcast_type>(p_mova2d::MOV_8_ROWS, 16, operand_id);
    } else if constexpr (type == B2D) {
        eltwise_unary_configure_mop<type, src_b_bcast_type>(p_movb2d::MOV_4_ROWS, 16, operand_id);
    } else {
        FWASSERT("Unsupported op!", false);
    }

    TTI_SETC16(CLR_DVALID_SrcA_Disable_ADDR32, 0);

    math::reset_counters(p_setrwc::SET_ABD_F);
}

END_NS_LLK

