// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace ronin {
namespace iss {
namespace whb0 {
namespace schedule {

class Coro;

//
//    Worker
//

class Worker {
public:
    Worker();
    virtual ~Worker();
public:
    virtual void run() = 0;
    virtual void on_yield() { }
    virtual void on_resume() { }
public:
    void set_cond(std::function<bool ()> cond);
    bool can_resume();
    void reset(size_t stack_size);
    void clear();
    void resume();
    void yield();
    bool is_alive();
protected:
    std::unique_ptr<Coro> m_co;
    std::function<bool ()> m_cond;
};

//
//    Scheduler
//

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
public:
    void set_stack_size(size_t stack_size);
    void add_worker(Worker *worker);
    void wait(std::function<bool ()> cond);
    void run();
private:
    void reset();
    void clear();
    bool schedule();
    void check_completion();
private:
    size_t m_stack_size;
    std::vector<Worker *> m_workers;
    bool m_running;
    int m_curr_worker_index;
};

} // namespace schedule
} // namespace whb0
} // namespace iss
} // namespace ronin

