// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_logical_not_noti.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_logical_not_unary_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::logical_not_unary_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_logical_not_unary_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_logical_not_unary<APPROXIMATE>,
        ckernel::sfpu::calculate_logical_not_unary<APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

END_NS_LLK

