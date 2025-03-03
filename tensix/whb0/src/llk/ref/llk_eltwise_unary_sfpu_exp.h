// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_elu.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_1_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_exponential(
        uint32_t dst_index, int vector_mode = Dim::RC, int param0 = 0) {
	constexpr bool zero_negative = true;
    constexpr int first_iterations = 1;
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_exponential<APPROXIMATE, zero_negative, false, first_iterations>,
        ckernel::sfpu::calculate_exponential<APPROXIMATE, zero_negative>,
        dst_index, 
        vector_mode, 
        param0);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_exponential_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::exp_init<APPROXIMATE>);
}

END_NS_LLK

