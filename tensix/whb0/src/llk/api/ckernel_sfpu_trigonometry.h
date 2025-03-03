// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "sfpi/sfpi.hpp"

#include "ckernel_defs.h"
#include "ckernel_sfpi.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

using namespace sfpi;

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat sfpu_tangent_maclaurin_series(vFloat val) {
    // Mclauren series
    // tan(x) = x + (x^3)/3 + (2x^5)/15 + (17x^7)/315 + (62x^9)/2835 + (1382x^11)/155925 + (21844x^13)/6081075 + ...

    vFloat tmp = val;
    vFloat val_square = val * val;

    // x
    vFloat output = tmp;
    // x^3/3
    tmp = tmp * val_square;
    output += 0.3333333333333333f * tmp;
    // (2x^5)/15
    tmp = tmp * val_square;
    output += 0.13333333333333333f * tmp;

    // (17x^7)/315
    tmp = tmp * val_square;
    output += 0.05396825396825397f * tmp;

    // (62x^9)/2835
    tmp = tmp * val_square;
    output += 0.021869488536155203f * tmp;

    // (1382x^11)/155925
    tmp = tmp * val_square;
    output += 0.008863235529902197f * tmp;

    // (21844x^13)/6081075
    tmp = tmp * val_square;
    output += 0.003592128036572481f * tmp;

    // Write out output
    return output;
}

#define PI  3.14159265358979323846f

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_tangent() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        // Periodic, Range Reduction: To cover more input range
        v_if (v > PI_2) {
            v = v - PI;
        } v_elseif (v < -PI_2) {
            v = v + PI;
        } v_else {
            v = v;
        } v_endif;
        v = sfpu_tangent_maclaurin_series<APPROXIMATION_MODE>(v);
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat sfpu_sine_maclaurin_series(vFloat val) {
    // Good for [-pi:pi]
    // Mclauren series = x - x^3/3! + x^5/5! - x^7/7! + x^9/9! - x^11/11!
    vFloat tmp = val;
    // x
    vFloat output = tmp;
    // x^3/3!
    tmp = tmp * val * val;
    output += -0.166666666f * tmp;
    // x^5/5!
    tmp = tmp * val * val;
    output += 0.0083333333f * tmp;
    // x^7/7!
    tmp = tmp * val * val;
    output += -0.0001984126f * tmp;

    // x^9/9!
    tmp = tmp * val * val;
    output += 0.0000027557f * tmp;

    // x^11/11!
    tmp = tmp * val * val;
    output += -0.00000002505f * tmp;

    if constexpr (!APPROXIMATION_MODE) {
#if 0 // TODO: Revise this (apparent bug in original code)
        // x^11/11!
        tmp = tmp * val * val;
        output += -0.00000002505f * tmp;
#endif

        // x^13/13!
        tmp = tmp * val * val;
        output += 1.6059043836821613e-10f * tmp;
    }

    // Write out output
    return output;
}

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat sfpu_cosine_maclaurin_series(vFloat val) {
    // Good for [-pi:pi]
    // Mclauren series = 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! - x^10/10! + x^12/12!
    // 1
    vFloat output = 1.0f;
    // x^2/2!
    vFloat tmp = val * val;
    output += -0.5f * tmp;
    // x^4/4!
    tmp = tmp * val * val;
    output += 0.0416666666f * tmp;
    // x^6/6!
    tmp = tmp * val * val;
    output += -0.0013888888f * tmp;

    // x^8/8!
    tmp = tmp * val * val;
    output += 0.0000248015f * tmp;

    // x^10/10!
    tmp = tmp * val * val;
    output += -0.0000002755f * tmp;

    if constexpr (!APPROXIMATION_MODE) {
        // x^12/12!
        tmp = tmp * val * val;
        output += 2.08767569878681e-9f * tmp;

        // x^14/14!
        tmp = tmp * val * val;
        output += -1.1470745597729725e-11f * tmp;
    }

    // Write out output
    return output;
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_sine() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v = 0.318309886183791f * v; // *1/pi to get number of pi rads.
        vInt whole_v = float_to_int16(v);
        vFloat whole_v_float = int32_to_float(whole_v, 0);
        v = v - whole_v_float;
        v *= 3.141592653589793f; // fractional * pi to get it in [-pi:pi]
        v = sfpu_sine_maclaurin_series<APPROXIMATION_MODE>(v);
        whole_v = whole_v & 0x1;
        v_if (whole_v != 0) {
            // odd so flip the sign
            v *= -1;
        }
        v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_cosine() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v = 0.318309886183791f * v; // *1/pi to get number of pi rads.
        vInt whole_v = float_to_int16(v);
        vFloat whole_v_float = int32_to_float(whole_v, 0);
        v = v - whole_v_float;
        v *= 3.141592653589793f; // fractional * pi to get it in [-pi:pi]
        v = sfpu_cosine_maclaurin_series<APPROXIMATION_MODE>(v);
        whole_v = whole_v & 0x1;
        v_if (whole_v != 0) {
            // odd so flip the sign
            v *= -1;
        }
        v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <SfpuType operation, bool APPROXIMATION_MODE, int ITERATIONS = 8>
inline void calculate_sfpu_trig() {
    if constexpr (operation == SfpuType::sine) {
        calculate_sine<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::cosine) {
        calculate_cosine<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::tan) {
        calculate_tangent<APPROXIMATION_MODE, ITERATIONS>();
    }
}

template <bool APPROXIMATION_MODE>
void sine_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void cosine_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void tan_init() {
    // nothing to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

