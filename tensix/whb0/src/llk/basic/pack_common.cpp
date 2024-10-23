
#include <cstdint>
#include <cassert>

#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/pack.hpp"

//
//    ACHTUNG: Conceptually concurrent actions of MATH and PACK are serialized
//        by simulator. This simplified implementation does not model flipping
//        of Dest register banks.
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_setadc;
using core::p_stall;
using core::p_zeroacc;

//
//    Pack
//

// llk

void Pack::packer_wait_for_math_done() {
    wait_sem_math_pack(p_stall::STALL_ON_ZERO);
}

void Pack::pack_dest_section_done(DstSync Dst, bool is_fp32_dest_acc_en) {
    bool clear_dest = (Dst != DstSync::SyncTile16);

    if (clear_dest) {
        // SKIPPED: STALLWAIT(p_stall::STALL_MATH, p_stall::PACK)

        if (Dst == DstSync::SyncFull) {
            uint32_t CLEAR_MODE = 
                is_fp32_dest_acc_en ? p_zeroacc::CLR_ALL_32B : p_zeroacc::CLR_ALL;
            ZEROACC(CLEAR_MODE, 1, 0);
        } else {
            assert(Dst == DstSync::SyncHalf || Dst == DstSync::SyncTile2);
            uint32_t CLEAR_MODE = 
                is_fp32_dest_acc_en ? p_zeroacc::CLR_HALF_32B : p_zeroacc::CLR_HALF;
            // currently no 'dest_offset_id' flipping
            ZEROACC(CLEAR_MODE, 1, /*dest_offset_id % 2*/ 0);
        }
    }
 
    // Tell math that it can write again
    get_sem_math_pack();

    // SKIPPED: flip_dest support
}

void Pack::pack_dest_init(
        DstSync Dst, 
        DstTileFaceLayout FaceLayout, 
        bool untilize, 
        bool is_fp32_dest_acc_en) {
    // SKIPPED: tensix_sync
    // SKIPPED: reset_dest_offset_id
    init_packer_dest_offset_registers(Dst, FaceLayout, untilize);
    packer_addr_counter_init();
    m_pack_sync_tile_dst_ptr = 0; 
}

void Pack::init_packer_dest_offset_registers(
        DstSync Dst, 
        DstTileFaceLayout FaceLayout, 
        bool untilize) {
    // configure Dest offsets registers to specify mapping of Dest rows to packers

    // ACHTUNG: Original code contained two separate functions,
    //     'llk_init_packer_dest_offset_registers' and 'init_packer_dest_offset_registers'
    //     that have similar functionality both setting DEST_TARGET_REG_CFG_PACK_SEC[0-3]_Offset
    //     and 'llk_' version call STALLWAIT at start. For emulator, there is no difference
    //     and both functions are emulated using this common method.

    // specifying offset for each packer allows shuffling Dest rows while packing
    // each packer sequentially selects and packs 16 Dest rows
    // sequences of row indices are listed below
    // see comments in "pack.cpp" for explanation of these sequences
    if (untilize) {
        if (FaceLayout == DstTileFaceLayout::ColMajor) {
            // Packer0 :  0, 32,  1, 33 ...  7, 39
            // Packer1 :  8, 40,  9, 41 ... 15, 47
            // Packer2 : 16, 48, 17, 49 ... 23, 55
            // Packer3 : 24, 56, 25, 57 ... 31, 63
            set_pack_dest_offset(0x00, 0x08, 0x10, 0x18);
        } else {
            // Packer0 :  0, 16,  1, 17 ...  7, 23
            // Packer1 :  8, 24,  9, 25 ... 15, 31
            // Packer2 : 32, 48, 33, 49 ... 39, 55
            // Packer3 : 40, 56, 41, 57 ... 47, 63
            set_pack_dest_offset(0x00, 0x08, 0x20, 0x28);
        }
    } else {
        if (FaceLayout == DstTileFaceLayout::ColMajor) {
            set_pack_dest_offset(0x00, 0x20, 0x10, 0x30);
        } else {
            set_pack_dest_offset(0x00, 0x10, 0x20, 0x30);
        }
    }
}

