// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "system/cfg_defines.h"

#include "ckernel_ops.h"
#include "ckernel_thread.h"
#include "ckernel_asm.h"
#include "ckernel_instr_params.h"
#include "ckernel_structs.h"
#include "ckernel_mem_ptr.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

namespace ckernel = llk::api;

extern MemPtr regfile;
extern MemPtr pc_buf_base;

extern uint32_t *instrn_buffer;

inline uint32_t semaphore_read(const uint32_t index) {
    return pc_buf_base[PC_BUF_SEMAPHORE_BASE + index];
}

inline void semaphore_post(const uint32_t index) {
    pc_buf_base[PC_BUF_SEMAPHORE_BASE + index] = 0;
}

inline void semaphore_get(const uint32_t index) {
    pc_buf_base[PC_BUF_SEMAPHORE_BASE + index] = 1;
}

// Tensix thread semaphore post optionally stalled
template <uint32_t WaitRes = p_stall::NONE>
inline void t6_semaphore_post(uint32_t index) {
    if constexpr (WaitRes != p_stall::NONE) {
        TTI_STALLWAIT(p_stall::STALL_SYNC, WaitRes);
    }
    TTI_SEMPOST(semaphore::t6_sem(index));
}

// Tensix thread semaphore get optionally stalled
template <uint32_t WaitRes = p_stall::NONE>
inline void t6_semaphore_get(uint32_t index) {
    if constexpr (WaitRes != p_stall::NONE) {
        TTI_STALLWAIT(p_stall::STALL_SYNC, WaitRes);
    }
    TTI_SEMGET(semaphore::t6_sem(index));
}

// Tensix thread semaphore get optionally stalled
inline void t6_semaphore_init(uint32_t index, uint32_t min_value, uint32_t max_value) {
    TTI_SEMINIT(max_value, min_value, semaphore::t6_sem(index));
}

inline void t6_mutex_acquire(uint32_t index) {
    TTI_ATGETM(index);
}

inline void t6_mutex_release(uint32_t index) {
    TTI_ATRELM(index);
}

// Return pointer to CFG with the right base address for the current state
inline MemPtr get_cfg_pointer() {
    if (cfg_state_id == 0) {
        return MemPtr::map(TENSIX_CFG_BASE);
    } else {
        return MemPtr::map(TENSIX_CFG_BASE + CFG_STATE_SIZE * 16);
    }
}

inline void reset_dest_offset_id() {
    dest_offset_id = 0;
}

// MOP run version without zmask
inline void mop_run(uint32_t type, uint32_t count) {
    TTI_MOP(type, count - 1, 0);
}

inline void sync_regfile_write(uint32_t index) {
    // nothing to do in sumulator case
}

template <uint32_t CfgAddr32, uint32_t Shamt, uint32_t Mask>
inline void cfg_reg_rmw_tensix(uint32_t val) {
    uint32_t wrdata = val << Shamt;
    uint32_t mask_b0 = Mask & 0xff;

    if (mask_b0 != 0) {
        uint8_t data_b0 = wrdata & 0xff;
        TT_RMWCIB0(mask_b0, data_b0, CfgAddr32);
    }

    wrdata >>= 8;
    uint32_t mask_b1 = (Mask >> 8) & 0xff;

    if (mask_b1 != 0) {
        uint8_t data_b1 = wrdata & 0xff;
        TT_RMWCIB1(mask_b1, data_b1, CfgAddr32);
    }

    wrdata >>= 8;
    uint32_t mask_b2 = (Mask >> 16) & 0xff;

    if (mask_b2 != 0) {
        uint8_t data_b2 = wrdata & 0xff;
        TT_RMWCIB2(mask_b2, data_b2, CfgAddr32);
    }

    wrdata >>= 8;
    uint32_t mask_b3 = (Mask >> 24) & 0xff;

    if (mask_b3 != 0) {
        uint8_t data_b3 = wrdata & 0xff;
        TT_RMWCIB3(mask_b3, data_b3, CfgAddr32);
    }
}

inline void tensix_sync() {
    // nothing to do in sumulator case
}

inline uint32_t get_df_item_size(DataFormat df) {
    DataFormat base_df = DataFormat(uint32_t(df) & 0x3);
    return (base_df == DataFormat::Float32) ? 4 : 
        (base_df == DataFormat::Float16) ? 2 : 1;
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

