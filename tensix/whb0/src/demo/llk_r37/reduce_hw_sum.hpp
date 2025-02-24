// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf

#include "compute/reduce.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace TRISC_NS {

using namespace ckernel::TRISC_NS;

// reduce_hw_sum

void reduce_hw_sum_main() {
    reduce_init<PoolType::SUM, ReduceDim::REDUCE_SCALAR, true>(0, 1, 16);
    acquire_dst(DstMode::Half); 
    reduce_tile<PoolType::SUM, ReduceDim::REDUCE_SCALAR>(0, 1, 0, 0, 0);
    pack_tile(0, 16); 
    release_dst(DstMode::Half); 
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

