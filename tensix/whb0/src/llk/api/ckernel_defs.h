// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "core/tensix_types.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

using core::DataFormat;

using core::DEST_FACE_WIDTH;
using core::DEST_REGISTER_FULL_SIZE;
using core::DEST_REGISTER_HALF_SIZE;

enum Srcs { 
    SrcA = 0, 
    SrcB = 1, 
    SrcC = 2 
};

enum Unpackers { 
    Unp0 = 0, 
    Unp1 = 1, 
    UnpAll = 2 
};

enum DstStart { 
    StartZero = 0, 
    StartHalf = 1
};

enum DstClear { 
    ClearRow = 0, 
    Clear16Rows = 1, 
    ClearHalf = 2, 
    ClearFull = 3 
};

enum ThreadId { 
    UnpackThreadId = 0, 
    MathThreadId = 1, 
    PackThreadId = 2
};

enum DstSync {
    SyncHalf = 0,
    SyncFull = 1,
    SyncTile16 = 2,
    SyncTile2 = 3,
};

enum DstTileLayout {
    Default,
    Interleaved,
    // TightDest,
    // Conv3x3,
    // Conv1x1,
    // L1ReadSource,
    // NLLLoss,
    // IndexAccumulate, //Add polling before packing to L1
};

enum DstTileFaceLayout {
    RowMajor, // default
    ColMajor,
};

enum DstTileShape {
    Tile32x32 = 0,
    Tile32x16 = 1,
    Tile16x16 = 2
};

enum class ParallelPackerMode {
    Disabled,
    SingleFTEntry,
    MultiFTEntry,
    TileParallel
};

enum register_space_e {
    TDMA_REGS = 0x0,
    LOCAL_REGS = 0x1,
    ADDR_COUNTERS = 0x2
};

enum PackSelMask {
    PACK_ALL = 0xF, // default
    PACK_0 = 0x1,
    PACK_1 = 0x2,
    PACK_2 = 0x4,
    PACK_3 = 0x8,
    PACK_01 = 0x3,
    PACK_23 = 0xC
};

// moved from llk_defs.h
enum TileDim {
    R_IDX = 0,
    C_IDX = 1,
};

// moved from llk_defs.h
enum SfpuType {
    tanh,
    hardtanh,
    gelu,
    exponential,
    exp_with_base,
    sigmoid,
    reciprocal,
    sqrt,
    lrelu,
    power,
    square,
    tanh_derivative,
    log,
    log_with_base,
    equal_zero,
    not_equal_zero,
    less_than_zero,
    greater_than_equal_zero,
    less_than_equal_zero,
    greater_than_zero,
    clamp,
    gelu_derivative,
    dropout,
    abs,
    sign,
    max,
    sine,
    cosine,
    tan,
    relu_max,
    relu_min,
    cast_fp32_to_fp16a,
    sigmoid_appx,
    gelu_appx,
    elu,
    min,
    exp2,
    heaviside,
    expm1,
    signbit,
    asin,
    acos,
    atan,
    erf,
    erfc,
    rsqrt,
    isfinite,
    isinf,
    isposinf,
    isneginf,
    isnan,
    logical_not_unary,
    erfinv,
    i0,
    unused,
};

constexpr uint32_t FACE_HEIGHT = 16;
constexpr uint32_t FACE_WIDTH  = 16;
constexpr uint32_t TILE_HEIGHT = 32;
constexpr uint32_t TILE_WIDTH  = 32;
constexpr uint32_t DATUMS_PER_ROW = 16;
constexpr uint32_t TILE_HEADER_SIZE = 0;

constexpr uint32_t FACE_R_DIM = FACE_HEIGHT;
constexpr uint32_t FACE_C_DIM = FACE_WIDTH ;

constexpr uint32_t TILE_R_DIM = TILE_HEIGHT;
constexpr uint32_t TILE_C_DIM = TILE_WIDTH ;

constexpr uint32_t TILE_NUM_FACES = (TILE_R_DIM * TILE_C_DIM) / (FACE_R_DIM * FACE_C_DIM);

constexpr uint32_t DEST_NUM_TILES_FP16 = 
    (DEST_REGISTER_FULL_SIZE * DEST_FACE_WIDTH) / (TILE_HEIGHT * TILE_HEIGHT);
