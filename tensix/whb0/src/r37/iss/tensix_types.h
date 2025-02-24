// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

//
//    Reuse core version to avoid redefinitions
//

#include "core/tensix_types.hpp"

using ronin::iss::whb0::core::DEST_FACE_WIDTH;
using ronin::iss::whb0::core::DEST_FACE_WIDTH_LOG2;
using ronin::iss::whb0::core::DEST_FACE_HEIGHT;
using ronin::iss::whb0::core::DEST_FACE_HEIGHT_LOG2;
using ronin::iss::whb0::core::DEST_REGISTER_FULL_SIZE;
using ronin::iss::whb0::core::DEST_REGISTER_FULL_SIZE_LOG2;
using ronin::iss::whb0::core::DEST_REGISTER_HALF_SIZE;
using ronin::iss::whb0::core::BIT32_DEST_REGISTER_HALF_SIZE;

using ronin::iss::whb0::core::DEST_REGISTER_FULL_SIZE_BYTES;
using ronin::iss::whb0::core::DEST_REGISTER_HALF_SIZE_BYTES;

using ronin::iss::whb0::core::DataFormat;

//
//    Define other constants here
//

// Voluntary FIFO alignment so that we can pack fifo address down to 16 bits in the command.
// At 8, we can cover 16MB. The upper limit is 15 because the command queue is at 32K (by default)
// Even though the command queue address never goes into a command, assertions complain if it is misaligned.
// (Hardware only requires 16B alignment.)
static constexpr uint32_t FIFO_BASE_ADDRESS_ALIGN_BITS = 9;
static constexpr uint32_t FIFO_BASE_ADDRESS_ALIGN = 1 << FIFO_BASE_ADDRESS_ALIGN_BITS;

