// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
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
//    RSTDMA: Soft reset of TDMA engine
//
//    Is this instruction really used? [-A.G.]
//

void Tensix::RSTDMA() {
    // nothing to do so far
}

//
//    SETDMAREG: Set TDMA register file register with 16b immediate value provided
//        with instruction or with routed signals from TDMA via indirect side-effects
//
//    payload_sig_sel_size    Payload size in SetSignals mode
//                                0 - 16-bit
//                                1 - 32-bit
//                                2 - 128-bit
//                                3 - TileHeader
//    payload_sig_sel         In normal mode - immediate payload to set into selected register.
//                            In SetSignals mode selector for which 16b chunk of signals to set
//                            into selected register. 
//                                0 - Accumulated tile size (16-bit) and last written
//                                    tile size (16-bit) for 4 packers (128-bit)
//                                1 - All zero flag per XY plane [31:0] for 4 packers (128-bit)
//                                2 - Tile header for packer 0 (SigSelSize set to 3 - TileHeader)
//                                3 - Tile header for packer 1 (SigSelSize set to 3 - TileHeader)
//                                4 - Tile header for packer 2 (SigSelSize set to 3 - TileHeader)
//                                5 - Tile header for packer 3 (SigSelSize set to 3 - TileHeader)
//                                6 - FPU stats 0 (payload bits 8:7 are used to select packer)
//                                7 - FPU stats 1 (payload bits 8:7 are used to select packer)
//                                8 - Clear accumulated tile size as a side-effect
//                                    (payload bits 10:7 is the mask used to select packer)
//                                9 - FPU stats max exp
//    set_signals_mode        Sets registers from signals routed into TDMA block rather than
//                            from payload immediate bellow. In this mode payload immediate
//                            represents a mux control for which 16b signal chunk to feed into
//                            selected register
//    reg_index_16b           DMA register index in 16b quants
//

void Tensix::SETDMAREG(
        uint32_t payload_sig_sel_size,
        uint32_t payload_sig_sel,
        uint32_t set_signals_mode,
        uint32_t reg_index_16b) {
    // ACHTUNG: SetSignals mode is not supported
    assert(set_signals_mode == 0);
    uint32_t *reg = &m_dma_reg[m_thread_id][reg_index_16b >> 1];
    uint32_t val = (payload_sig_sel & 0xFFFF);
    if ((reg_index_16b & 0x1) != 0) {
        *reg = (*reg & 0xFFFF) | (val << 16);
    } else {
        *reg = (*reg & 0xFFFF0000) | val;
    }
}

//
//    ADDDMAREG: If OpBisConst is 0 adds 32-bit values from DMA registers pointed by
//        OpARegIndex and OpBRegIndex or if OpBisConst is 1 adds 32-bit value from
//        DMA register pointed by OpARegIndex and 6-bit const value set in OpBRegIndex field
//        and writes result to register ResultRegIndex
//
//    op_bis_const        Use OpBRegIndex as const value for math operation
//    result_reg_index    TDMA 32b register index for result
//    op_breg_index       TDMA 32b register index for operand B
//    op_areg_index       TDMA 32b register index for operand A
//

void Tensix::ADDDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    if (op_bis_const) {
        gpr[result_reg_index] = gpr[op_areg_index] + op_breg_index;
    } else {
        gpr[result_reg_index] = gpr[op_areg_index] + gpr[op_breg_index];
    }
}

//
//    SUBDMAREG: If OpBisConst is 0 subtracts 32-bit values from DMA registers pointed by
//        OpARegIndex and OpBRegIndex or if OpBisConst is 1 subtracts 32-bit value from
//        DMA register pointed by OpARegIndex with 6-bit const value set in OpBRegIndex field
//        and writes result to register ResultRegIndex
//
//    Arguments: see ADDDMAREG above
//

void Tensix::SUBDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    if (op_bis_const) {
        gpr[result_reg_index] = gpr[op_areg_index] - op_breg_index;
    } else {
        gpr[result_reg_index] = gpr[op_areg_index] - gpr[op_breg_index];
    }
}

//
//    MULDMAREG: If OpBisConst is 0 multiplies 16-bit values from DMA registers pointed by
//        OpARegIndex and OpBRegIndex or if OpBisConst is 1 multiplies 16-bit value from
//        DMA register pointed by OpARegIndex with 6-bit const value set in OpBRegIndex field
//        and writes result to register ResultRegIndex
//
//    Arguments: see ADDDMAREG above
//

