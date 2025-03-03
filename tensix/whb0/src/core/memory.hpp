// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <vector>

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

class Memory {
public:
    Memory() { }
    virtual ~Memory() { }
public:
    virtual uint32_t size() = 0;
    virtual uint8_t *map_addr(uint32_t addr) = 0;
};

class BasicMemory: public Memory {
public:
    BasicMemory(uint32_t size);
    ~BasicMemory();
public:
    void reset();
    uint32_t size() override;
    uint8_t *map_addr(uint32_t addr) override;
private:
    uint32_t m_size;
    std::vector<uint8_t> m_data;
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespac ronin

