// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_erf_erfc.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_erf_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::erf_init<APPROXIMATE>);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_erfc_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::erfc_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_erf(uint32_t dst_index, int param0 = 0) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_erf_erfc<SfpuType::erf, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_erf_erfc<SfpuType::erf, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_erfc(uint32_t dst_index, int param0 = 0) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_erf_erfc<SfpuType::erfc, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_erf_erfc<SfpuType::erfc, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

END_NS_LLK

