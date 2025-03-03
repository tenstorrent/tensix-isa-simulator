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
inline void calculate_logical_not_unary() {
    begin_sfpi();

#pragma GCC unroll 0
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == 0) {
            dst_reg[0] = 1.0f;
        } v_else {
            dst_reg[0] = 0.0f;
        }
        v_endif;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
void logical_not_unary_init() {
    // nothing to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

