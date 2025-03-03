// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf

#define APPROX false

#include "compute/tile_move_copy.h"
#include "compute/eltwise_unary.h"
#include "compute/sfpu.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace TRISC_NS {

using namespace ckernel::TRISC_NS;

// borrowed from "ckernel_defs.h", replicated for convenience
enum SfpuType {
    tanh,
    hardtanh,
    gelu,
    exponential,
    exp_with_base,
    sigmoid,
    reciprocal,
    sqrt,
    lrelu,
    power,
    square,
    tanh_derivative,
    log,
    log_with_base,
    equal_zero,
    not_equal_zero,
    less_than_zero,
    greater_than_equal_zero,
    less_than_equal_zero,
    greater_than_zero,
    clamp,
    gelu_derivative,
    dropout,
    abs,
    sign,
    max,
    sine,
    cosine,
    tan,
    relu_max,
    relu_min,
    cast_fp32_to_fp16a,
    sigmoid_appx,
    gelu_appx,
    elu,
    min,
    exp2,
    heaviside,
    expm1,
    signbit,
    asin,
    acos,
    atan,
    erf,
    erfc,
    rsqrt,
    isfinite,
    isinf,
    isposinf,
    isneginf,
    isnan,
    logical_not_unary,
    erfinv,
    i0
};

uint32_t float_as_u32(float x) {
    union U32 {
        float f;
        uint32_t i;
    } u32;
    u32.f = x;
    return u32.i;
}

