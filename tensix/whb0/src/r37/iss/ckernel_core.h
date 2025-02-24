// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cfg_defines.h"

#include "ckernel_ops.h"
#include "ckernel_template.h"
#include "ckernel_thread.h"
#include "ckernel_asm.h"
#include "ckernel_mem_ptr.h"

#ifdef _MSC_VER
#define __attribute__(X)
#endif

