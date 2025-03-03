// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
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

#include "api/ckernel_core.h"

#include "trisc.hpp"

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

class MachineWorker: public Worker {
public:
    MachineWorker();
    ~MachineWorker();
public:
    void reset(Trisc *thread);
    void run() override;
    void on_yield();
    void on_resume();
private:
    Trisc *m_thread;
};

class MachineSync: public ThreadSync {
public:
    MachineSync();
    ~MachineSync();
public:
    void reset(Trisc *thread);
    Worker *worker() {
        return &m_worker;
    }
    void wait(std::function<bool ()> cond) override;
private:
    MachineWorker m_worker;
    Trisc *m_thread;
};

class Machine {
public:
    Machine();
    ~Machine();
public:
    void setup_cb(
        uint32_t cb_id,
        uint32_t fifo_addr,
        uint32_t fifo_size,
        uint32_t fifo_num_pages,
        uint32_t fifo_page_size);
    void cb_push_back(uint32_t cb_id, uint32_t num_pages);
    void cb_pop_front(uint32_t cb_id, uint32_t num_pages);
    uint8_t *cb_rd_tile_ptr(uint32_t cb_id, uint32_t tile_index);
    uint8_t *cb_wr_tile_ptr(uint32_t cb_id, uint32_t tile_index);
    void reset_memory();
    uint8_t *map_addr(uint32_t addr) {
        return m_memory.map_addr(addr);
    }
    float *dest_row_data(uint32_t row_index) {
        return m_tensix.dest_row_data(row_index);
    }
    void run(
        std::function<void ()> unpack_main,
        std::function<void ()> math_main,
        std::function<void ()> pack_main);
private:
    BasicMemory m_memory;
    Tensix m_tensix;
    MachineSync m_unpack_sync;
    MachineSync m_math_sync;
    MachineSync m_pack_sync;
    ThreadContext m_thread_context;
    std::unique_ptr<Unpack> m_unpack;
    std::unique_ptr<Math> m_math;
    std::unique_ptr<Pack> m_pack;
    std::unique_ptr<Scheduler> m_scheduler;
};

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

