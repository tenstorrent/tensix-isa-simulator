// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sfpi/sfpi.hpp"

#define sfpi_inline inline

#define BEGIN_SFPI begin_sfpi();
#define END_SFPI end_sfpi();

namespace sfpi = ronin::iss::whb0::sfpi;

namespace ckernel {

void begin_sfpi();
void end_sfpi();

} // namespace ckernel

