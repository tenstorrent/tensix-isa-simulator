// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/instr_params.hpp"
#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/unpack.hpp"

//
//    ACHTUNG: Conceptually concurrent actions of UNPACK and MATH are serialized
//        by simulator. This simplified implementation uses single
//        configuration context rather than two flipped configuration contexts.
//        Also, twin SrcA and SrcB register banks are not modelled.
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_setadc;

using core::DataFormat;

//
//    Unpack
//

void Unpack::configure_unpack_AB(
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format,
        uint32_t srca_face_height,
        uint32_t srcb_face_height,
        bool row_pool,
        bool transpose_xy_srca_en,
        bool is_fp32_dest_acc_en) {
    // modeled after "cunpack_common.h"

    // Currently not interpreted (use cases pending):
    //     unpa_dst_format
    //     unpb_dst_format
    //     is_fp32_dest_acc_en

    // SKIPPED: wait_for_idle

    // Reset address counters (unpacker_addr_counter_init)
    SETADCXY(0b011, 0, 0, 0, 0, 0b1011);
    SETADCZW(0b011, 0, 0, 0, 0, 0b1111); 

    // SKIPPED: Strides (TODO: Figure out usage)
    // SKIPPED: ALU payload

    // Set tile descriptor
    DataFormat in_data_format_unp0 = unpa_src_format;
    DataFormat in_data_format_unp1 = row_pool ? DataFormat::Float32 : unpb_src_format;
    uint32_t x_dim = 256;
    uint32_t y_dim = 1;
    uint32_t z_dim = 4;
    set_tile_descriptor(0, in_data_format_unp0, x_dim, y_dim, z_dim);
    set_tile_descriptor(1, in_data_format_unp1, x_dim, y_dim, z_dim);

    // SKIPPED: Set other fields of unpacker config
    set_haloize_mode(transpose_xy_srca_en ? 1 : 0);
    set_tileize_mode(0);
    set_shift_amount(0);

    // Range in X dimension
    SETADCXX(p_setadc::UNP0, (srca_face_height << 4) - 1, 0x0);
    if (row_pool) {
       SETADCXX(p_setadc::UNP1, (1 << 4) - 1, 0x0);
    } else {
       SETADCXX(p_setadc::UNP1, (srcb_face_height << 4) - 1, 0x0);
    } 

    // SKIPPED: Program base address
    
    set_tile_x_dim(256);

    // SKIPPED: unpacker0 needs to keep appending to srcA new faces 
    //     TODO: Understand what this setting is used for
    //         (THCON_SEC0_REG2_Unpack_Src_Reg_Set_Upd)

    // SKIPPED: reset_config_context
}

void Unpack::start_unpack_A(BroadcastType BType, bool acc_to_dest, uint32_t address) {
    // Full implementation must:
    //   - wait for free context (of two contexts)
    //   - post semaphore for context acquire
    //   - program SrcA base address
    // This simulator just programs base address

    // Unpacker is programmed to automatically skip tile header (+1) 
    // Since address points to tile content, we need to -1 address
    // (in terms of 16B words) to offet unpacker's automatic +1.
    set_base_addr(0, 0, address - 1);
}

void Unpack::end_unpack_A() {
    // Full implementation must:
    //   - get semaphore for context release
    //   - switch unpacker config context
    // This simulator has nothing to do
}

void Unpack::start_unpack_AB(uint32_t address_a, uint32_t address_b) {
    // Full implementation must:
    //   - wait for free context (of two contexts)
    //   - program SrcA and SrcB base addresses
    //   - post semaphore for context acquire
    // This simulator just programs base addresses

    // Unpacker is programmed to automatically skip tile header (+1) 
    // Since address_a and address_b point to tile content, we need to -1 address
    // (in terms of 16B words) to offet unpacker's automatic +1.
    set_base_addr(0, 0, address_a - 1);
    set_base_addr(1, 0, address_b - 1);
}

void Unpack::end_unpack_AB() {
    // Full implementation must:
    //   - get semaphore for context release
    //   - switch unpacker config context
    // This simulator has nothing to do
}

void Unpack::start_unpack_A_untilize(uint32_t address) {
    // Full implementation must:
    //   - wait for free context (of only one context)
    //   - post semaphore for context acquire
    //   - program SrcA base address
    // This simulator just programs base address

    // Unpacker is programmed to automatically skip tile header (+1) 
    // Since address points to tile content, we need to -1 address
    // (in terms of 16B words) to offet unpacker's automatic +1.
    set_base_addr(0, 0, address - 1);
}

void Unpack::end_unpack_A_untilize() {
    // Full implementation must:
    //   - get semaphore for context release
    // This simulator has nothing to do
}

void Unpack::mop_run(uint32_t count) {
    MOP(0, count - 1, 0);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

