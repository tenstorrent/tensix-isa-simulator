/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_math_eltwise_unary_sfpu_common_includes.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_1_param.h"
#include "ckernel_sfpu_relu.h"

namespace ckernel {

// New LLK SFPU APIs


template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_relu_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::relu_min_init<APPROXIMATE>);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_lrelu_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::lrelu_init<APPROXIMATE>);
}
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_relu_max_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::relu_max_init<APPROXIMATE>);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_relu_min_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::relu_min_init<APPROXIMATE>);
}


template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_lrelu(uint dst_index, uint param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_lrelu<APPROXIMATE>,
                                ckernel::sfpu::calculate_lrelu<APPROXIMATE>,
                                dst_index, Dim::RC, param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu_max(uint dst_index, uint param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::relu_max<APPROXIMATE>,
                                ckernel::sfpu::relu_max<APPROXIMATE>,
                                dst_index, Dim::RC, param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu_min(uint dst_index, uint param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::relu_min<APPROXIMATE>,
                                ckernel::sfpu::relu_min<APPROXIMATE>,
                                dst_index, Dim::RC, param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu(uint dst_index) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::relu_min<APPROXIMATE>,
                                ckernel::sfpu::relu_min<APPROXIMATE>,
                                dst_index, Dim::RC, 0);
}

}
