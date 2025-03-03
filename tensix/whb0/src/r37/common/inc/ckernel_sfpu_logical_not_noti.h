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


template <bool APPROXIMATION_MODE>
inline void calculate_logical_not_unary()
{
    BEGIN_SFPI

    #pragma GCC unroll 0
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == 0) {
            dst_reg[0] = 1.0f;
        }v_else {
            dst_reg[0] = 0.0f;
        }
        v_endif;
        dst_reg++;
    }

    END_SFPI
}

template <bool APPROXIMATION_MODE>
void logical_not_unary_init() {
    ;
}

}  // namespace sfpu
}  // namespace ckernel
