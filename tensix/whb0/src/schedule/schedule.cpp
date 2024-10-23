
#include <cassert>
#include <string>
#include <functional>
#include <stdexcept>

#include "schedule/minicoro.hpp"
#include "schedule/schedule.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace schedule {

//
//    Worker
//

Worker::Worker():
        m_cond(nullptr) {
    std::function<void ()> func = std::bind(&Worker::run, this);
    m_co.reset(new Coro(func));
}

Worker::~Worker() { }

void Worker::set_cond(std::function<bool ()> cond) {
    m_cond = cond;
}

bool Worker::can_resume() {
    if (m_cond == nullptr) {
        return true;
    }
    if (m_cond()) {
        m_cond = nullptr;
        return true;
    }
    return false;
}

void Worker::reset(size_t stack_size) {
    m_co->reset(stack_size);
}

void Worker::clear() {
    m_co->clear();
}

void Worker::resume() {
    m_co->resume();
}

void Worker::yield() {
    m_co->yield();
}

bool Worker::is_alive() {
    return m_co->is_alive();
}

//
//    Scheduler
//

Scheduler::Scheduler():
        m_stack_size(0),
        m_running(false),
        m_curr_worker_index(0) { }

Scheduler::~Scheduler() { }

void Scheduler::set_stack_size(size_t stack_size) {
    m_stack_size = stack_size;
}

void Scheduler::add_worker(Worker *worker) {
    m_workers.push_back(worker);
}

void Scheduler::wait(std::function<bool ()> cond) {
    assert(m_running);
    if (cond()) {
        return;
    }
    Worker *worker = m_workers[m_curr_worker_index];
    worker->set_cond(cond);
    worker->on_yield();
    worker->yield();
}

void Scheduler::run() {
    reset();
    m_running = true;
    m_curr_worker_index = 0;
    while (schedule()) {
        Worker *worker = m_workers[m_curr_worker_index];
        worker->on_resume();
        worker->resume();
    }
    m_running = false;
    check_completion();
    clear();
}

void Scheduler::reset() {
    for (Worker *worker: m_workers) {
        worker->reset(m_stack_size);
    }
}

void Scheduler::clear() {
    for (Worker *worker: m_workers) {
        worker->clear();
    }
}

bool Scheduler::schedule() {
    int worker_count = int(m_workers.size());
    int index = (m_curr_worker_index + 1) % worker_count;
    while (index != m_curr_worker_index) {
        Worker *worker = m_workers[index];
        if (worker->is_alive() && worker->can_resume()) {
            m_curr_worker_index = index;
            return true;
        }
        index = (index + 1) % worker_count;
    }
    return false;
}

void Scheduler::check_completion() {
    int alive_count = 0;
    for (Worker *worker: m_workers) {
        if (worker->is_alive()) {
            alive_count++;
        }
    }
    if (alive_count != 0) {
        std::string what(
            "None of " + std::to_string(alive_count) + " alive workers can be resumed");
        throw std::runtime_error(what);
    }
}

} // namespace schedule
} // namespace whb0
} // namespace iss
} // namespace ronin

