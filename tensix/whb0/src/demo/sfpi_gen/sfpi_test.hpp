// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace sfpi_test {

enum class SfpiTestType {
    test1,
    test2,
    test3,
    test4,
    test5,
    test6,
    test7,
    test8,
    test9,
    test10,
    test11,
    test12,
    test13,
    test14,
    test15,
    test16,
    test17
};

void calculate_sfpi(
   SfpiTestType operation,
   uint32_t param0 = 0, 
   uint32_t param1 = 0, 
   uint32_t param2 = 0, 
   uint32_t param3 = 0, 
   uint32_t param4 = 0, 
   uint32_t param5 = 0);

} // namespace sfpi_test

