#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_erfinv.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_1_param.h"

BEGIN_NS_LLK

using namespace ckernel;

#if 0 // ACHTUNG: Apparent bug in original code: report to TT-Metal core team
template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_logical_not_unary_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::logical_erfinv_init<APPROXIMATE>);
}
#endif

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_erfinv_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::erfinv_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_erfinv_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_erfinv<APPROXIMATE>,
        ckernel::sfpu::calculate_erfinv<APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

END_NS_LLK

