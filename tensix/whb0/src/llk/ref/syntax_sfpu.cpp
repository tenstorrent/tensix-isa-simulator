// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include "ckernel_core.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu.h"
#include "ckernel_sfpu_elu.h"
#include "ckernel_sfpu_erf_erfc.h"
#include "ckernel_sfpu_erfinv.h"
#include "ckernel_sfpu_exp.h"
#include "ckernel_sfpu_gelu.h"
#include "ckernel_sfpu_i0.h"
#include "ckernel_sfpu_isinf_isnan.h"
#include "ckernel_sfpu_logical_not_noti.h"
#include "ckernel_sfpu_recip.h"
#include "ckernel_sfpu_sqrt.h"
#include "ckernel_sfpu_trigonometry.h"

BEGIN_NS_LLK

using namespace ckernel;

void syntax_sfpu() {
    sfpu::calculate_sfpu<SfpuType::exp_with_base, false>();
    sfpu::calculate_sfpu<SfpuType::tanh, false>();
    sfpu::calculate_sfpu<SfpuType::hardtanh, false>();
    sfpu::calculate_sfpu<SfpuType::rsqrt, false>();
    sfpu::calculate_sfpu<SfpuType::sigmoid, false>();
    sfpu::calculate_sfpu<SfpuType::sigmoid_appx, false>();
    sfpu::calculate_sfpu<SfpuType::tanh_derivative, false>();
    sfpu::calculate_sfpu<SfpuType::dropout, false>();
    sfpu::calculate_sfpu<SfpuType::power, false>();
    sfpu::calculate_sfpu<SfpuType::square, false>();
    sfpu::calculate_sfpu<SfpuType::log, false>();
    sfpu::calculate_sfpu<SfpuType::log_with_base, false>();
    sfpu::calculate_sfpu<SfpuType::equal_zero, false>();
    sfpu::calculate_sfpu<SfpuType::not_equal_zero, false>();
    sfpu::calculate_sfpu<SfpuType::less_than_zero, false>();
    sfpu::calculate_sfpu<SfpuType::greater_than_equal_zero, false>();
    sfpu::calculate_sfpu<SfpuType::less_than_equal_zero, false>();
    sfpu::calculate_sfpu<SfpuType::greater_than_zero, false>();
    sfpu::calculate_sfpu<SfpuType::clamp, false>();
    sfpu::calculate_sfpu<SfpuType::abs, false>();
    sfpu::calculate_sfpu<SfpuType::sign, false>();
    sfpu::calculate_sfpu<SfpuType::max, false>();
    sfpu::calculate_sfpu<SfpuType::min, false>();
    sfpu::calculate_sfpu<SfpuType::exp2, false>();
    sfpu::calculate_sfpu<SfpuType::heaviside, false>();
    sfpu::calculate_sfpu<SfpuType::expm1, false>();
    sfpu::calculate_sfpu<SfpuType::asin, false>();
    sfpu::calculate_sfpu<SfpuType::acos, false>();
    sfpu::calculate_sfpu<SfpuType::atan, false>();
    // "new API"
    sfpu::elu_init<false>();
    sfpu::calculate_elu<false>(0);
    sfpu::erf_init<false>();
    sfpu::erfc_init<false>();
    sfpu::calculate_sfpu_erf_erfc<SfpuType::erf, false>();
    sfpu::calculate_sfpu_erf_erfc<SfpuType::erfc, false>();
    sfpu::erfinv_init<false>();
    sfpu::calculate_erfinv<false>();
    sfpu::exp_init<false>();
    sfpu::calculate_exponential<false, false>();
    sfpu::gelu_init<false>();
    sfpu::gelu_derivative_init<false>();
    sfpu::calculate_gelu<false>();
    sfpu::calculate_gelu_derivative<false>();
    sfpu::i0_init<false>();
    sfpu::calculate_i0<false>();
    sfpu::isinf_init<false>();
    sfpu::isposinf_init<false>();
    sfpu::isneginf_init<false>();
    sfpu::isnan_init<false>();
    sfpu::isfinite_init<false>();
    sfpu::calculate_sfpu_isinf_isnan<SfpuType::isinf, false>();
    sfpu::calculate_sfpu_isinf_isnan<SfpuType::isposinf, false>();
    sfpu::calculate_sfpu_isinf_isnan<SfpuType::isneginf, false>();
    sfpu::calculate_sfpu_isinf_isnan<SfpuType::isnan, false>();
    sfpu::calculate_sfpu_isinf_isnan<SfpuType::isfinite, false>();
    sfpu::logical_not_unary_init<false>();
    sfpu::calculate_logical_not_unary<false>();
    sfpu::recip_init<false>();
    sfpu::calculate_reciprocal<false>();
    sfpu::sqrt_init<false>();
    sfpu::calculate_sqrt<false>();
    sfpu::sine_init<false>();
    sfpu::cosine_init<false>();
    sfpu::tan_init<false>();
    sfpu::calculate_sfpu_trig<SfpuType::sine, false>();
    sfpu::calculate_sfpu_trig<SfpuType::cosine, false>();
    sfpu::calculate_sfpu_trig<SfpuType::tan, false>();
}

END_NS_LLK