void Tensix::MULDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    uint32_t a = (gpr[op_areg_index] & 0xFFFF);
    if (op_bis_const) {
        uint32_t r = a * op_breg_index;
        gpr[result_reg_index] = (r & 0xFFFF);
    } else {
        uint32_t b = (gpr[op_breg_index] & 0xFFFF);
        uint32_t r = a * b;
        gpr[result_reg_index] = (r & 0xFFFF);
    }
}

//
//    BITWOPDMAREG: If OpBisConst is 0 performs bit-wise operation on 32-bit values from
//        DMA registers pointed by OpARegIndex and OpBRegIndex or if OpBisConst is 0 performs
//        bit-wise operation on 32-bit value from DMA register pointed by OpARegIndex with 6-bit
//        const value set in OpBRegIndex field and writes result to register ResultRegIndex
//
//    op_bis_const        Use OpBRegIndex as const value for math operation
//    op_sel              0 - AND, 1 - OR, 2 - XOR
//    result_reg_index    TDMA 32b register index for result
//    op_breg_index       TDMA 32b register index for operand B
//    op_areg_index       TDMA 32b register index for operand A
//

void Tensix::BITWOPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    if (op_sel == 0) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] & op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] & gpr[op_breg_index]);
        }
    } else if (op_sel == 1) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] | op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] | gpr[op_breg_index]);
        }
    } else if (op_sel == 2) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] ^ op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] ^ gpr[op_breg_index]);
        }
    } else {
        assert(false);
    }
}

//
//    SHIFTDMAREG: Shifts OpARegIndex <<|>> OpBRegIndex and writes result to register ResultRegIndex.
//        If OpBisConst is 1 6-bit const value set in OpBRegIndex field is used as operand B.
//
//    op_bis_const        Use OpBRegIndex as const value for math operation
//    op_sel              0 - LEFT, 1 - RIGHT
//    result_reg_index    TDMA 32b register index for result
//    op_breg_index       TDMA 32b register index for operand B
//    op_areg_index       TDMA 32b register index for operand A
//

void Tensix::SHIFTDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    if (op_sel == 0) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] << op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] << gpr[op_breg_index]);
        }
    } else if (op_sel == 1) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] >> op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] >> gpr[op_breg_index]);
        }
    } else {
        assert(false);
    }
}

//
//    CMPDMAREG: Compares values in OpARegIndex and OpBRegIndex registers and writes
//        result 0 or 1 to register ResultRegIndex. If OpBisConst is 1 6-bit const value set
//        in OpBRegIndex field is used as operand B.
//
//    op_bis_const        Use OpBRegIndex as const value for math operation
//    op_sel              0 - (A > B), 1 - (A < B), 2 - (A == B)
//    result_reg_index    TDMA 32b register index for result
//    op_breg_index       TDMA 32b register index for operand B
//    op_areg_index       TDMA 32b register index for operand A
//

void Tensix::CMPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    // TODO: Figure out whether comparison is signed or unsigned
    uint32_t *gpr = m_dma_reg[m_thread_id];
    if (op_sel == 0) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] > op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] > gpr[op_breg_index]);
        }
    } else if (op_sel == 1) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] < op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] < gpr[op_breg_index]);
        }
    } else if (op_sel == 2) {
        if (op_bis_const) {
            gpr[result_reg_index] = (gpr[op_areg_index] == op_breg_index);
        } else {
            gpr[result_reg_index] = (gpr[op_areg_index] == gpr[op_breg_index]);
        }
    } else {
        assert(false);
    }
}

//
//    ATINCGET: Atomic increment and get - will read value in targeted memory location and
//        return it to TDMA register and post-increment it atomically with the value pointed
//        by DataRegIndex field
//
//    mem_hier_sel      Selects whether transaction is targeting DataRam (0) or L1 (1)
//    wrap_val          Wrap the targeted address value at (1 << wrap_val[4:0])
//    sel_32b           Selects the 32b word within a 16B word to operate on
//    data_reg_index    Source(inc value) and destination data register index
//    addr_reg_index    Address register index (16B word address)
//

void Tensix::ATINCGET(
        uint32_t mem_hier_sel,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    // TODO
}

