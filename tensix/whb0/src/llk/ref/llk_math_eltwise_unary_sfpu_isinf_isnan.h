// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_isinf_isnan.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isinf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isinf_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isinf(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isinf, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isinf, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isposinf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isposinf_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isposinf(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isposinf, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isposinf, APPROXIMATE>,
        dst_index,
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isneginf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isneginf_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isneginf(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isneginf, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isneginf, APPROXIMATE>,
        dst_index,
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isnan_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isnan_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isnan(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isnan, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isnan, APPROXIMATE>,
        dst_index,
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_isfinite_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::isfinite_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_isfinite(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isfinite, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_isinf_isnan<SfpuType::isfinite, APPROXIMATE>,
        dst_index,
        Dim::RC);
}

END_NS_LLK

