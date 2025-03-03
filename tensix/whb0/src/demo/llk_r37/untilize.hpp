// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf

#include "compute/untilize.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace TRISC_NS {

using namespace ckernel::TRISC_NS;

// untilize

void untilize_main() {
    uint32_t block_count = 4;
    uint32_t tile_count = 4;
    untilize_init(0, 16);
    for (uint32_t i = 0; i < block_count; i++) {
        cb_wait_front(0, tile_count);
        cb_reserve_back(16, tile_count);
        untilize_block(0, tile_count, 16);
        cb_push_back(16, tile_count);
        cb_pop_front(0, tile_count);
    }
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

