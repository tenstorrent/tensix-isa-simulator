
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
//    SETC16: Sets thread specific control register to the 16-bit value stored in the slot argument
//
//    setc16_reg      Register index
//    setc16_value    The value to store 
//

void Tensix::SETC16(uint32_t setc16_reg, uint32_t setc16_value) {
    setc16(setc16_reg, setc16_value);
}

//
//    WRCFG: Write configuration register of current thread's state
//        with 32b data read from Gpr file
//
//    gpr_address    gpr address to read data from (32-bit aligned)
//    wr128b         do 128-bit Reg to Cfg write if set to 1
//    cfg_reg        configuration register address to write data to (32-bit aligned)
//

void Tensix::WRCFG(
        uint32_t gpr_address, 
        uint32_t wr128b, 
        uint32_t cfg_reg) {
    uint32_t *ptr = &m_dma_reg[m_thread_id][gpr_address];
    if (wr128b != 0) {
        wrcfg128(cfg_reg, ptr);
    } else {
        wrcfg32(cfg_reg, *ptr);
    }
}

//
//    RDCFG: Read configuration register of current thread's state and write to its from Gpr file
//
//    gpr_address    gpr address to write config data to (32-bit aligned)
//    cfg_reg        configuration register address to read data from (32-bit aligned)
//

void Tensix::RDCFG(uint32_t gpr_address, uint32_t cfg_reg) {
    assert(cfg_reg < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    m_dma_reg[m_thread_id][gpr_address] = m_cfg_reg[cfg_state_id][cfg_reg];
}

//
//    RMWCIB0: Read-Modify-Write on Byte 0 of the cfg register, only updating the bits set in mask
//
//    mask            Mask to use for data (8-bits) 
//    data            Data to write (8-bits)
//    cfg_reg_addr    Address of CFG register in active state space
//

void Tensix::RMWCIB0(
        uint32_t mask, 
        uint32_t data, 
        uint32_t cfg_reg_addr) {
    rmwcib(cfg_reg_addr, 0, mask, data);
}

//
//    RMWCIB1: Read-Modify-Write on Byte 1 of the cfg register, only updating the bits set in mask
//
//    Arguments: see RMWCIB0 above
//

void Tensix::RMWCIB1(
        uint32_t mask, 
        uint32_t data, 
        uint32_t cfg_reg_addr) {
    rmwcib(cfg_reg_addr, 1, mask, data);
}

//
//    RMWCIB2: Read-Modify-Write on Byte 2 of the cfg register, only updating the bits set in mask
//
//    Arguments: see RMWCIB0 above
//

void Tensix::RMWCIB2(
        uint32_t mask, 
        uint32_t data, 
        uint32_t cfg_reg_addr) {
    rmwcib(cfg_reg_addr, 2, mask, data);
}

//
//    RMWCIB3: Read-Modify-Write on Byte 3 of the cfg register, only updating the bits set in mask
//
//    Arguments: see RMWCIB0 above
//

void Tensix::RMWCIB3(
        uint32_t mask, 
        uint32_t data, 
        uint32_t cfg_reg_addr) {
    rmwcib(cfg_reg_addr, 3, mask, data);
}

// utility methods

uint32_t Tensix::read_cfg_reg(uint32_t reg) {
    assert(reg < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    return m_cfg_reg[cfg_state_id][reg];
}

void Tensix::write_cfg_reg(uint32_t reg, uint32_t value) {
    // same as wrcfg32
    assert(reg < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    m_cfg_reg[cfg_state_id][reg] = value;
    // Trigger state update
    on_wrcfg32(reg);
}

// implementation

void Tensix::setc16(uint32_t reg, uint32_t value) {
    assert(reg < THD_REG_COUNT);
    value &= 0xFFFF;
    m_thd_reg[m_thread_id][reg] = value;
    // Trigger state changes
    on_setc16(reg);
}

void Tensix::wrcfg8(uint32_t reg, uint32_t value, uint32_t offset) {
    assert(reg < CFG_REG_COUNT);
    assert(offset < 4);
    uint32_t shift = offset * 8;
    uint32_t mask = (uint32_t(0xFF) << shift);
    // TODO: Figure out whether value also must be shifted
    value &= 0xFF;
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    uint32_t *ptr = &m_cfg_reg[cfg_state_id][reg];
    *ptr = (*ptr & ~mask) | ((value << shift) & mask);
    // Trigger state update
    on_wrcfg(reg, mask);
}

void Tensix::wrcfg16(uint32_t reg, uint32_t value, uint32_t offset) {
    assert(reg < CFG_REG_COUNT);
    assert(offset == 0 || offset == 2);
    uint32_t shift = offset * 8;
    uint32_t mask = (uint32_t(0xFFFF) << shift);
    // TODO: Figure out whether value also must be shifted
    value &= 0xFFFF;
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    uint32_t *ptr = &m_cfg_reg[cfg_state_id][reg];
    *ptr = (*ptr & ~mask) | ((value << shift) & mask);
    // Trigger state update
    on_wrcfg(reg, mask);
}

void Tensix::wrcfg32(uint32_t reg, uint32_t value) {
    assert(reg < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    m_cfg_reg[cfg_state_id][reg] = value;
    // Trigger state update
    on_wrcfg32(reg);
}

void Tensix::wrcfg128(uint32_t reg, const uint32_t *value) {
    assert(reg + 3 < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    uint32_t *ptr = &m_cfg_reg[cfg_state_id][reg];
    ptr[0] = value[0];
    ptr[1] = value[1];
    ptr[2] = value[2];
    ptr[3] = value[3];
    // Trigger state update
    on_wrcfg128(reg);
}

void Tensix::rmwcib(uint32_t reg, uint32_t offset, uint32_t mask, uint32_t value) {
    uint32_t shift = offset * 8;
    mask = ((mask & 0xFF) << shift);
    value = ((value & 0xFF) << shift);
    assert(reg < CFG_REG_COUNT);
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    uint32_t *ptr = &m_cfg_reg[cfg_state_id][reg];
    *ptr = (*ptr & ~mask) | (value & mask);
    // Trigger state update
    on_wrcfg(reg, mask);
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

