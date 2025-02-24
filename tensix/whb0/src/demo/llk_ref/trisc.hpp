// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "api/ckernel_core.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {

using core::Tensix;
using core::ThreadType;
using core::ThreadSync;
using core::Thread;

using llk::api::ThreadContext;
using llk::api::ThreadBase;

class Trisc: public ThreadBase {
public:
    Trisc(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main);
    ~Trisc();
public:
    void run() override;
public:
    std::function<void ()> m_main;
};

class Unpack: public Trisc {
public:
    Unpack(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main);
    ~Unpack();
};

class Math: public Trisc {
public:
    Math(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main);
    ~Math();
};

class Pack: public Trisc {
public:
    Pack(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main);
    ~Pack();
};

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

