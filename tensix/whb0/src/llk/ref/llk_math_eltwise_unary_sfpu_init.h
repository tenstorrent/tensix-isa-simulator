// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_addrmod.h"
#include "ckernel_instr_params.h"
#include "ckernel_defs.h"
#include "cmath_common.h"

BEGIN_NS_LLK

using namespace ckernel;

inline void eltwise_unary_sfpu_configure_addrmod(){
    // NOTE: This kernel is typically used in conjunction with A2D,
    //     which is using ADDR_MOD_0 and ADDR_MOD_2, so use one that doesn't conflict
    addr_mod_t {
        .srca = {.incr = 0},
        .srcb = {.incr = 0},
        .dest = {.incr = 0},
    }
    .set(ADDR_MOD_7);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_init(void (*func)()) {
    eltwise_unary_sfpu_configure_addrmod();
    func();
    math::reset_counters(p_setrwc::SET_ABD_F);
}

END_NS_LLK

