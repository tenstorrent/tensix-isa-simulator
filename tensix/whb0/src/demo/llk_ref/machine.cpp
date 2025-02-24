// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <memory>
#include <functional>

#include "schedule/schedule.hpp"

#include "core/memory.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "api/ckernel_core.h"

#include "trisc.hpp"
#include "machine.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {

using schedule::Scheduler;
using schedule::Worker;

using core::Memory;
using core::BasicMemory;
using core::Tensix;
using core::ThreadSync;

using llk::api::ThreadContext;
using llk::api::ThreadBase;

//
//    MachineWorker
//

MachineWorker::MachineWorker():
        m_thread(nullptr) { }

MachineWorker::~MachineWorker() { }

void MachineWorker::reset(Trisc *thread) {
    m_thread = thread;
}

void MachineWorker::run() {
    assert(m_thread != nullptr);
    m_thread->run();
}

void MachineWorker::on_yield() {
    m_thread->read_globals();
}

void MachineWorker::on_resume() {
    ThreadBase::set_curr_thread(m_thread);
    m_thread->write_globals();
}

//
//    MachineSync
//

MachineSync::MachineSync():
        m_thread(nullptr) { }

MachineSync::~MachineSync() { }

void MachineSync::reset(Trisc *thread) {
    m_worker.reset(thread);
    m_thread = thread;
}

void MachineSync::wait(std::function<bool ()> cond) {
    // ACHTUNG: Use Scheduler::wait instead ??
    if (cond()) {
        return;
    }
    m_worker.set_cond(cond);
    m_worker.on_yield();
    m_worker.yield();
}

//
//    Machine
//

Machine::Machine():
        m_memory(3 * 512 * 1024),
        m_tensix(&m_memory) { }

Machine::~Machine() { }

void Machine::setup_cb(
        uint32_t cb_id,
        uint32_t fifo_addr,
        uint32_t fifo_size,
        uint32_t fifo_num_pages,
        uint32_t fifo_page_size) {
    m_thread_context.setup_cb(cb_id, fifo_addr, fifo_size, fifo_num_pages, fifo_page_size);
}

void Machine::cb_push_back(uint32_t cb_id, uint32_t num_pages) {
    m_thread_context.cb_push_back(cb_id, num_pages);
}

void Machine::cb_pop_front(uint32_t cb_id, uint32_t num_pages) {
    m_thread_context.cb_pop_front(cb_id, num_pages);
}

uint8_t *Machine::cb_rd_tile_ptr(uint32_t cb_id, uint32_t tile_index) {
    uint32_t addr = m_thread_context.cb_rd_tile_addr(cb_id, tile_index);
    return m_memory.map_addr(addr * 16);
}

uint8_t *Machine::cb_wr_tile_ptr(uint32_t cb_id, uint32_t tile_index) {
    uint32_t addr = m_thread_context.cb_wr_tile_addr(cb_id, tile_index);
    return m_memory.map_addr(addr * 16);
}

void Machine::reset_memory() {
    m_memory.reset();
}

void Machine::run(
        std::function<void ()> unpack_main,
        std::function<void ()> math_main,
        std::function<void ()> pack_main) {
    m_unpack.reset(new Unpack(&m_tensix, &m_unpack_sync, &m_thread_context, unpack_main));
    m_math.reset(new Math(&m_tensix, &m_math_sync, &m_thread_context, math_main));
    m_pack.reset(new Pack(&m_tensix, &m_pack_sync, &m_thread_context, pack_main));

    m_unpack_sync.reset(m_unpack.get());
    m_math_sync.reset(m_math.get());
    m_pack_sync.reset(m_pack.get());

    m_scheduler.reset(new Scheduler());
    m_scheduler->add_worker(m_unpack_sync.worker());
    m_scheduler->add_worker(m_math_sync.worker());
    m_scheduler->add_worker(m_pack_sync.worker());
    m_scheduler->run();
}

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