template <SfpuType OP>
void sfpu_op() {
    if constexpr (OP == SfpuType::tanh) {
        tanh_tile_init();
        tanh_tile(0);
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::hardtanh) {
        hardtanh_tile_init();
        hardtanh_tile(0);
#endif
    } else if constexpr (OP == SfpuType::gelu) {
        gelu_tile_init();
        gelu_tile(0);
    } else if constexpr (OP == SfpuType::exponential) {
        exp_tile_init();
        exp_tile(0);
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::exp_with_base) {
        exp_with_base_tile_init();
        exp_with_base_tile(0);
#endif
    } else if constexpr (OP == SfpuType::sigmoid) {
        sigmoid_tile_init();
        sigmoid_tile(0);
    } else if constexpr (OP == SfpuType::reciprocal) {
        recip_tile_init();
        recip_tile(0);
    } else if constexpr (OP == SfpuType::sqrt) {
        sqrt_tile_init();
        sqrt_tile(0);
    } else if constexpr (OP == SfpuType::lrelu) {
        leaky_relu_tile_init();
        leaky_relu_tile(0, float_as_u32(0.1f));
    } else if constexpr (OP == SfpuType::power) {
        power_tile_init();
        power_tile(0, 3);
    } else if constexpr (OP == SfpuType::square) {
        square_tile_init();
        square_tile(0);
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::tanh_derivative) {
        tanh_derivative_tile_init();
        tanh_derivative_tile(0);
#endif
    } else if constexpr (OP == SfpuType::log) {
        log_tile_init();
        log_tile(0);
    } else if constexpr (OP == SfpuType::log_with_base) {
        log_with_base_tile_init();
//        log_with_base_tile(0, float_as_u32(2.0f));
        log_with_base_tile(0, 0x3c00);
    } else if constexpr (OP == SfpuType::equal_zero) {
        eqz_tile_init();
        eqz_tile(0);
    } else if constexpr (OP == SfpuType::not_equal_zero) {
        nez_tile_init();
        nez_tile(0);
    } else if constexpr (OP == SfpuType::less_than_zero) {
        ltz_tile_init();
        ltz_tile(0);
    } else if constexpr (OP == SfpuType::greater_than_equal_zero) {
        gez_tile_init();
        gez_tile(0);
    } else if constexpr (OP == SfpuType::less_than_equal_zero) {
        lez_tile_init();
        lez_tile(0);
    } else if constexpr (OP == SfpuType::greater_than_zero) {
        gtz_tile_init();
        gtz_tile(0);
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::clamp) {
        clamp_tile_init();
        clamp_tile(0);
    } else if constexpr (OP == SfpuType::gelu_derivative) {
        gelu_derivative_tile_init();
        gelu_derivative_tile(0);
    } else if constexpr (OP == SfpuType::dropout) {
        dropout_tile_init();
        dropout_tile(0);
#endif
    } else if constexpr (OP == SfpuType::abs) {
        abs_tile_init();
        abs_tile(0);
    } else if constexpr (OP == SfpuType::sign) {
        sign_tile_init();
        sign_tile(0);
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::max) {
        max_tile_init();
        max_tile(0);
#endif
    } else if constexpr (OP == SfpuType::sine) {
        sin_tile_init();
        sin_tile(0);
    } else if constexpr (OP == SfpuType::cosine) {
        cos_tile_init();
        cos_tile(0);
    } else if constexpr (OP == SfpuType::tan) {
        tan_tile_init();
        tan_tile(0);
    } else if constexpr (OP == SfpuType::relu_max) {
        relu_max_tile_init();
        relu_max_tile(0, float_as_u32(1.0f));
    } else if constexpr (OP == SfpuType::relu_min) {
        relu_min_tile_init();
        relu_min_tile(0, float_as_u32(1.0f));
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::cast_fp32_to_fp16a) {
        cast_fp32_to_fp16a_tile_init();
        cast_fp32_to_fp16a_tile(0);
    } else if constexpr (OP == SfpuType::sigmoid_appx) {
        sigmoid_appx_tile_init();
        sigmoid_appx_tile(0);
    } else if constexpr (OP == SfpuType::gelu_appx) {
        gelu_appx_tile_init();
        gelu_appx_tile(0);
#endif
    } else if constexpr (OP == SfpuType::elu) {
        elu_tile_init();
        elu_tile(0, float_as_u32(0.1f));
#if 0 // Unsupported
    } else if constexpr (OP == SfpuType::min) {
        min_tile_init();
        min_tile(0);
#endif
    } else if constexpr (OP == SfpuType::exp2) {
        exp2_tile_init();
        exp2_tile(0);
    } else if constexpr (OP == SfpuType::heaviside) {
        heaviside_tile_init();
        heaviside_tile(0, float_as_u32(0.5f));
    } else if constexpr (OP == SfpuType::expm1) {
        expm1_tile_init();
        expm1_tile(0);
    } else if constexpr (OP == SfpuType::signbit) {
        signbit_tile_init();
        signbit_tile(0);
    } else if constexpr (OP == SfpuType::asin) {
        asin_tile_init();
        asin_tile(0);
    } else if constexpr (OP == SfpuType::acos) {
        acos_tile_init();
        acos_tile(0);
    } else if constexpr (OP == SfpuType::atan) {
        atan_tile_init();
        atan_tile(0);
    } else if constexpr (OP == SfpuType::erf) {
        erf_tile_init();
        erf_tile(0);
    } else if constexpr (OP == SfpuType::erfc) {
        erfc_tile_init();
        erfc_tile(0);
    } else if constexpr (OP == SfpuType::rsqrt) {
        rsqrt_tile_init();
        rsqrt_tile(0);
    } else if constexpr (OP == SfpuType::isfinite) {
        isfinite_tile_init();
        isfinite_tile(0);
    } else if constexpr (OP == SfpuType::isinf) {
        isinf_tile_init();
        isinf_tile(0);
    } else if constexpr (OP == SfpuType::isposinf) {
        isposinf_tile_init();
        isposinf_tile(0);
    } else if constexpr (OP == SfpuType::isneginf) {
        isneginf_tile_init();
        isneginf_tile(0);
    } else if constexpr (OP == SfpuType::isnan) {
        isnan_tile_init();
        isnan_tile(0);
    } else if constexpr (OP == SfpuType::logical_not_unary) {
        logical_not_unary_tile_init();
        logical_not_unary_tile(0);
    } else if constexpr (OP == SfpuType::erfinv) {
        erfinv_tile_init();
        erfinv_tile(0);
    } else if constexpr (OP == SfpuType::i0) {
        i0_tile_init();
        i0_tile(0);
    } else {
        assert(false);
    }
}

template <SfpuType OP>
void sfpu_kernel() {
    init_sfpu(0);
    acquire_dst(DstMode::Half); 
    copy_tile(0, 0, 0);
    sfpu_op<OP>();
    pack_tile(0, 16); 
    release_dst(DstMode::Half); 
}

void sfpu_tanh_main() {
    sfpu_kernel<SfpuType::tanh>();
}

void sfpu_hardtanh_main() {
    sfpu_kernel<SfpuType::hardtanh>();
}

void sfpu_gelu_main() {
    sfpu_kernel<SfpuType::gelu>();
}

void sfpu_exponential_main() {
    sfpu_kernel<SfpuType::exponential>();
}

void sfpu_exp_with_base_main() {
    sfpu_kernel<SfpuType::exp_with_base>();
}

void sfpu_sigmoid_main() {
    sfpu_kernel<SfpuType::sigmoid>();
}

void sfpu_reciprocal_main() {
    sfpu_kernel<SfpuType::reciprocal>();
}

void sfpu_sqrt_main() {
    sfpu_kernel<SfpuType::sqrt>();
}

