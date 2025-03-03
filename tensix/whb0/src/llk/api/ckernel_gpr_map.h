// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

// Hand-coded parameter encoding for various GPR mappings

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

// Common GPR mapping across all threads
struct p_gpr {
    constexpr static uint32_t ZERO     = 0;      // Always stores 0
    constexpr static uint32_t DBG_RESERVED = 1;  // Reserved for future use
    constexpr static uint32_t DBG_MSG =  2;      // Firmware debug message
    constexpr static uint32_t DBG_CKID = 3;      // Ckernel ID
};

// Unpack GPR thread
struct p_gpr_unpack {
    constexpr static uint32_t OPERAND_BASE_ADDR = 4;    // Operand base address used by zero buffer function
    constexpr static uint32_t OPERAND_OFFSET_ADDR = 5;  // Operand offset address used by zero buffer function
    constexpr static uint32_t ZERO_0            = 8;    // Zero data
    constexpr static uint32_t ZERO_1            = 9;    // Zero data
    constexpr static uint32_t ZERO_2            = 10;   // Zero data
    constexpr static uint32_t ZERO_3            = 11;   // Zero data
    constexpr static uint32_t TMP0              = 12;   // Temp data
    constexpr static uint32_t TMP1              = 13;   // Temp data
    constexpr static uint32_t TILE_SIZE         = 14;   // Tile size
    constexpr static uint32_t TILE_OFFSET       = 15;   // Tile offset
    constexpr static uint32_t L1_BUFFER_ADDR    = 17;   // Holds address of fixed l1 buffer used for reduce in1
    constexpr static uint32_t TMP_LO            = 18;   // Temp data. Upper 16-bits always 0
    constexpr static uint32_t TMP_HI            = 19;   // Temp data. Lower 16-bits always 0
    constexpr static uint32_t PERF_FIRST_UNP_LO = 32;   // timestamp for first-unpack-instruction (low 32b)
    constexpr static uint32_t PERF_FIRST_UNP_HI = 33;   // timestamp for first-unpack-instruction (high 32b)
    constexpr static uint32_t TILE_SIZE_A       = 36;   // Holds tile size for unpacker 0
    constexpr static uint32_t TILE_SIZE_B       = 37;   // Holds tile size for unpacker 1
    constexpr static uint32_t KT_DIM            = 38;   // Holds matmul kt_dim
    constexpr static uint32_t FACE_DIM_16x16    = 40;   // Holds face dimension (16x16)
    constexpr static uint32_t FACE_DIM_8x16     = 41;   // Holds face dimension (8x16)
    constexpr static uint32_t FACE_DIM_4x16     = 42;   // Holds face dimension (4x16)
    constexpr static uint32_t FACE_DIM_2x16     = 43;   // Holds face dimension (2x16)
    constexpr static uint32_t FACE_DIM_1x16     = 44;   // Holds face dimension (1x16)
    constexpr static uint32_t PERF_UNPACK_NUM_TILES_0 = 45;  // num tiles for input operands 0-1
    constexpr static uint32_t PERF_UNPACK_NUM_TILES_1 = 46;  // num tiles for input operands 2-3
    constexpr static uint32_t PERF_UNPACK_NUM_TILES_2 = 47;  // num tiles for input operands 4-5
    constexpr static uint32_t PERF_UNPACK_NUM_TILES_3 = 48;  // num tiles for input operands 6-7
};

// Math GPR thread
struct p_gpr_math {
    constexpr static uint32_t PERF_DBUS_CNTL    = 4;    // Control debug bus perf counter selection
    constexpr static uint32_t PERF_MEM_DUMP_CNTL_CLEAR= 5;  // Clear write to memory flag
    constexpr static uint32_t PERF_MEM_DUMP_CNTL_SET= 6;    // Set write to memory flag
    constexpr static uint32_t PERF_CNT_START    = 7;    // Start perf counter
    constexpr static uint32_t PERF_CNT_STOP     = 8;    // Stop perf counter
    constexpr static uint32_t PERF_EPOCH_BASE_ADDR= 9;  // Perf event ID
    constexpr static uint32_t PERF_EPOCH_OFFSET = 10;   // The offset address for epoch variables
    constexpr static uint32_t DEST_OP0_BASE     = 48;   // dest base for sfpu op0
    constexpr static uint32_t DEST_OP1_BASE     = 49;   // dest base for sfpu op1
    constexpr static uint32_t DEST_REGW_OFFSET  = 50;   // dest rwc base (1st set)
    constexpr static uint32_t DEST_REGW_INCR    = 51;   // dest rwc incr (1st set)
    constexpr static uint32_t DEST_REGW_OFFSET2 = 52;   // dest rwc base (2nd set)
    constexpr static uint32_t DEST_REGW_INCR2   = 53;   // dest rwc incr (2nd set)
    constexpr static uint32_t TMP0              = 60;
    constexpr static uint32_t NUM_DRAM_REQS     = 61;
};

// Pack GPR thread
struct p_gpr_pack {
    constexpr static uint32_t DEST_OFFSET_LO = 4;   // dest lower bank offsets
    constexpr static uint32_t DEST_OFFSET_HI = 8;   // dest upper bank offsets
    constexpr static uint32_t OUTPUT_ADDR    = 12;  // output address that packer is writing to
    constexpr static uint32_t TILE_HEADER    = 16;  // tile header - ID + tile size

    constexpr static uint32_t TEMP_TILE_OFFSET  = 20;  // Temp var which holds tile offset in dest
    constexpr static uint32_t NUM_MSGS_RECEIVED = 24;  // holds tile count and word size
    constexpr static uint32_t ONE_MSG_RECEIVED  = 25;  // by default holds 1 tile count and word size for streaming per tile
    constexpr static uint32_t HEADER_ADDR       = 26;  // Holds the address of the header (used by pack shift kernel only)
    constexpr static uint32_t TMP0              = 28;  // Temp data
    constexpr static uint32_t TMP1              = 29;  // Temp data
    constexpr static uint32_t TMP_LO            = 30;  // Temp data, upper 16-bit always 0
    constexpr static uint32_t TMP_HI            = 31;  // Temp data, lower 16-bit always 0
    constexpr static uint32_t PACK_STREAM_SYNC  = 32;  // sync between pack and output stream [32:63]
    constexpr static uint32_t PERF_PACK_NUM_TILES = 51;  // output operand num tiles
    constexpr static uint32_t EXP0_SEC_SIZE_BFP   = 52;  // pack0,1,2,3 exp section size for bfp8,4,2
    constexpr static uint32_t EXP1_SEC_SIZE_BFP8  = 53;  // pack1 exp section size for bfp8
    constexpr static uint32_t EXP2_SEC_SIZE_BFP8  = 54;  // pack2 exp section size for bfp8
    constexpr static uint32_t EXP3_SEC_SIZE_BFP8  = 55;  // pack2 exp section size for bfp8
    constexpr static uint32_t EXP1_SEC_SIZE_BFP4  = 57;  // pack1 exp section size for bfp4
    constexpr static uint32_t EXP2_SEC_SIZE_BFP4  = 58;  // pack2 exp section size for bfp4
    constexpr static uint32_t EXP3_SEC_SIZE_BFP4  = 59;  // pack3 exp section size for bfp4
    constexpr static uint32_t EXP1_SEC_SIZE_BFP2  = 61;  // pack1 exp section size for bfp2
    constexpr static uint32_t EXP2_SEC_SIZE_BFP2  = 62;  // pack2 exp section size for bfp2
    constexpr static uint32_t EXP3_SEC_SIZE_BFP2  = 63;  // pack3 exp section size for bfp2
};

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

