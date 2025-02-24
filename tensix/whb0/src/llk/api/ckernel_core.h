// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
//    Convenience header for use in LLK code
//

#include "ckernel_ops.h"
#include "ckernel_template.h"
#include "ckernel_thread.h"
#include "ckernel_asm.h"

#define BEGIN_NS_LLK \
    namespace ronin { \
    namespace iss { \
    namespace whb0 { \
    namespace llk { \
    namespace ref { \
    namespace ckernel = ronin::iss::whb0::llk::api;

#define END_NS_LLK } } } } }

