// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/math.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_setrwc;
using core::p_stall;

namespace {

constexpr uint32_t DstTileSize[3] = {
    64,    // 32x32 tile shape
    32,    // 32x16, 16x32 tile shape
    16     // 16x16 tile shape
};

constexpr uint32_t DstTileSizeLog2[3] = {
    6,    // 32x32 tile shape
    5,    // 32x16, 16x32 tile shape
    4     // 16x16 tile shape
};
 
} // namespace

//
//    Math
//

// ckernels

void Math::reset_counters(uint32_t setrwc) {
    SETRWC(p_setrwc::CLR_NONE, 0, 0, 0, 0, setrwc);
} 

void Math::set_dst_write_addr(
        DstTileLayout layout, 
        DstTileShape tile_shape,
        uint32_t tile_index) {
    assert(layout == DstTileLayout::Default);
    uint32_t dst_index = tile_index << DstTileSizeLog2[int(tile_shape)];
    dst_index = dst_index + get_dest_buffer_base();
    set_dest_offset(dst_index);
}

void Math::clear_dst_reg_addr() {
    SETRWC(p_setrwc::CLR_NONE, 0, 0, 0, 0, p_setrwc::SET_D); 
}

uint32_t Math::get_dest_buffer_base() {
    // TODO: Implement once 'dest_offset_id' flipping is supported
    return 0;
}

uint32_t Math::get_math_sync_tile_dst_index() {
    // TODO (Unused by current LLKs)
    return 0;
}

// llk

void Math::math_wait_for_dest_available(DstSync Dst) {
    // math_dest_wait
    wait_sem_math_pack(p_stall::STALL_ON_MAX);
}

void Math::math_dest_section_done(DstSync Dst, bool is_fp32_dest_acc_en) {
    // set_math_semaphores
    post_sem_math_pack();
    // SKIPPED: Dest bank flipping and 'math_sync_tile_dst_index'
}

void Math::math_pack_sync_init(DstSync Dst, bool is_fp32_dest_acc_en) {
    // SKIPPED: tensix_sync
    // SKIPPED: wait for previous packs to finish
    // SKIPPED: init p_stall::SEMAPHORE_1
    // SKIPPED: reset_dest_offset_id
    // SKIPPED: set_dest_section_base
    // SKIPPED: 'math_sync_tile_dst_index'
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

