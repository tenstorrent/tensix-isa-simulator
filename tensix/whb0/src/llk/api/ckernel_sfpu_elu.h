// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "sfpi/sfpi.hpp"

#include "ckernel_instr_params.h"
#include "ckernel_defs.h"
#include "ckernel_sfpi.h"
#include "ckernel_sfpu_converter.h"
#include "ckernel_sfpu_exp.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

using namespace sfpi;

template <bool APPROXIMATION_MODE>
inline void calculate_elu(uint32_t slope) {
    begin_sfpi();

    // SFPU microcode
    constexpr bool zero_negative = true;
    Converter c_slope;
    c_slope.u = slope;
    vFloat s = c_slope.f;

#pragma GCC unroll 0
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v < 0.0f) {
            vFloat v_exp = 
                calculate_exponential_body_improved<APPROXIMATION_MODE, zero_negative>(v);
            v = s * (v_exp - 1.0f);
        }
        v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
void elu_init() {
    begin_sfpi();

    if constexpr (APPROXIMATION_MODE) {
        vConstFloatPrgm0 = 1.442695f; // ln2_recip
        vConstFloatPrgm1 = s2vFloat16b(p_exp::C23_73);
        vConstFloatPrgm2 = s2vFloat16b(p_exp::ADJ_EXP);
    }
    else{
        vConstFloatPrgm0 = 1.442695f; // ln2_recip
        vConstFloatPrgm1 = 2.0f;
        vConstFloatPrgm2 = 0.863281f;
    }

    end_sfpi();
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

