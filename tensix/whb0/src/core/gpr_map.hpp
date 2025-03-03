// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Hand-coded parameter encoding for various GPR mappings

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

#include <cstdint>

// Common GPR mapping across all threads
struct p_gpr {
    constexpr static uint32_t
        ZERO         = 0,  // Always stores 0
        DBG_RESERVED = 1,  // Reserved for future use
        DBG_MSG      = 2,  // Firmware debug message
        DBG_CKID     = 3;  // Ckernel ID
};

// Unpack GPR thread
struct p_gpr_unpack {
    constexpr static uint32_t
        OPERAND_BASE_ADDR   = 4,   // Operand base address used by zero buffer function
        OPERAND_OFFSET_ADDR = 5,   // Operand offset address used by zero buffer function
        ZERO_0              = 8,   // Zero data
        ZERO_1              = 9,   // Zero data
        ZERO_2              = 10,  // Zero data
        ZERO_3              = 11,  // Zero data
        TMP0                = 12,  // Temp data
        TMP1                = 13,  // Temp data
        TILE_SIZE           = 14,  // Tile size stored for untilize llk
        TILE_OFFSET         = 15,  // Tile offset address used by untilize llk
        SRCA_TRANSPOSE_XY   = 17;  // unpacker can do src A transpose in wormhole
};

// Math GPR thread
struct p_gpr_math {
    constexpr static uint32_t
        PERF_DBUS_CNTL           = 4,   // Control debug bus perf counter selection
        PERF_MEM_DUMP_CNTL_CLEAR = 5,   // Clear write to memory flag
        PERF_MEM_DUMP_CNTL_SET   = 6,   // Set write to memory flag
        PERF_CNT_START           = 7,   // Start perf counter
        PERF_CNT_STOP            = 8,   // Stop perf counter
        PERF_EPOCH_BASE_ADDR     = 9,   // Perf event ID
        PERF_EPOCH_OFFSET        = 10,  // The offset address for epoch variables
        PERF_BUF_BASE            = 11,  // Base address for perf buffer
        PERF_EVENT_ID            = 12,  // Event id for math thread events
        PERF_EVENT_OFFSET        = 13,  // The offset address for event id
        DEST_OP0_BASE            = 48,  // dest base for sfpu op0
        DEST_OP1_BASE            = 49,  // dest base for sfpu op1
        DEST_REGW_OFFSET         = 50,  // dest rwc base (1st set)
        DEST_REGW_INCR           = 51,  // dest rwc incr (1st set)
        DEST_REGW_OFFSET2        = 52,  // dest rwc base (2nd set)
        DEST_REGW_INCR2          = 53,  // dest rwc incr (2nd set)
        TMP0                     = 60,
        NUM_DRAM_REQS            = 61;
};

// Pack GPR thread
struct p_gpr_pack {
    constexpr static uint32_t 
        DEST_OFFSET_LO    = 4,   // dest lower bank offsets
        DEST_OFFSET_HI    = 8,   // dest upper bank offsets
        OUTPUT_ADDR       = 12,  // output address that packer is writing to
        TILE_HEADER       = 16,  // tile header - ID + tile size
        TEMP_TILE_OFFSET  = 20,  // Temp var which holds tile offset in dest
        NUM_MSGS_RECEIVED = 24,  // holds tile count and word size
        ONE_MSG_RECEIVED  = 25,  // by default holds 1 tile count and word size for streaming per tile
        HEADER_ADDR       = 26,  // Holds the address of the header (used by pack shift kernel only)
        TMP0              = 28,  // Temp data
        TMP1              = 29,  // Temp data
        PACK_STREAM_SYNC  = 32;  // sync between pack and output stream [32:63]
}; 

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