constexpr uint32_t DEST_NUM_TILES_FP16_HALF = DEST_NUM_TILES_FP16 / 2;
static_assert((DEST_NUM_TILES_FP16 & (DEST_NUM_TILES_FP16 - 1)) == 0);

// For instructions that address lower/upper 16 bits of a register
#define LO_16(REG) (2 * (REG))
#define HI_16(REG) (2 * (REG) + 1)

// all sizes are headerless
// in 16B words, in L1/DRAM, headerless
constexpr static uint32_t MUL_TILE_SIZE_AND_INDEX(uint32_t format, uint32_t index) {
    switch (format & 0x1F) {
    case (uint32_t)DataFormat::Float32: 
        return index << 8;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return index << 7;
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b: 
        return (index << 6) + (index << 2);
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b: 
        return (index << 5) + (index << 2);
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return (index << 4) + (index << 2);
    case (uint32_t)DataFormat::Int8:
    case (uint32_t)DataFormat::Lf8: 
        return index << 6;
    default: 
        // Keep default as Bfp8?
        return (index << 6) + (index << 2);
    }
}

// in Bytes, in DST REG, headerless
constexpr static uint32_t MUL_DEST_TILE_SIZE_AND_INDEX(uint32_t format, uint32_t index) {
    switch (format & 0x1F) {
    case (uint32_t)DataFormat::Float32: 
        return index << 12;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return index << 11;
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b: 
        return index << 10;
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b: 
        return index << 9;
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return index << 8;
    case (uint32_t)DataFormat::Int8:
    case (uint32_t)DataFormat::Lf8: 
        return index << 10;
    default: 
        return index << 10;
    }
}

// 16B words, L1/dram headerless!
constexpr static uint32_t GET_L1_TILE_SIZE(uint32_t format) {
    switch (format & 0x1F) {
    case (uint32_t)DataFormat::Float32: 
        return 4096 >> 4;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return 2048 >> 4;
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b: 
        return (1024 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b: 
        return (512 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return (256 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Int8:
    case (uint32_t)DataFormat::Lf8: 
        return 1024 >> 4;
    default: 
        return (1024 >> 4) + (64 >> 4);
    }
}

constexpr static uint32_t GET_DEST_TILE_BYTE_SIZE(uint32_t format) {
    switch (format & 0x1F) {
    case (uint32_t)DataFormat::Float32: 
        return 4096;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return 2048;
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b: 
        return 1024;
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b: 
        return 512;
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return 256;
    case (uint32_t)DataFormat::Int8:
    case (uint32_t)DataFormat::Lf8: 
        return 1024;
    default: 
        return 1024;
    }
}

constexpr static uint32_t GET_L1_HEADERLESS_TILE_SIZE(uint32_t format) {
    switch (format & 0xF) {
    case (uint32_t)DataFormat::Float32: 
        return 4096 >> 4;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return 2048 >> 4;
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b: 
        return (1024 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b: 
        return (512 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return (256 >> 4) + (64 >> 4);
    case (uint32_t)DataFormat::Int8:
    case (uint32_t)DataFormat::Lf8: 
        return 1024 >> 4;
    default: 
        return (1024 >> 4) + (64 >> 4);
    }
}

constexpr static bool IS_BFP_FORMAT(uint32_t format) {
    switch (format & 0xF) {
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp8_b:
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp4_b:
    case (uint32_t)DataFormat::Bfp2:
    case (uint32_t)DataFormat::Bfp2_b: 
        return true;
    default: 
        return false;
    }
}

constexpr static bool IS_BFP_A_FORMAT(uint32_t format) {
    switch (format & 0xF) {
    case (uint32_t)DataFormat::Bfp8:
    case (uint32_t)DataFormat::Bfp4:
    case (uint32_t)DataFormat::Bfp2: 
        return true;
    default: 
        return false;
    }
}

constexpr static uint32_t SCALE_DATUM_SIZE(uint32_t format, uint32_t datum_count) {
    switch (format & 0xF) {
    case (uint32_t)DataFormat::Float32: 
        return datum_count << 2;
    case (uint32_t)DataFormat::Float16:
    case (uint32_t)DataFormat::Float16_b: 
        return datum_count << 1;
    default: 
        return datum_count;
    }
}

#define LOWER_HALFWORD(x) ((x) & 0xFFFF)
#define UPPER_HALFWORD(x) ((x) >> 16)

constexpr int WHB0_ITERATIONS = 8;

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

