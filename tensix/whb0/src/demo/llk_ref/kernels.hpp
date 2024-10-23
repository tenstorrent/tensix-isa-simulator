#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#include "machine.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {

struct KernelSpec {
    std::function<void ()> unpack_main;
    std::function<void ()> math_main;
    std::function<void ()> pack_main;
    std::function<void (Machine *)> init_in0;
    std::function<void (Machine *)> init_in1;
    std::function<void (Machine *, std::vector<float> &)> get_out;
    int out_tiles;
};

KernelSpec *map_kernel(const std::string &name);
void setup_cb(Machine *machine);

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

