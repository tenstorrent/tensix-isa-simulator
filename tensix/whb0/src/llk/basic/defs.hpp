#pragma once

#include "core/tensix_types.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

// from "kernels/hostdevcommon/kernel_structs.h"

enum class DstMode {
    Full          = 0,
    Half          = 1,
    Tile          = 2,
    NUM_DST_MODES = 3,
};

// from "common/inc/ckernel_defs.h"

enum {
    SrcA = 0, 
    SrcB = 1, 
    SrcC = 2
};

enum class DstTileFaceLayout {
    RowMajor, // default
    ColMajor
};

enum class DstTileLayout {
    Default
};

enum class DstTileShape {
    Tile32x32 = 0,
    Tile32x16 = 1,
    Tile16x16 = 2
}; 

enum class PackSelMask {
    PACK_ALL = 0xF, // default
    PACK_0 = 0x1,
    PACK_1 = 0x2,
    PACK_2 = 0x4,
    PACK_3 = 0x8,
    PACK_01 = 0x3,
    PACK_23 = 0xC
}; 

#define FACE_HEIGHT 16
#define TILE_HEIGHT 32
#define TILE_HEADER_SIZE 1

constexpr uint32_t DEST_NUM_TILES_FP16 = 
    (core::DEST_REGISTER_FULL_SIZE * core::DEST_FACE_WIDTH) / (TILE_HEIGHT * TILE_HEIGHT);
constexpr uint32_t DEST_NUM_TILES_FP16_HALF = DEST_NUM_TILES_FP16 / 2;
static_assert((DEST_NUM_TILES_FP16 & (DEST_NUM_TILES_FP16 - 1)) == 0); 

// For instructions that address lower/upper 16 bits of a register
#define LO_16(REG) (2 * (REG))
#define HI_16(REG) (2 * (REG) + 1) 

// from "llk_lib/llk_defs.h"

enum class ReduceDim {
    REDUCE_ROW,
    REDUCE_COL,
    REDUCE_SCALAR
};

enum class PoolType {
    SUM,
    AVG,
    MAX
};

enum class DataCopyType {
    A2D,
    B2D
}; 

enum class EltwiseBinaryType {
    ELWMUL,
    ELWDIV,
    ELWADD,
    ELWSUB,
    ELWLESS
};

enum class DstSync {
    SyncHalf   = 0,
    SyncFull   = 1,
    SyncTile16 = 2,
    SyncTile2  = 3
};

enum class BroadcastType {
    NONE   = 0x0,  // A - None || B - None
    COL    = 0x1,  // A - None || B - Col Broadcast
    ROW    = 0x2,  // A - None || B - Row Broadcast
    SCALAR = 0x3   // A - None || B - Scalar Broadcast
};

enum class ReluType {
    NO_RELU,
    ZERO_RELU,
    MIN_THRESHOLD_RELU,
    MAX_THRESHOLD_RELU
}; 

enum class SfpuType {
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
    multiply,
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
    square,
    max,
    sine,
    cosine,
    relu_min,
    relu_max,
    unused
};

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

