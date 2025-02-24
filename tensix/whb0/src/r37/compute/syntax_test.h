// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf
#define APPROX false

#if 0 // TODO: Revise this (macros replaced with templates)
#define ELTWISE_OP
#define ELTWISE_OP_CODE 0
#endif

#include "compute/bcast.h"
#include "compute/eltwise_binary.h"
#include "compute/eltwise_unary.h"
#include "compute/sfpu.h"
#include "compute/matmul.h"
#include "compute/reduce.h"
#include "compute/tile_move_copy.h"
#include "compute/transpose_wh.h"
#include "compute/tilize.h"
#include "compute/untilize.h"

BEGIN_NS_COMPUTE

namespace {

void test_bcast() {
    sub_tiles_bcast_cols(0, 1, 0, 0, 0);
    mul_tiles_bcast_cols(0, 1, 0, 0, 0);
    mul_tiles_bcast_rows(0, 1, 0, 0, 0);
    add_tiles_bcast_rows(0, 1, 0, 0, 0);
    add_tiles_bcast_cols(0, 1, 0, 0, 0);
    init_bcast<EltwiseBinaryType::ELWADD, BroadcastType::ROW>(0, 1);
    any_tiles_bcast<EltwiseBinaryType::ELWADD, BroadcastType::ROW>(0, 1, 0, 0, 0);
    add_tiles_bcast<BroadcastType::ROW>(0, 1, 0, 0, 0);
    sub_tiles_bcast<BroadcastType::ROW>(0, 1, 0, 0, 0);
    mul_tiles_bcast<BroadcastType::ROW>(0, 1, 0, 0, 0);
    add_bcast_rows_init_short();
    add_bcast_cols_init_short();
    mul_tiles_bcast_scalar_init_short();
    mul_tiles_bcast_scalar(0, 1, 0, 0, 0);
    mul_bcast_cols_init_short();
    mul_bcast_rows_init_short();
    sub_bcast_cols_init_short();
}

void test_eltwise_binary() {
    binary_op_init_common(0, 1);
    mul_tiles_init_f();
    mul_tiles_init();
    add_tiles_init_nof();
    add_tiles_init();
    sub_tiles_init_nof();
    sub_tiles_init();
    mul_tiles(0, 1, 0, 0, 0);
    add_tiles(0, 1, 0, 0, 0);
    sub_tiles(0, 1, 0, 0, 0);
#if 0 // TODO: Revise this (macros replaced with templates)
    binary_op_specific_init(ELTWISE_OP_CODE);
#endif
    binary_op_specific_init<EltwiseBinaryType::ELWADD>();
}

void test_eltwise_unary() {
    unary_op_init_common(0);
    init_sfpu(0);
    rsqrt_tile_init();
    rsqrt_tile(0);
    sigmoid_tile_init();
    sigmoid_tile(0);
    log_tile_init();
    log_tile(0);
    log_with_base_tile_init();
    log_with_base_tile(0, 1);
    tanh_tile_init();
    tanh_tile(0);
    signbit_tile_init();
    signbit_tile(0);
    abs_tile_init();
    abs_tile(0);
    sign_tile_init();
    sign_tile(0);
    square_tile_init();
    square_tile(0);
    ltz_tile_init();
    ltz_tile(0);
    eqz_tile_init();
    eqz_tile(0);
    lez_tile_init();
    lez_tile(0);
    gtz_tile_init();
    gtz_tile(0);
    nez_tile_init();
    nez_tile(0);
    gez_tile_init();
    gez_tile(0);
    power_tile_init();
    power_tile(0, 1);
    exp2_tile_init();
    exp2_tile(0);
    heaviside_tile_init();
    heaviside_tile(0, 1);
    expm1_tile_init();
    expm1_tile(0);
    asin_tile_init();
    asin_tile(0);
    atan_tile_init();
    atan_tile(0);
    acos_tile_init();
    acos_tile(0);
    elu_tile_init();
    elu_tile(0, 0);
    // recent additions
    erf_tile_init(); 
    erf_tile(0);
    erfc_tile_init(); 
    erfc_tile(0);
    erfinv_tile_init();
    erfinv_tile(0);
    exp_tile_init();
    exp_tile(0);
    gelu_tile_init();
    gelu_tile(0);
    i0_tile_init();
    i0_tile(0);
    isinf_tile_init();
    isinf_tile(0);
    isposinf_tile_init();
    isposinf_tile(0);
    isneginf_tile_init();
    isneginf_tile(0);
    isnan_tile_init();
    isnan_tile(0);
    isfinite_tile_init();
    isfinite_tile(0);
    logical_not_unary_tile_init();
    logical_not_unary_tile(0);
    recip_tile_init();
    recip_tile(0);
    relu_max_tile_init();
    relu_max_tile(0, 0);
    relu_min_tile_init();
    relu_min_tile(0, 0);
    relu_tile_init();
    relu_tile(0);
    leaky_relu_tile_init();
    leaky_relu_tile(0, 0);
    sqrt_tile_init();
    sqrt_tile(0);
    sin_tile_init();
    sin_tile(0);
    cos_tile_init();
    cos_tile(0);
    tan_tile_init();
    tan_tile(0);
}

void test_matmul() {
    mm_init();
    mm_init_once();
    matmul_tiles(0, 1, 0, 0, 0, false);
#if 0 // TODO: Revise this
    mm_init_short_with_dt(0);
#endif
    mm_init_short();
}

#if 0 // TODO: Revise this
void test_reduce() {
    reduce_init<true>(PoolType::SUM, ReduceDim::REDUCE_ROW, 0, 1);
    reduce_init_short(PoolType::SUM, ReduceDim::REDUCE_ROW, 0, 1);
    reduce_init_delta<true>(PoolType::SUM, ReduceDim::REDUCE_ROW);
    reduce_revert_delta();
    reduce_tile(PoolType::SUM, ReduceDim::REDUCE_ROW, 0, 1, 0, 0, 0);
}
#endif

void test_reduce() {
    reduce_init<PoolType::SUM, ReduceDim::REDUCE_ROW, true>(0, 1);
    reduce_init_short<PoolType::SUM, ReduceDim::REDUCE_ROW>(0, 1);
    reduce_init_delta<PoolType::SUM, ReduceDim::REDUCE_ROW, true>();
    reduce_revert_delta<ReduceDim::REDUCE_ROW>();
    reduce_tile<PoolType::SUM, ReduceDim::REDUCE_ROW>(0, 1, 0, 0, 0);
}

void test_tile_move_copy() {
#if 0 // TODO: Revise this
    copy_tile_to_dst_init_short_with_dt(0);
#endif
    copy_tile_to_dst_init_short();
    copy_tile_init();
    copy_tile(0, 0, 0);
}

void test_transpose_wh() {
    transpose_wh_init(0);
    transpose_wh_tile(0, 0, 0);
}

void test_tilize() {
    tilize_init(0, 0);
    tilize_init_short(0, 0);
    tilize_block(0, 8, 16);
    tilize_uninit();
}

void test_untilize() {
    untilize_init(0);
    untilize_init_short(0);
    untilize_block(0, 8, 16);
    untilize_uninit(0);
}

void test_reg_api() {
    acquire_dst(DstMode::Half);
    release_dst(DstMode::Half);
}

void test_pack() {
    pack_tile(0, 0);
    pack_reconfig_data_format(0);
}

void test_unpack() {
    unpack_reconfig_data_format(0, 1);
}

void test_cb_api() {
    cb_wait_front(0, 1);
    cb_pop_front(0, 1);
    cb_reserve_back(16, 1);
    cb_push_back(16, 1);
}

} // namespace

void MAIN {
    test_bcast();
    test_eltwise_binary();
    test_eltwise_unary();
    test_matmul();
    test_reduce();
    test_tile_move_copy();
    test_transpose_wh();
    test_tilize();
    test_untilize();
    test_reg_api();
    test_pack();
    test_unpack();
    test_cb_api();
}

END_NS_COMPUTE

