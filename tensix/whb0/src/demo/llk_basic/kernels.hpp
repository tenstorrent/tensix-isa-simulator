// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#include "llk/basic/common.hpp"
#include "llk/basic/unpack.hpp"
#include "llk/basic/math.hpp"

#include "demo/llk_basic/machine.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using llk::basic::ThreadParams;
using llk::basic::Unpack;
using llk::basic::Math;

struct KernelSpec {
    ThreadParams *thread_params;
    std::function<void (Unpack *)> unpack_main;
    std::function<void (Math *)> math_main;
    std::function<void (Pack *)> pack_main;
    std::function<void (Machine *)> init_in0;
    std::function<void (Machine *)> init_in1;
    std::function<void (Machine *, std::vector<float> &)> get_out;
    int out_tiles;
};

KernelSpec *map_kernel(const std::string &name);

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