//
//    ATINCGETPTR: Atomic increment and get pointer - will access a memory location designated
//        as a FIFO pointer location (contains a 32b read pointer and a 32b write pointer),
//        return the pointer value to TDMA register and post-increment it unless the FIFO
//        condition precludes that. For example, write pointer will not be incremented if FIFO
//        is full. Read pointer will not be incremented if FIFO is empty. FIFO full or empty
//        conditions are returned as an unsuccessful return condition code, so that the thread
//        controller can retry until success (retry reads if FIFO empty, retry writes if FIFO
//        full.) If NoIncr is set to 1 then hardware will not increment the pointers but retry
//        will still occur if FIFO is empty or full.
//
//    mem_hier_sel      Selects whether transaction is targetting DataRam (0) or L1 (1)
//    no_incr           Disable pointer increment
//    incr_val          Post-increment by (1 << incr_val) if no_incr is 0
//    wrap_val          Wrap the targeted address value at (1 << wrap_val)
//                      (e.g. Set wrap_val == 3 for a 4-byte byte-addressed FIFO)
//    sel_32b           Selects the 32b word within a 16B word to operate on
//                      (i.e. Read/Write select)
//    data_reg_index    Destination data register index
//    addr_reg_index    Address register index (16B word address)
//

void Tensix::ATINCGETPTR(
        uint32_t mem_hier_sel,
        uint32_t no_incr,
        uint32_t incr_val,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    // TODO
}

//
//    ATSWAP: Atomic unconditional SWAP. Swaps selected 16b chunks of memory location
//        with new ones provided on write data bus.
//
//    mem_hier_sel      Selects whether transaction is targetting DataRam (0) or L1 (1)
//    swap_mask         Swap mask at 16b granulatiry (8b of mask for 16B words)
//    data_reg_index    First of four data register indexes. Must be aligned to 4.
//    addr_reg_index    Address register index (16B word address)
//

void Tensix::ATSWAP(
        uint32_t mem_hier_sel,
        uint32_t swap_mask,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    // TODO
}

//
//    ATCAS: Atomic compare-and-swap. If value at selected memory location matches that provided
//        by programmer it is swapped to a new one, also provided by programmer. Otherwise,
//        instruction will be retried until it does match at which time it will swap.
//        This instruction is implemented for implementations of mutual exclusion between
//        Tensix cores and threads.
//
//    mem_hier_sel      Selects whether transaction is targeting DataRam (0) or L1 (1)
//    swap_val          Swap value provided as an immediate (4 bits)
//    cmp_val           Compare value provided as an immediate (4 bits)
//    sel_32b           Selects the 32b word within a 16B word to operate on
//    data_reg_index    Destination data register index
//                      (FIXME: unused and to be removed. Issue #434)
//    addr_reg_index    Address register index (16B word address)
//

void Tensix::ATCAS(
        uint32_t mem_hier_sel,
        uint32_t swap_val,
        uint32_t cmp_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    // TODO
}

//
//    REG2FLOP: Move data from TDMA register file into flip flops driving actual config signals.
//        Used for certain TDMA configuration signal setting.
//
//    size_sel        Select size of the data transfer:
//                        0: 16 Bytes
//                        1: 32 bits
//                        2: bits (16 bits?)
//                        3: 8 bits 
//    target_sel      Target Select:
//                        0: TDMA Registers
//                        1: Local Registers
//                        2: Context Registers
//                        3: Override thread id with context id
//    byte_offset     Offset (in bytes) into the 32 bit register location.
//                    Used for field extraction during 8 or 16 bit moves.
//    context_id_2    Context ID 0..3
//    flop_index      TDMA 32b flop storage location
//    reg_index       TDMA 32b register index
//

void Tensix::REG2FLOP(
        uint32_t size_sel,
        uint32_t target_sel,
        uint32_t byte_offset,
        uint32_t context_id_2,
        uint32_t flop_index,
        uint32_t reg_index) {
    // TODO: Figure out whether 'size_sel' is significant if (target_sel != 0)
    //     (currently it is supported for TDMA registers only)
    uint32_t *gpr = &m_dma_reg[m_thread_id][reg_index];
    if (target_sel == 0) {
        uint32_t reg = flop_index + THCON_CFG_REG_BASE;
        if (size_sel == 0) {
            wrcfg128(reg, gpr);
        } else if (size_sel == 1) {
            wrcfg32(reg, *gpr);
        } else if (size_sel == 2) {
            wrcfg16(reg, *gpr, byte_offset);
        } else if (size_sel == 3) {
            wrcfg8(reg, *gpr, byte_offset);
        } else {
            assert(false);
        }
    } else if (target_sel == 1) {
        // TODO: Figure out whether non-identity mapping of 'flop_index' to CFG reg is required
        uint32_t reg = flop_index;
        setc16(reg, *gpr);
    } else if (target_sel == 2 || target_sel == 3) {
        uint32_t context_id = (target_sel == 3) ? context_id_2 : m_thread_id;
        uint32_t dimension_index = flop_index % 4;     // X = 0, Y = 1, Z = 2, W = 3
        uint32_t counter_id = (flop_index / 4) % 8;    // UNP0 = 0, UNP1 = 2, PCK0 = 4
        uint32_t channel_index = flop_index / 32;      // src = 0, dst = 1
        setadc(context_id, counter_id, dimension_index, channel_index, *gpr);
    } else {
        assert(false);
    }
}