void sfpu_lrelu_main() {
    sfpu_kernel<SfpuType::lrelu>();
}

void sfpu_power_main() {
    sfpu_kernel<SfpuType::power>();
}

void sfpu_square_main() {
    sfpu_kernel<SfpuType::square>();
}

void sfpu_tanh_derivative_main() {
    sfpu_kernel<SfpuType::tanh_derivative>();
}

void sfpu_log_main() {
    sfpu_kernel<SfpuType::log>();
}

void sfpu_log_with_base_main() {
    sfpu_kernel<SfpuType::log_with_base>();
}

void sfpu_equal_zero_main() {
    sfpu_kernel<SfpuType::equal_zero>();
}

void sfpu_not_equal_zero_main() {
    sfpu_kernel<SfpuType::not_equal_zero>();
}

void sfpu_less_than_zero_main() {
    sfpu_kernel<SfpuType::less_than_zero>();
}

void sfpu_greater_than_equal_zero_main() {
    sfpu_kernel<SfpuType::greater_than_equal_zero>();
}

void sfpu_less_than_equal_zero_main() {
    sfpu_kernel<SfpuType::less_than_equal_zero>();
}

void sfpu_greater_than_zero_main() {
    sfpu_kernel<SfpuType::greater_than_zero>();
}

void sfpu_clamp_main() {
    sfpu_kernel<SfpuType::clamp>();
}

void sfpu_gelu_derivative_main() {
    sfpu_kernel<SfpuType::gelu_derivative>();
}

void sfpu_dropout_main() {
    sfpu_kernel<SfpuType::dropout>();
}

void sfpu_abs_main() {
    sfpu_kernel<SfpuType::abs>();
}

void sfpu_sign_main() {
    sfpu_kernel<SfpuType::sign>();
}

void sfpu_max_main() {
    sfpu_kernel<SfpuType::max>();
}

void sfpu_sine_main() {
    sfpu_kernel<SfpuType::sine>();
}

void sfpu_cosine_main() {
    sfpu_kernel<SfpuType::cosine>();
}

void sfpu_tan_main() {
    sfpu_kernel<SfpuType::tan>();
}

void sfpu_relu_max_main() {
    sfpu_kernel<SfpuType::relu_max>();
}

void sfpu_relu_min_main() {
    sfpu_kernel<SfpuType::relu_min>();
}

void sfpu_cast_fp32_to_fp16a_main() {
    sfpu_kernel<SfpuType::cast_fp32_to_fp16a>();
}

void sfpu_sigmoid_appx_main() {
    sfpu_kernel<SfpuType::sigmoid_appx>();
}

void sfpu_gelu_appx_main() {
    sfpu_kernel<SfpuType::gelu_appx>();
}

void sfpu_elu_main() {
    sfpu_kernel<SfpuType::elu>();
}

void sfpu_min_main() {
    sfpu_kernel<SfpuType::min>();
}

void sfpu_exp2_main() {
    sfpu_kernel<SfpuType::exp2>();
}

void sfpu_heaviside_main() {
    sfpu_kernel<SfpuType::heaviside>();
}

void sfpu_expm1_main() {
    sfpu_kernel<SfpuType::expm1>();
}

void sfpu_signbit_main() {
    sfpu_kernel<SfpuType::signbit>();
}

void sfpu_asin_main() {
    sfpu_kernel<SfpuType::asin>();
}

void sfpu_acos_main() {
    sfpu_kernel<SfpuType::acos>();
}

void sfpu_atan_main() {
    sfpu_kernel<SfpuType::atan>();
}

void sfpu_erf_main() {
    sfpu_kernel<SfpuType::erf>();
}

void sfpu_erfc_main() {
    sfpu_kernel<SfpuType::erfc>();
}

void sfpu_rsqrt_main() {
    sfpu_kernel<SfpuType::rsqrt>();
}

void sfpu_isfinite_main() {
    sfpu_kernel<SfpuType::isfinite>();
}

void sfpu_isinf_main() {
    sfpu_kernel<SfpuType::isinf>();
}

void sfpu_isposinf_main() {
    sfpu_kernel<SfpuType::isposinf>();
}

void sfpu_isneginf_main() {
    sfpu_kernel<SfpuType::isneginf>();
}

void sfpu_isnan_main() {
    sfpu_kernel<SfpuType::isnan>();
}

void sfpu_logical_not_unary_main() {
    sfpu_kernel<SfpuType::logical_not_unary>();
}

void sfpu_erfinv_main() {
    sfpu_kernel<SfpuType::erfinv>();
}

void sfpu_i0_main() {
    sfpu_kernel<SfpuType::i0>();
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

