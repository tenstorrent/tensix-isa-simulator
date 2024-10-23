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

// reduce_w_sum

void reduce_w_sum_main() {
    reduce_init<PoolType::SUM, ReduceDim::REDUCE_ROW, true>(0, 1, 16);
    acquire_dst(DstMode::Half); 
    reduce_tile<PoolType::SUM, ReduceDim::REDUCE_ROW>(0, 1, 0, 0, 0);
    pack_tile(0, 16); 
    release_dst(DstMode::Half); 
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

