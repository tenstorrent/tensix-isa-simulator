#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_gelu.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_gelu(
        uint32_t dst_index, int vector_mode = Dim::RC, int param0 = 0) {
    constexpr int first_iterations = 1;
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_gelu<APPROXIMATE, first_iterations>,
        ckernel::sfpu::calculate_gelu<APPROXIMATE>,
        dst_index, 
        vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_gelu_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::gelu_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_gelu_derivative(
        uint32_t dst_index, int vector_mode = Dim::RC) {
    constexpr int first_iterations = 1;
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_gelu_derivative<APPROXIMATE, first_iterations>,
        ckernel::sfpu::calculate_gelu_derivative<APPROXIMATE>,
        dst_index, 
        vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_gelu_derivative_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::gelu_derivative_init<APPROXIMATE>);
}

END_NS_LLK

