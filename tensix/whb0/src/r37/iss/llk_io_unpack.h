// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
//    NOTE: This implementation differs from original as it uses
//        thread primitives instead of direct coding of CB functionality
//

#include <cstdint>

#include "ckernel_core.h"

namespace ckernel {

inline void llk_wait_tiles(uint32_t operand, uint32_t num_tiles) {
    cb_wait_tiles(operand, num_tiles);
}

inline void llk_pop_tiles(uint32_t operand, uint32_t num_tiles, uint32_t block_c_dim = 0) {
    // ACHTUNG: Argument block_c_dim is unsupported
    cb_pop_tiles(operand, num_tiles);
}

inline void llk_setup_operands() {
    // nothing to do in ISS version
    // (input streams shall be set up externally via Thread methods)
}

} // namespace ckernel

