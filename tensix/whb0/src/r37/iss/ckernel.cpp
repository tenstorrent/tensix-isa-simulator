// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel.h"

namespace ckernel {

// originally in "tt_metal/hw/firmware/src/trisc.cc"

MEM_PTR_T reg_base = MAP_MEM_PTR(0xFFB10000);
MEM_PTR_T pc_buf_base = MAP_MEM_PTR(PC_BUF_BASE);
MEM_PTR_T regfile = MAP_MEM_PTR(REGFILE_BASE);

// never accessed (and unsupported) in ISS case
uint32_t *instrn_buffer = nullptr; 

} // namespace ckernel

