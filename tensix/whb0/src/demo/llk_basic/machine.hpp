// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include "schedule/schedule.hpp"

#include "core/memory.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/common.hpp"
#include "llk/basic/unpack.hpp"
#include "llk/basic/math.hpp"
#include "llk/basic/pack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using schedule::Scheduler;
using schedule::Worker;

using core::Memory;
using core::BasicMemory;
using core::Tensix;
using core::ThreadSync;
using core::Thread;

using llk::basic::ThreadParams;
using llk::basic::Unpack;
using llk::basic::Math;
using llk::basic::Pack;

class MachineWorker: public Worker {
public:
    MachineWorker();
    ~MachineWorker();
public:
    void reset(Thread *thread);
    void run() override;
private:
    Thread *m_thread;
};

class MachineSync: public ThreadSync {
public:
    MachineSync();
    ~MachineSync();
public:
    void reset(Thread *thread);
    Worker *worker() {
        return &m_worker;
    }
    void wait(std::function<bool ()> cond) override;
private:
    MachineWorker m_worker;
    Thread *m_thread;
};

class Machine {
public:
    Machine();
    ~Machine();
public:
    void reset_memory();
    uint8_t *map_addr(uint32_t addr) {
        return m_memory.map_addr(addr);
    }
    float *dest_row_data(uint32_t row_index) {
        return m_tensix.dest_row_data(row_index);
    }
    void run(
        const ThreadParams &thread_params,
        std::function<void (Unpack *)> unpack_main,
        std::function<void (Math *)> math_main,
        std::function<void (Pack *)> pack_main);
private:
    BasicMemory m_memory;
    Tensix m_tensix;
    MachineSync m_unpack_sync;
    MachineSync m_math_sync;
    MachineSync m_pack_sync;
    std::unique_ptr<Unpack> m_unpack;
    std::unique_ptr<Math> m_math;
    std::unique_ptr<Pack> m_pack;
    std::unique_ptr<Scheduler> m_scheduler;
};

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

