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

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_elu_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::elu_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_elu(uint32_t dst_index, uint32_t param0) {
    llk_math_eltwise_unary_sfpu_1_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_elu<APPROXIMATE>,
        ckernel::sfpu::calculate_elu<APPROXIMATE>, 
        dst_index, 
        Dim::RC, 
        param0);
}

END_NS_LLK