void Pack::pack_set_relu_config(uint32_t config) {
    uint32_t f_mode = config & 0xf;
    ReluType mode = 
        (f_mode == 0) ? ReluType::NO_RELU : 
        (f_mode == 3) ? ReluType::MAX_THRESHOLD_RELU : 
            ReluType::MIN_THRESHOLD_RELU;
    uint32_t threshold = config >> 16;
    set_relu_config(uint32_t(mode), threshold);
}

// ckernels

void Pack::packer_addr_counter_init() {
   // Set unpacker offsets to 0, except for
   // unpacker 0, channel 1, X, which is the tile X dimension 
   SETADCXY(0b100, 0, 0, 0, 0, 0b1011);
   SETADCZW(0b100, 0, 0, 0, 0, 0b1111); 
}

#if 0 // TODO: Revise this
void Pack::configure_pack(
        bool is_fp32_dest_acc_en,
        DataFormat src_format,
        DataFormat dst_format, 
        ReluType relu_type, 
        uint32_t relu_threshold) {
    // SKIPPED: STALLWAIT and tensix_sync on pack_dst_format change

    set_pack_fp32_dest(is_fp32_dest_acc_en);

    uint32_t pack_per_xy_plane = 16;

    // SKIPPED: Set souce address (Ch0) strides (dim info integrated into set_pack_config)
    // SKIPPED: Set ALU dst format

    // Set packer config
    uint32_t x_dim = 16;               // x = one row
    uint32_t y_dim = PACK_CNT * 16;    // xy = tile = PACK_CNT faces
    uint32_t z_dim = 1;
    set_pack_config(src_format, dst_format, x_dim, y_dim, z_dim);

    // SKIPPED: Disable auto last generation

    set_pack_edge_offset_mask(0, 0xffff); 
    set_pack_edge_row_set_select(0, 0);
    set_pack_edge_row_set_select(1, 0);
    set_pack_edge_row_set_select(2, 0);
    set_pack_edge_row_set_select(3, 0);
    set_tile_row_set_mapping(0, 0x0);

    // Emulator can derive these values from 'dst_format':
    // SKIPPED: Set THCON_SEC[0|2]_REG[1|8]_L1_Dest_addr
    // SKIPPED: Set p_gpr_pack::TILE_HEADER

    // Config RELU
    set_relu_config(uint32_t(relu_type), relu_threshold);

    // Assume face height 16
    SETADCXX(p_setadc::PAC, (256 / pack_per_xy_plane) - 1, 0x0);

    // SKIPPED: Store value for num_msg_received register when tile count is 1
}
#endif

// Upgraded to 0.33 (pack by 256-item rows unless untilize)

void Pack::configure_pack(
        bool is_fp32_dest_acc_en,
        bool untilize,
        DataFormat src_format,
        DataFormat dst_format, 
        ReluType relu_type, 
        uint32_t relu_threshold) {
    // SKIPPED: STALLWAIT and tensix_sync on pack_dst_format change

    set_pack_fp32_dest(is_fp32_dest_acc_en);

    // SKIPPED: Set souce address (Ch0) strides (dim info integrated into set_pack_config)
    // SKIPPED: Set ALU dst format

    // Set packer config
    uint32_t x_dim = 16;               // x = one row
    uint32_t y_dim = PACK_CNT * 16;    // xy = tile = PACK_CNT faces
    uint32_t z_dim = 1;
    set_pack_config(src_format, dst_format, x_dim, y_dim, z_dim);

    // SKIPPED: Disable auto last generation

    set_pack_edge_offset_mask(0, 0xffff); 
    set_pack_edge_row_set_select(0, 0);
    set_pack_edge_row_set_select(1, 0);
    set_pack_edge_row_set_select(2, 0);
    set_pack_edge_row_set_select(3, 0);
    set_tile_row_set_mapping(0, 0x0);

    // SKIPPED: Set p_gpr_pack::TILE_HEADER

    // Config RELU
    set_relu_config(uint32_t(relu_type), relu_threshold);

    // Assume face height 16
    uint32_t pack_x_dim = untilize ? 16 : 256;
    SETADCXX(p_setadc::PAC, pack_x_dim - 1, 0x0);
}

void Pack::program_packer_destination(PackSelMask PackSel, uint32_t addr) {
    // PackSel is not used
    set_pack_dest_addr(addr);
}

void Pack::program_packer_l1_acc(bool pack_l1_acc) {
    set_pack_l1_acc(pack_l1_acc);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