//
//    LOADIND: Load indirect from address specified in a TDMA register, with offset specified
//        in TDMA register to a TDMA register. Supports autoincrementing offset.
//
//    size_sel          Select whether the data move is 0 - 16B, 1 - 32b, 2 - 16b, 3 - 8b size
//    offset_index      Offset address register index, in 16b quants
//                      (i.e. double the normal R value). Offset is in Bytes.
//    auto_inc_spec     Autoincrement spec:
//                          0 - no autoinc
//                          1 - autoinc by 2B(ytes)
//                          2 - autoinc by 4B
//                          3 - autoinc by 16B
//    data_reg_index    Destination data register index
//    addr_reg_index    Address register index. Address is a L1 16B address
//

void Tensix::LOADIND(
        uint32_t size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    // TODO
}

//
//    STOREIND: Store indirect. Stores data from TDMA register to memory location 
//        specified by a combination of base + offset provided in other TDMA registers.
//        Supports auto-increment on offset value.
//
//    mem_hier_sel      Selects whether transaction is targetting source regfile (0) or L1 (1)
//                          p_ind::HIER_REGFILE
//                          p_ind::HIER_L1
//    size_sel          Selects whether the data move is 16B (0) or 32b (1) size
//    reg_size_sel      If mem_hier_sel bit is set to 0 (source regfile) then
//                          this bit selects between src A and B register file: 
//                          0 - SRCA, 1 - SRCB. Supported data move size is 16B and 32b.
//                      If mem_hier_sel bit is set to 1 (L1) then bits 22:21 are used for
//                          setting data move size: 0 - 16B, 1 - 32b, 2 - 16b, 3 - 8b size
//                              p_ind::LD_16B
//                              p_ind::LD_32bit
//                              p_ind::LD_16bit
//                              p_ind::LD_8bit
//    offset_index      Offset address register index (in 16b quants? [-A.G.])
//    auto_inc_spec     Autoincrement spec: 0 - no autoinc, 1 - autoinc by 2(B)ytes,
//                          2 - autoinc by 4B, 3 - autoinc by 16B
//                              p_ind::INC_NONE
//                              p_ind::INC_2B
//                              p_ind::INC_4B
//                              p_ind::INC_16B
//    data_reg_index    Source data register index
//    addr_reg_index    Address register index
//

