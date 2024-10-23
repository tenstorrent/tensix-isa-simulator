#pragma once

#include <cstdint>
#include <limits>

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

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isfinite() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity() || 
                v == -std::numeric_limits<float>::infinity() ||
                v == std::numeric_limits<float>::quiet_NaN() || 
                v == std::numeric_limits<float>::signaling_NaN()) {
            v = 0.0f;
        } v_else {
            v = 1.0f;
        } v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}


template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isinf() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity() || 
                v == -std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        } v_else {
            v = 0.0f;
        } v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isposinf() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        } v_else {
            v = 0.0f;
        } v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isneginf() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == -std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        } v_else {
            v = 0.0f;
        } v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isnan() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::quiet_NaN() || 
                v == std::numeric_limits<float>::signaling_NaN()) {
            v = 1.0f;
        } v_else {
            v = 0.0f;
        } v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    end_sfpi();
}

template <SfpuType operation, bool APPROXIMATION_MODE, int ITERATIONS = 8>
inline void calculate_sfpu_isinf_isnan() {
    if constexpr (operation == SfpuType::isinf) {
        calculate_isinf<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::isposinf) {
        calculate_isposinf<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::isneginf) {
        calculate_isneginf<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::isnan) {
        calculate_isnan<APPROXIMATION_MODE, ITERATIONS>();
    } else if constexpr (operation == SfpuType::isfinite) {
        calculate_isfinite<APPROXIMATION_MODE, ITERATIONS>();
    }
}

template <bool APPROXIMATION_MODE>
void isinf_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void isposinf_init() {
    // iothing to do
}

template <bool APPROXIMATION_MODE>
void isneginf_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void isnan_init() {
    // nothing to do
}

template <bool APPROXIMATION_MODE>
void isfinite_init() {
    // nothing to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

