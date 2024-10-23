
#include <cstdint>
#include <cstring>
#include <cassert>

#include "core/memory.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    BasicMemory
//

BasicMemory::BasicMemory(uint32_t size):
        m_size(size),
        m_data(size, 0) { }

BasicMemory::~BasicMemory() { }

void BasicMemory::reset() {
    memset(m_data.data(), 0, m_data.size());
}

uint32_t BasicMemory::size() {
    return m_size;
}

uint8_t *BasicMemory::map_addr(uint32_t addr) {
    assert(addr < m_size);
    return m_data.data() + addr;
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

