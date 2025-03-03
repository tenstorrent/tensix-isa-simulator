// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdlib>
#include <cassert>
#include <string>
#include <functional>
#include <stdexcept>

#include "minicoro/minicoro.h"

#include "schedule/minicoro.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace schedule {

namespace {

void check_result(mco_result res, const std::string &msg) {
    if (res != MCO_SUCCESS) {
        std::string what(msg + ": " + mco_result_description(res));
        fprintf(stderr, "MCO ERROR %s\n", what.c_str());
#if 0 // TODO: Revise this (cannot throw exceptions from coroutines)
        throw std::runtime_error(what);
#endif
        exit(1);
    }
}

} // namespace

//
//    Coro
//

Coro::Coro(std::function<void ()> func):
        m_func(func),
        m_valid(false),
        m_co(nullptr) { }

Coro::~Coro() {
    if (m_valid && m_co != nullptr) {
        mco_destroy(m_co);
    }
}

void Coro::reset(size_t stack_size) {
    assert(!m_valid && m_co == nullptr);
    mco_desc desc = mco_desc_init(wrap_func, stack_size);
    desc.user_data = this;
    mco_result res = mco_create(&m_co, &desc);
    check_result(res, "Failed to create coroutine");
    m_valid = true;
}

void Coro::clear() {
    assert(m_valid && m_co != nullptr);
    mco_result res = mco_destroy(m_co);
    check_result(res, "Failed to destroy coroutine");
    m_valid = false;
    m_co = nullptr;
}

void Coro::resume() {
    assert(m_valid && m_co != nullptr);
    mco_result res = mco_resume(m_co);
    check_result(res, "Failed to resume coroutine");
}

void Coro::yield() {
    assert(m_valid && m_co != nullptr);
    mco_result res = mco_yield(m_co);
    check_result(res, "Failed to yield coroutine");
}

bool Coro::is_alive() {
    return (m_valid && mco_status(m_co) != MCO_DEAD);
}

void Coro::wrap_func(mco_coro *co) {
    Coro *co_wrap = static_cast<Coro *>(mco_get_user_data(co));
    co_wrap->m_func();
}

} // namespace schedule
} // namespace whb0
} // namespace iss
} // namespace ronin

