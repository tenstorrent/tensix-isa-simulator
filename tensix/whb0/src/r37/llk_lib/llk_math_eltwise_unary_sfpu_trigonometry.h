/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_math_eltwise_unary_sfpu_common_includes.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"
#include "ckernel_sfpu_trigonometry.h"

namespace ckernel {

// New LLK SFPU APIs

//sine
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_sine_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::sine_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_sine_op(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_trig<SfpuType::sine, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_trig<SfpuType::sine, APPROXIMATE>,
                                dst_index, Dim::RC);
}


//cosine
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_cosine_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::cosine_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_cosine_op(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_trig<SfpuType::cosine, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_trig<SfpuType::cosine, APPROXIMATE>,
                                dst_index, Dim::RC);
}


//tangent
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_tan_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::tan_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_tan_op(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_trig<SfpuType::tan, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_trig<SfpuType::tan, APPROXIMATE>,
                                dst_index, Dim::RC);

}

}
