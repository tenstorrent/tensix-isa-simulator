// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

// from "kernels/hostdevcommon/kernel_structs.h"

// Move to "defs.hpp"?

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {

enum class DstMode {
    Full          = 0,
    Half          = 1,
    Tile          = 2,
    NUM_DST_MODES = 3,
};

} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

