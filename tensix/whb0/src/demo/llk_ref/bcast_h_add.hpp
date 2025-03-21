// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf

#include "compute/bcast.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace TRISC_NS {

using namespace llk::compute::TRISC_NS;

// bcast_h_add

void bcast_h_add_main() {
    init_bcast<EltwiseBinaryType::ELWADD, BroadcastType::ROW>(0, 1, 16);
    acquire_dst(DstMode::Half); 
    add_tiles_bcast<BroadcastType::ROW>(0, 1, 0, 0, 0);
    pack_tile(0, 16); 
    release_dst(DstMode::Half); 
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

