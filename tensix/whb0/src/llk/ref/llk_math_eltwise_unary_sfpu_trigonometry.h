#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu_trigonometry.h"

#include "llk_defs.h"
#include "llk_math_eltwise_unary_sfpu_init.h"
#include "llk_math_eltwise_unary_sfpu_0_param.h"

BEGIN_NS_LLK

using namespace ckernel;

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_sine_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::sine_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_sine_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::sine, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::sine, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_cosine_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::cosine_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_cosine_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::cosine, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::cosine, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_tan_init() {
    llk_math_eltwise_unary_sfpu_init<APPROXIMATE>(sfpu::tan_init<APPROXIMATE>);
}

template <bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_tan_op(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu_0_param<APPROXIMATE, Dst>(
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::tan, APPROXIMATE>,
        ckernel::sfpu::calculate_sfpu_trig<SfpuType::tan, APPROXIMATE>,
        dst_index, 
        Dim::RC);
}

END_NS_LLK

