// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "sfpi/sfpi.hpp"

#include "ckernel_defs.h"
#include "ckernel_sfpi.h"

#define POLYVAL5(coef4, coef3, coef2, coef1, coef0, val) \
    ((((coef4 * val + coef3) * val + coef2) * val + coef1) * val + coef0)

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

using namespace sfpi;

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat calculate_erf_body(vFloat x) {
    // assume x >= 0.
    vFloat result = 1.0f;
    v_if (x >= 3.0f) { 
        result = 1.0f; 
    } v_elseif (x >= 1.0f) { 
        result = POLYVAL5(-0.03170029f, 0.31310241f, -1.1603072f, 1.91684792f, -0.19469693f, x); 
    } v_elseif (x >= 0.0f) {
        result = POLYVAL5(0.166342190f, -0.476685015f, 0.0275416549f, 1.12544048f, 0.0000661338118f, x);
    } v_else { 
        // x <= 0.0f
        result = 0.0f; 
    }
    v_endif;
    // TODO: for higher accuracy (non APPROXIMATE) mode use higher degree polynomial.
    return result;
}

// TODO: Fix assertion error for accurate mode
template <bool APPROXIMATION_MODE>
inline void calculate_erf() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat x = dst_reg[0];
        v_if (x < 0.0f) {
            x = -x;
            x = -calculate_erf_body<APPROXIMATION_MODE>(x);
        } v_else { 
            x = calculate_erf_body<APPROXIMATION_MODE>(x); 
        }
        v_endif;
        dst_reg[0] = x;
        dst_reg++;
    }

    end_sfpi();
}

// TODO: Fix assertion error for accurate mode
template <bool APPROXIMATION_MODE>
inline void calculate_erfc() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat x = dst_reg[0];
        v_if (x < 0.0f) { 
            x = -x; // ACHTUNG: Missing in original code
            x = 1.0f + calculate_erf_body<APPROXIMATION_MODE>(x); 
        } v_else { 
            x = 1.0f - calculate_erf_body<APPROXIMATION_MODE>(x); 
        }
        v_endif;
        dst_reg[0] = x;
        dst_reg++;
    }

    end_sfpi();
}

template <SfpuType operation, bool APPROXIMATION_MODE>
inline void calculate_sfpu_erf_erfc() {
    if constexpr (operation == SfpuType::erf) {
        calculate_erf<APPROXIMATION_MODE>();
    } else if constexpr (operation == SfpuType::erfc) {
        calculate_erfc<APPROXIMATION_MODE>();
    }
}

template <bool APPROXIMATION_MODE>
void erf_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void erfc_init() {
    // nothing to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

#undef POLYVAL5

