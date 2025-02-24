// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#define MATH_FIDELITY 0
#define SYNC DstSync::SyncHalf

#if 0 // TODO: Revise this (macros replaced with templates)
#define ELTWISE_OP
#define ELTWISE_OP_CODE 0
#endif

#include "compute/eltwise_binary.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace TRISC_NS {

using namespace ckernel::TRISC_NS;

// binary_add

void binary_add_main() {
    binary_op_init_common(0, 1);
#if 0 // TODO: Revise this (macros replaced with templates)
    binary_op_specific_init(0);
#endif
    binary_op_specific_init<EltwiseBinaryType::ELWADD>();
    acquire_dst(DstMode::Half); 
    add_tiles(0, 1, 0, 0, 0);
    pack_tile(0, 16); 
    release_dst(DstMode::Half); 
}

} // namespace TRISC_NS
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

