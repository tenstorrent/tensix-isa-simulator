// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_math_eltwise_unary_sfpu_common_includes.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"
#include "ckernel_sfpu_isinf_isnan.h"

namespace ckernel {

// New LLK SFPU APIs


//isinf
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isinf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isinf_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isinf(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isinf, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isinf, APPROXIMATE>,
 				                dst_index, Dim::RC);

}

//isposinf
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isposinf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isposinf_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isposinf(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isposinf, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isposinf, APPROXIMATE>,
 				                dst_index,Dim::RC);

}


//isneginf
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isneginf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isneginf_init<APPROXIMATE>);
}


template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isneginf(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isneginf, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isneginf, APPROXIMATE>,
                                dst_index,Dim::RC);

}

//isnan
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isnan_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isnan_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isnan(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isnan, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isnan, APPROXIMATE>,
 				                dst_index,Dim::RC);

}

//isfinite
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isfinite_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isfinite_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isfinite(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isfinite, APPROXIMATE>,
                                ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isfinite, APPROXIMATE>,
 				                dst_index,Dim::RC);

}

}
