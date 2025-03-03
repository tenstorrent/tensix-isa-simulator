// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "system/cfg_defines.h"

#include "ckernel_ops.h"
#include "ckernel_thread.h"
#include "ckernel_structs.h"
#include "ckernel_mem_ptr.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

namespace {

constexpr uint32_t NULL_ADDR = ~uint32_t(0);

inline bool is_l1_addr(uint32_t addr) {
    return (addr < 0x180000);
}

inline bool is_dma_reg_addr(uint32_t addr) {
    return (addr >= REGFILE_BASE && addr < REGFILE_BASE + 4 * 64);
}

inline uint32_t map_dma_reg_addr(uint32_t addr) {
    return (addr - REGFILE_BASE) / 4;
}

inline bool is_sem_addr(uint32_t addr) {
    constexpr uint32_t SEM_BASE = PC_BUF_BASE + 4 * PC_BUF_SEMAPHORE_BASE;
    return (addr >= SEM_BASE && addr < SEM_BASE + 4 * 8);
}

inline uint32_t map_sem_addr(uint32_t addr) {
    constexpr uint32_t SEM_BASE = PC_BUF_BASE + 4 * PC_BUF_SEMAPHORE_BASE;
    return (addr - SEM_BASE) / 4;
}

inline bool is_cfg_reg_addr(uint32_t addr) {
    return (addr >= TENSIX_CFG_BASE && addr < TENSIX_CFG_BASE + 4 * 4 * CFG_STATE_SIZE);
}

inline uint32_t map_cfg_reg_addr(uint32_t addr) {
    return (addr - TENSIX_CFG_BASE) / 4;
}

} // namespace

//
//    MemRef
//

MemRef::MemRef():
        m_addr(NULL_ADDR) { }

MemRef::MemRef(const MemRef &other):
        m_addr(other.m_addr) { }

MemRef::~MemRef() { }

MemRef MemRef::map(uint32_t addr) {
    MemRef ref;
    ref.m_addr = addr;
    return ref;
}

MemRef &MemRef::operator=(const MemRef &other) {
    m_addr = other.m_addr;
    return *this;
}

MemRef &MemRef::operator=(uint32_t value) {
    assert(m_addr != NULL_ADDR);
    if (is_l1_addr(m_addr)) {
        // regular L1
        uint32_t *ptr = get_thread()->map_l1_addr(m_addr);
        *ptr = value;
    } else if (is_dma_reg_addr(m_addr)) {
        // DMA register
        uint32_t reg = map_dma_reg_addr(m_addr);
        get_thread()->write_dma_reg(reg, value);
    } else if (is_sem_addr(m_addr)) {
        // semaphore
        uint32_t sem = map_sem_addr(m_addr);
        // no direct write: 'value' is interpreted as command
        if (value == 0) {
            get_thread()->incr_sem(sem);
        } else {
            get_thread()->decr_sem(sem);
        }
    } else if (is_cfg_reg_addr(m_addr)) {
        // config register
        // TODO: Implement support for non-0 config state
        uint32_t reg = map_cfg_reg_addr(m_addr);
        get_thread()->write_cfg_reg(reg, value);
    } else {
        assert(false);
    }
    return *this;
}

MemRef::operator uint32_t() {
    assert(m_addr != NULL_ADDR);
    if (is_l1_addr(m_addr)) {
        // regular L1
        uint32_t *ptr = get_thread()->map_l1_addr(m_addr);
        return *ptr;
    }
    if (is_dma_reg_addr(m_addr)) {
        // DMA register
        uint32_t reg = map_dma_reg_addr(m_addr);
        return get_thread()->read_dma_reg(reg);
    }
    if (is_sem_addr(m_addr)) {
        // semaphore
        uint32_t sem = map_sem_addr(m_addr);
        return get_thread()->read_sem(sem);
    }
    if (is_cfg_reg_addr(m_addr)) {
        // config register
        // TODO: Implement support for non-0 config state
        uint32_t reg = map_cfg_reg_addr(m_addr);
        return get_thread()->read_cfg_reg(reg);
    }
    assert(false);
    return 0;
}

//
//    MemPtr
//

MemPtr::MemPtr():
        m_addr(NULL_ADDR) { }

MemPtr::MemPtr(const MemPtr &other):
        m_addr(other.m_addr) { }

MemPtr::~MemPtr() { }

MemPtr MemPtr::map(uint32_t addr) {
    MemPtr ptr;
    ptr.m_addr = addr;
    return ptr;
}

MemPtr &MemPtr::operator=(const MemPtr &other) {
    m_addr = other.m_addr;
    return *this;
}

MemRef MemPtr::operator[](int index) {
    assert(m_addr != NULL_ADDR);
    return MemRef::map(m_addr + 4 * index);
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

