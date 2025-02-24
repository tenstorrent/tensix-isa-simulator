// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

static constexpr uint32_t DEST_FACE_WIDTH = 16;
static constexpr uint32_t DEST_FACE_WIDTH_LOG2 = 4;
static constexpr uint32_t DEST_FACE_HEIGHT = 16;
static constexpr uint32_t DEST_FACE_HEIGHT_LOG2 = 4;
static constexpr uint32_t DEST_REGISTER_FULL_SIZE = 64 * DEST_FACE_HEIGHT;
static constexpr uint32_t DEST_REGISTER_FULL_SIZE_LOG2 = 12;
static constexpr uint32_t DEST_REGISTER_HALF_SIZE = DEST_REGISTER_FULL_SIZE / 2;
static constexpr uint32_t BIT32_DEST_REGISTER_HALF_SIZE = DEST_REGISTER_HALF_SIZE / 2;

static constexpr uint32_t DEST_REGISTER_FULL_SIZE_BYTES = DEST_REGISTER_FULL_SIZE * 2 * 16;
static constexpr uint32_t DEST_REGISTER_HALF_SIZE_BYTES = DEST_REGISTER_FULL_SIZE_BYTES / 2;

enum class DataFormat {
    Float32   = 0,
    Float16   = 1,
    Bfp8      = 2,
    Bfp4      = 3,
    Bfp2      = 11,
    Float16_b = 5,
    Bfp8_b    = 6,
    Bfp4_b    = 7,
    Bfp2_b    = 15,
    Lf8       = 10,
    UInt16    = 12,
    Int8      = 14,
    UInt8     = 30,
    Int32     = 8,
    Int16     = 9,
    Tf32      = 4,
    testMan7  = 0x82,       // intermediate format for testing: 7bit mantissa (6+hidden)
    testMan2  = 0x8A,       // intermediate format for testing: 2bit mantissa (2+hidden)
    Invalid   = 0xff
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

