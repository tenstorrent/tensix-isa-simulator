// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>
#include <functional>

#include "schedule/schedule.hpp"

#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/common.hpp"
#include "llk/basic/unpack.hpp"
#include "llk/basic/math.hpp"

#include "demo/llk_basic/machine.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using schedule::Scheduler;

using core::Thread;

using llk::basic::ThreadParams;
using llk::basic::Unpack;
using llk::basic::Math;
using llk::basic::Pack;

//
//    MachineWorker
//

MachineWorker::MachineWorker():
        m_thread(nullptr) { }

MachineWorker::~MachineWorker() { }

void MachineWorker::reset(Thread *thread) {
    m_thread = thread;
}

void MachineWorker::run() {
    assert(m_thread != nullptr);
    m_thread->run();
}

//
//    MachineSync
//

MachineSync::MachineSync():
        m_thread(nullptr) { }

MachineSync::~MachineSync() { }

void MachineSync::reset(Thread *thread) {
    m_worker.reset(thread);
    m_thread = thread;
}

void MachineSync::wait(std::function<bool ()> cond) {
    if (cond()) {
        return;
    }
    m_worker.set_cond(cond);
    m_worker.yield();
}

//
//    Machine
//

Machine::Machine():
        m_memory(3 * 512 * 1024),
        m_tensix(&m_memory) { }

Machine::~Machine() { }

void Machine::reset_memory() {
    m_memory.reset();
}

void Machine::run(
        const ThreadParams &thread_params,
        std::function<void (Unpack *)> unpack_main,
        std::function<void (Math *)> math_main,
        std::function<void (Pack *)> pack_main) {
    m_unpack.reset(new Unpack(&m_tensix, &m_unpack_sync, thread_params, unpack_main));
    m_math.reset(new Math(&m_tensix, &m_math_sync, thread_params, math_main));
    m_pack.reset(new Pack(&m_tensix, &m_pack_sync, thread_params, pack_main));

    m_unpack_sync.reset(m_unpack.get());
    m_math_sync.reset(m_math.get());
    m_pack_sync.reset(m_pack.get());

    m_scheduler.reset(new Scheduler());
    m_scheduler->add_worker(m_unpack_sync.worker());
    m_scheduler->add_worker(m_math_sync.worker());
    m_scheduler->add_worker(m_pack_sync.worker());
    m_scheduler->run();
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

