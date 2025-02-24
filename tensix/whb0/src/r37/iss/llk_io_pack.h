// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
//    NOTE: This implementation differs from original as it uses
//        thread primitives instead of direct coding of CB functionality
//

#include "ckernel_core.h"

namespace ckernel {

template <bool skip_sync = false, bool wait_for_blocks = false, bool brisc_pack = false>
inline void llk_wait_for_free_tiles(uint32_t operand, uint32_t num_tiles) {
    // ACHTUNG: Non-default template parameters are not supported
    cb_wait_for_free_tiles(operand, num_tiles);
}

template <bool push_blocks = false, bool brisc_pack = false>
inline void llk_push_tiles(uint32_t operand, uint32_t num_tiles) {
    // ACHTUNG: Non-default template parameters are not supported
    cb_push_tiles(operand, num_tiles);
}

inline void llk_setup_outputs() {
    // nothing to do in ISS version
    // (output streams shall be set up externally via Thread methods)
}

} // namespace ckernel

