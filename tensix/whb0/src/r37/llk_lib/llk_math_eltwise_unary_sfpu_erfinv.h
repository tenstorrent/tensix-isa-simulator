// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_math_eltwise_unary_sfpu_common_includes.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"
#include "ckernel_sfpu_erfinv.h"

namespace ckernel {

// New LLK SFPU APIs

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_erfinv_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::erfinv_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_erfinv_op(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_erfinv<APPROXIMATE>,
                                ckernel::sfpu::calculate_erfinv<APPROXIMATE>,
                                dst_index, Dim::RC);
}

}
