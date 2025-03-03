// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Tensix
//

//
//    ATGETM: Acquires mutex with index `mutex_index' for the issuing thread.
//        At most one thread can hold the mutex at any time.
//        Returns immediately if, when the instruction starts, mutex is
//        not held by any thread, or is held by the issuing thread.
//        Otherwise, stalls issuing thread until mutex is acquired.
//        When instruction completes, issuing thread holds the mutex, and it
//        must be released using instruction ATRELM. 
//
//    mutex_index    Mutex index:
//                       0 - math
//                       2 - unpack0
//                       3 - unpack1
//                       4 - pack0
//                       5 - pack1
//                       6 - pack2
//                       7 - pack3 
//

void Tensix::ATGETM(uint32_t mutex_index) {
    // thread must wait until mutex is in released state before calling this method
    assert(m_mutex[mutex_index] == MUTEX_NONE);
    m_mutex[mutex_index] = m_thread_id;
}

//
//    ATRELM: Releases mutex with index `mutex_index' if it is held by issuing thread.
//        Otherwise, has no effect. 
//
//    Arguments: see ATGETM above
//

void Tensix::ATRELM(uint32_t mutex_index) {
    if (m_mutex[mutex_index] == m_thread_id) {
        m_mutex[mutex_index] = MUTEX_NONE;
    }
}

//
//    SEMINIT: Initialize a semaphore
//
//    max_value     Semaphore max value [0..15]
//    init_value    Semaphore initial value [0..15]
//    sem_sel       One-hot encoded semaphore 0..7 select
//

void Tensix::SEMINIT(
        uint32_t max_value,
        uint32_t init_value,
        uint32_t sem_sel) {
    uint32_t sem_index = decode_sem_sel(sem_sel);
    m_sem[sem_index] = init_value;
    m_sem_max[sem_index] = max_value;
}

//
//    SEMPOST: Semaphore increment
//
//    sem_sel    One-hot encoded semaphore 0..7 select
//

void Tensix::SEMPOST(uint32_t sem_sel) {
    uint32_t sem_index = decode_sem_sel(sem_sel);
    incr_sem(sem_index);
}

//
//    SEMGET: Semaphore decrement
//
//    sem_sel    One-hot encoded semaphore 0..7 select
//

void Tensix::SEMGET(uint32_t sem_sel) {
    uint32_t sem_index = decode_sem_sel(sem_sel);
    decr_sem(sem_index);
}

// utility methods

bool Tensix::is_mutex_free(uint32_t mutex_index, uint32_t thread_id) {
    uint32_t value = m_mutex[mutex_index];
    return (value == MUTEX_NONE || value == thread_id);
}

bool Tensix::is_sem_zero(uint32_t sem_index) {
    return (m_sem[sem_index] == 0);
}

bool Tensix::is_sem_max(uint32_t sem_index) {
    return (m_sem[sem_index] == m_sem_max[sem_index]);
}

uint32_t Tensix::read_sem(uint32_t sem_index) {
    return m_sem[sem_index];
}

void Tensix::incr_sem(uint32_t sem_index) {
    if (m_sem[sem_index] < m_sem_max[sem_index]) {
        m_sem[sem_index]++;
    }
}

void Tensix::decr_sem(uint32_t sem_index) {
    if (m_sem[sem_index] > 0) {
        m_sem[sem_index]--;
    }
}

uint32_t Tensix::decode_sem_sel(uint32_t sem_sel) {
    if (sem_sel == 0x01) {
        return 0;
    }
    if (sem_sel == 0x2) {
        return 1;
    }
    if (sem_sel == 0x4) {
        return 2;
    }
    if (sem_sel == 0x8) {
        return 3;
    }
    if (sem_sel == 0x10) {
        return 4;
    }
    if (sem_sel == 0x20) {
        return 5;
    }
    if (sem_sel == 0x40) {
        return 6;
    }
    if (sem_sel == 0x80) {
        return 7;
    }
    assert(false);
    return 0;
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

