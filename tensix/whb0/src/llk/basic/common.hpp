// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "llk/basic/defs.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

struct ThreadParams {
    uint32_t MATH_FIDELITY;
    DstSync SYNC;
    bool APPROX;
};

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

