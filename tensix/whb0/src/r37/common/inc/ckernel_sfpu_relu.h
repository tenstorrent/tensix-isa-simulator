/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "ckernel.h"
#include "ckernel_defs.h"
#include "noc_nonblocking_api.h"
#include "sfpi.h"

using namespace sfpi;

namespace ckernel {
namespace sfpu {



template <bool APPROXIMATION_MODE>
inline void relu_min(uint uint_threshold)
{
    BEGIN_SFPI

    vFloat threshold = Converter::to_float(uint_threshold);
    for (int d = 0; d < WHB0_ITERATIONS; d++)
    {
        vFloat a = dst_reg[0];
        v_if(a < threshold) {
            a = threshold;
        }
        v_endif;
        dst_reg[0] = a;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE>
inline void relu_max(uint uint_threshold)
{
    BEGIN_SFPI

    vFloat threshold = Converter::to_float(uint_threshold);
    for (int d = 0; d < WHB0_ITERATIONS; d++)
    {
        vFloat a = dst_reg[0];
        v_if(a > threshold) {
            a = threshold;
        }
        v_endif;
        v_if(a < 0.0f) {
            a = 0.0f;
        }
        v_endif;
        dst_reg[0] = a;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE>
inline void calculate_lrelu(uint slope)
{
    BEGIN_SFPI

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

    END_SFPI
}

template <bool APPROXIMATION_MODE>
void relu_max_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void relu_min_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void lrelu_init() {
    ;
}


}  // namespace sfpu
}  // namespace ckernel
