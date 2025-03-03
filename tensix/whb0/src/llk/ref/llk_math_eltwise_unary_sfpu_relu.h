// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_relu.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_1_param.h"

BEGIN_NS_LLK

using namespace ckernel;

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
inline void llk_math_eltwise_unary_sfpu_lrelu(uint32_t dst_index, uint32_t param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_lrelu<APPROXIMATE>,
        ckernel::sfpu::calculate_lrelu<APPROXIMATE>,
        dst_index, 
        Dim::RC, 
        param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu_max(uint32_t dst_index, uint32_t param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::relu_max<APPROXIMATE>,
        ckernel::sfpu::relu_max<APPROXIMATE>,
        dst_index, 
        Dim::RC, 
        param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu_min(uint32_t dst_index, uint32_t param0 = 0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::relu_min<APPROXIMATE>,
        ckernel::sfpu::relu_min<APPROXIMATE>,
        dst_index, 
        Dim::RC, 
        param0);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_relu(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::relu_min<APPROXIMATE>,
        ckernel::sfpu::relu_min<APPROXIMATE>,
        dst_index, 
        Dim::RC, 
        0);
}

END_NS_LLK

