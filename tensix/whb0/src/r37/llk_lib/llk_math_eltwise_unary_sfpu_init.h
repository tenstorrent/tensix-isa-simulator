/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "llk_math_eltwise_unary_sfpu_common_includes.h"

namespace ckernel {

inline void eltwise_unary_sfpu_configure_addrmod(){
    // NOTE: this kernel is typically used in conjunction with
    //       A2D, which is using ADDR_MOD_0 and ADDR_MOD_2, so use one
    //       that doesn't conflict!

    addr_mod_t{
        .srca = {.incr = 0},
        .srcb = {.incr = 0},
        .dest = {.incr = 0},
    }.set(ADDR_MOD_7);

}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_init(void (*func)()) {
    eltwise_unary_sfpu_configure_addrmod();
    func();
    math::reset_counters(p_setrwc::SET_ABD_F);
}

}
