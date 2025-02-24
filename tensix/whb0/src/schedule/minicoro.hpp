// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "minicoro/minicoro.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace schedule {

class Coro {
public:
    Coro(std::function<void ()> func);
    ~Coro();
public:
    void reset(size_t stack_size);
    void clear();
    void resume();
    void yield();
    bool is_alive();
private:
    static void wrap_func(mco_coro *co);
private:
    std::function<void ()> m_func;
    bool m_valid;
    mco_coro *m_co;
};

} // namespace schedule
} // namespace whb0
} // namespace iss
} // namespace ronin