void Tensix::STOREIND(
        uint32_t mem_hier_sel,
        uint32_t size_sel,
        uint32_t reg_size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    uint32_t *gpr = m_dma_reg[m_thread_id];
    uint32_t *data_reg = &gpr[data_reg_index];
    uint32_t base = gpr[addr_reg_index];
    uint32_t *offset_reg = &gpr[offset_index >> 1];
    uint32_t offset_half = (offset_index & 0x1);
    uint32_t offset = (offset_half == 0) ? (*offset_reg & 0xFFFF) : (*offset_reg >> 16);
    if (mem_hier_sel == 0) {
        // TODO: Discover exact algorithm for regfile case
        float *src = (reg_size_sel == 0) ? m_srca[0] : m_srcb[0];
        // Assume base and offset in bytes: is this correct in regfile case?
        uint32_t *ptr = reinterpret_cast<uint32_t *>(src + base + offset);
        if (size_sel == 0) {
            ptr[0] = data_reg[0];
            ptr[1] = data_reg[1];
            ptr[2] = data_reg[2];
            ptr[3] = data_reg[3];
        } else if (size_sel == 1) {
            ptr[0] = data_reg[0];
        } else {
            assert(false);
        }
    } else if (mem_hier_sel == 1) {
        uint8_t *ptr = m_memory->map_addr(base + offset);
        // 'size_sel' is ignored in L1 case
        if (reg_size_sel == 0) {
            uint32_t *p32 = reinterpret_cast<uint32_t *>(ptr);
            p32[0] = data_reg[0];
            p32[1] = data_reg[1];
            p32[2] = data_reg[2];
            p32[3] = data_reg[3];
        } else if (reg_size_sel == 1) {
            uint32_t *p32 = reinterpret_cast<uint32_t *>(ptr);
            p32[0] = data_reg[0];
        } else if (reg_size_sel == 2) {
            // little endian assumed
            uint16_t *s16 = reinterpret_cast<uint16_t *>(data_reg);
            uint16_t *p16 = reinterpret_cast<uint16_t *>(ptr);
            *p16 = *s16;
        } else if (reg_size_sel == 3) {
            // little endian assumed
            uint8_t *p8 = reinterpret_cast<uint8_t *>(ptr);
            *p8 = *data_reg;
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }
    assert(auto_inc_spec < 4);
    uint32_t incr =
        (auto_inc_spec == 0) ? 0 :
        (auto_inc_spec == 1) ? 2 :
        (auto_inc_spec == 2) ? 4 : 16;
    offset = (offset + incr) & 0xFFFF;
    if (offset_half == 0) {
        *offset_reg = ((*offset_reg & 0xFFFF0000) | offset);
    } else {
        *offset_reg = (*offset_reg & 0xFFFF) | (offset << 16);
    }
}

//
//    LOADREG: Loads data from local RISC register space to GPR
//        (Shall final register address be computed like for STOREREG? - [A.G.])
//
//    tdma_data_reg_index    Dest data register index
//    reg_addr               18-bit register index aligned to 32-bit
//

void Tensix::LOADREG(uint32_t tdma_data_reg_index, uint32_t reg_addr) {
    // TODO
}

//
//    STOREREG: Stores data from GPR to local RISC register space.
//        Final register address is computed as 0xFFB0_0000 | (reg_addr << 2)
//
//    tdma_data_reg_index    Source data register index
//    reg_addr               18-bit register index aligned to 32-bit
//

void Tensix::STOREREG(uint32_t tdma_data_reg_index, uint32_t reg_addr) {
    // TODO
}

//
//    FLUSHDMA: Flush TDMA engine or some subset of it as specified by instruction argument
//
//    flush_spec    Flushes (blocks until completely drained and idle) the entire TDMA engine
//                  or a selected component of it:
//                      0 - flush entire DMA
//                      1 - flush TC (thread controller)
//                      2 - flush unpacker0
//                      4 - flush unpacker1
//                      8 - flush packer
//

void Tensix::FLUSHDMA(uint32_t flush_spec) {
    // nothing to do
}

//
//    DMANOP: Do nothing and consume an tdma instruction slot and cycle
//

void Tensix::DMANOP() {
    // nothing to do
}

//
//    TBUFCMD: [reserved for future use]
//

void Tensix::TBUFCMD() {
    // nothing to do so far
}

// primitives

uint32_t Tensix::read_dma_reg(uint32_t reg) {
    return m_dma_reg[m_thread_id][reg];
}

void Tensix::write_dma_reg(uint32_t reg, uint32_t val) {
    m_dma_reg[m_thread_id][reg] = val;
}

// implementation

void Tensix::setadc(
        uint32_t context_id,
        uint32_t counter_id,
        uint32_t dimension_index,
        uint32_t channel_index,
        uint32_t value) {
#define KEY(c, d) ((uint32_t(c) << 2) | uint32_t(d))
    assert(context_id <= 2);
    assert(counter_id == 0 || counter_id == 2 || counter_id == 4);
    assert(dimension_index < 4);
    assert(channel_index < 2);
    AddrCtrl &adc = 
        (counter_id == 0) ? m_adc[0] :
        (counter_id == 1) ? m_adc[1] : m_adc[2];
    switch (KEY(channel_index, dimension_index)) {
    case KEY(0, 0):
        adc.ch0_x = value;
        adc.ch0_x_cr = value;
        break;
    case KEY(0, 1):
        adc.ch0_y = value;
        adc.ch0_y_cr = value;
        break;
    case KEY(0, 2):
        adc.ch0_z = value;
        adc.ch0_z_cr = value;
        break;
    case KEY(0, 3):
        adc.ch0_w = value;
        adc.ch0_w_cr = value;
        break;
    case KEY(1, 0):
        adc.ch1_x = value;
        adc.ch1_x_cr = value;
        break;
    case KEY(1, 1):
        adc.ch1_y = value;
        adc.ch1_y_cr = value;
        break;
    case KEY(1, 2):
        adc.ch1_z = value;
        adc.ch1_z_cr = value;
        break;
    case KEY(1, 3):
        adc.ch1_w = value;
        adc.ch1_w_cr = value;
        break;
    default:
        assert(false);
        break;
    }
#undef KEY
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

