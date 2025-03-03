// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "ckernel.h"
#include "ckernel_defs.h"
#include "noc_nonblocking_api.h"

#include "sfpi.h"

using namespace sfpi;

namespace ckernel {
namespace sfpu {

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isfinite()
{
    BEGIN_SFPI

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++)
    {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity() || v == -std::numeric_limits<float>::infinity() ||
              v == std::numeric_limits<float>::quiet_NaN() || v == std::numeric_limits<float>::signaling_NaN()) {
            v = 0.0f;
        }v_else {
            v = 1.0f;
        }v_endif;

        dst_reg[0] = v;
        dst_reg++;
    }

    END_SFPI
}


template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isinf()
{
    BEGIN_SFPI

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++)
    {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity() || v == -std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        }v_else {
            v = 0.0f;
        }v_endif;

        dst_reg[0] = v;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isposinf()
{
    BEGIN_SFPI

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++)
    {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        }v_else {
            v = 0.0f;
        }v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isneginf()
{
    BEGIN_SFPI

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++)
    {
        vFloat v = dst_reg[0];
        v_if (v == -std::numeric_limits<float>::infinity()) {
            v = 1.0f;
        }v_else {
            v = 0.0f;
        }v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE, int ITERATIONS>
inline void calculate_isnan()
{
    BEGIN_SFPI

    // SFPU microcode
    for (int d = 0; d < ITERATIONS; d++)
    {
        vFloat v = dst_reg[0];
        v_if (v == std::numeric_limits<float>::quiet_NaN() || v == std::numeric_limits<float>::signaling_NaN()) {
            v = 1.0f;
        }v_else {
            v = 0.0f;
        }v_endif;
        dst_reg[0] = v;
        dst_reg++;
    }

    END_SFPI
}

template <SfpuType operation, bool APPROXIMATION_MODE, int ITERATIONS=8>
inline void calculate_sfpu_isinf_isnan() {

    if constexpr (operation == SfpuType::isinf) {
        calculate_isinf<APPROXIMATION_MODE, ITERATIONS>();
    }
    else if constexpr (operation == SfpuType::isposinf) {
        calculate_isposinf<APPROXIMATION_MODE, ITERATIONS>();
    }
    else if constexpr (operation == SfpuType::isneginf) {
        calculate_isneginf<APPROXIMATION_MODE, ITERATIONS>();
    }
    else if constexpr (operation == SfpuType::isnan) {
        calculate_isnan<APPROXIMATION_MODE, ITERATIONS>();
    }
    else if constexpr (operation == SfpuType::isfinite) {
        calculate_isfinite<APPROXIMATION_MODE, ITERATIONS>();
    }
}

template <bool APPROXIMATION_MODE>
void isinf_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void isposinf_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void isneginf_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void isnan_init() {
    ;
}

template <bool APPROXIMATION_MODE>
void isfinite_init() {
    ;
}

}  // namespace sfpu
}  // namespace ckernel
