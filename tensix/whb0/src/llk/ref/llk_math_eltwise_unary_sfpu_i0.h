#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_i0.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_i0_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::i0_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_i0_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_i0<APPROXIMATE>,
        ckernel::sfpu::calculate_i0<APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

END_NS_LLK

