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
#include "ckernel_sfpu_i0.h"

namespace ckernel {

// New LLK SFPU APIs

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_i0_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::i0_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_i0_op(uint dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>
                                (ckernel::sfpu::calculate_i0<APPROXIMATE>,
                                ckernel::sfpu::calculate_i0<APPROXIMATE>,
                                dst_index, Dim::RC);
}

}
