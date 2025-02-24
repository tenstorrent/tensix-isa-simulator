// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

union Converter {
    float f;
    uint32_t u;
    static float to_float(uint32_t v) {
        Converter c;
        c.u = v;
        return c.f;
    }
};

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

