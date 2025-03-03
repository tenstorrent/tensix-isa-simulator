// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "sfpi/sfpi.hpp"

#include "ckernel_defs.h"
#include "ckernel_sfpi.h"
#include "ckernel_sfpu_converter.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

using namespace sfpi;

template <bool APPROXIMATION_MODE>
inline void relu_min(uint32_t uint_threshold) {
    begin_sfpi();

    vFloat threshold = Converter::to_float(uint_threshold);
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat a = dst_reg[0];
        v_if (a < threshold) {
            a = threshold;
        }
        v_endif;
        dst_reg[0] = a;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
inline void relu_max(uint32_t uint_threshold) {
    begin_sfpi();

    vFloat threshold = Converter::to_float(uint_threshold);
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat a = dst_reg[0];
        v_if (a > threshold) {
            a = threshold;
        }
        v_endif;
        v_if (a < 0.0f) {
            a = 0.0f;
        }
        v_endif;
        dst_reg[0] = a;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
inline void calculate_lrelu(uint32_t slope) {
    begin_sfpi();

    // SFPU microcode
    Converter c_slope;
    c_slope.u = slope;
    vFloat s = c_slope.f;

#pragma GCC unroll 0
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v < 0.0f) {
            v *= s;
        }
        v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
void relu_max_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void relu_min_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void lrelu_init() {
    // noting to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

