// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h"
#include "ckernel_instr_params.h"
#include "cmath_common.h"

#include "llk_defs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::math;

template <DstSync Dst>
inline void llk_math_wait_for_dest_available() {
    // These liteweight functions for sync with packer imply
    // no mode change - entire epoch is either double buffer or single buffer
    math_dest_wait();
}

template <DstSync Dst = SyncFull, bool is_fp32_dest_acc_en = false>
inline void llk_math_dest_section_done() {
    constexpr uint32_t DEST_NUM_TILES_SHIFT = is_fp32_dest_acc_en ? 1 : 0;
    constexpr uint32_t DEST_NUM_TILES = DEST_NUM_TILES_FP16 >> DEST_NUM_TILES_SHIFT;

    set_math_semaphores();
    if constexpr (Dst == DstSync::SyncHalf || Dst == DstSync::SyncTile2) {
        math_sync_tile_dst_index = 0;
        dest_section_flip();
    } else if constexpr (Dst == DstSync::SyncTile16) {
        math_sync_tile_dst_index++;
        math_sync_tile_dst_index &= (DEST_NUM_TILES - 1);
    }
}

template <DstSync Dst, bool is_fp32_dest_acc_en = false>
inline void llk_math_pack_sync_init() {
    tensix_sync();
    // Wait for previous packs to finish before claiming all dest
    while (semaphore_read(semaphore::MATH_PACK) > 0) { }
    if constexpr (Dst == DstSync::SyncFull) {
        TTI_SEMINIT(1, 0, p_stall::SEMAPHORE_1);
        reset_dest_offset_id();
        set_dest_section_base<StartZero>();
    } else if constexpr (Dst == DstSync::SyncHalf) {
        TTI_SEMINIT(2, 0, p_stall::SEMAPHORE_1);
        reset_dest_offset_id();
        set_dest_section_base<StartZero>();
    } else if constexpr (Dst == DstSync::SyncTile2) {
        TTI_SEMINIT(2, 0, p_stall::SEMAPHORE_1);
        reset_dest_offset_id();
        set_dest_section_base<StartZero>();
        math_sync_tile_dst_index = 0;
    } else {
        static_assert(Dst == DstSync::SyncTile16);

        constexpr uint32_t DEST_NUM_TILES_SHIFT = is_fp32_dest_acc_en ? 1 : 0;
        constexpr uint32_t DEST_NUM_TILES = DEST_NUM_TILES_FP16 >> DEST_NUM_TILES_SHIFT;
        constexpr uint32_t SEM_INIT_MAX = (DEST_NUM_TILES < 15) ? DEST_NUM_TILES : 15;

        TTI_SEMINIT(SEM_INIT_MAX, 0, p_stall::SEMAPHORE_1);
        reset_dest_offset_id();
        set_dest_section_base<StartZero>();
        math_sync_tile_dst_index = 0;
    }
}

END_NS_LLK

