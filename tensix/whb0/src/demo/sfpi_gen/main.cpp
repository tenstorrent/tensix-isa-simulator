// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <exception>

#include "sfpi/sfpi.hpp"

#include "demo/sfpi_gen/sfpi_test.hpp"

int main() {
    try {
        sfpi_test::calculate_sfpi(sfpi_test::SfpiTestType::test17, 35);
    } catch (std::exception &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
    return 0;
}

