// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

// Semaphores mapping and trisc space -> tensix space conversion

//
//    MATH_PACK 
//        math <-> pack sync on dest register
//    UNPACK_PACK 
//        pack <-> unpack sync on scratch buffer
//    UNPACK_OPERAND_SYNC 
//        unpack <-> pack, math sync on operand get/release
//    PACK_DONE 
//        Wait for beinning and end of each pack-iteration. 
//        For recording perf events and inserting delay.
//    UNPACK_SYNC 
//        trisc <-> unpack sync on hw kernel
//    UNPACK_MATH_DONE
//        Wait for beinning and end of each unpack or math iteration.
//        For recording perf events and inserting delay.
//        This semaphore should only be used for either unpack or math.
//        Not both at the same time.
//

struct semaphore {
    constexpr static uint32_t MATH_PACK = 1;
    constexpr static uint32_t UNPACK_PACK = 2;
    constexpr static uint32_t UNPACK_OPERAND_SYNC = 3;
    constexpr static uint32_t PACK_DONE = 4;
    constexpr static uint32_t UNPACK_SYNC = 5;
    constexpr static uint32_t UNPACK_MATH_DONE = 6;

    constexpr static uint32_t t6_sem(uint32_t sem_index) {
        return (1 << sem_index);
    }
};

struct mutex {
    // used for atomic register read-modify-write from different threads
    constexpr static uint32_t REG_RMW = 0;
};

constexpr uint32_t PC_BUF_SEMAPHORE_BASE = 8; // base address for semaphores in PC buffer

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

