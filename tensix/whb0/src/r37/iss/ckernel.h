// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h"

#include "ckernel_include.h"

namespace ckernel {

extern MEM_PTR_T reg_base;
extern MEM_PTR_T pc_buf_base;
extern MEM_PTR_T regfile;

extern uint32_t *instrn_buffer;

//
//    Following code is copied from original "ckernel.h"
//    Necessary changes are marked with [-ISS]
//    Some unused functions are not included
//

void tensix_sync();
void mop_sync();

inline void sync_regfile_write(const uint index);

// Field value overflow check
template<typename T>
static constexpr bool is_valid(const T val, const uint8_t wid)
{
	const T mask = (1 << wid) - 1;
	return (val & mask) == val;
}

inline void mmio_register_write(register_space_e space, uint addr, uint data)
{
    const uint regaddr = (space << 6) | (addr & 0x3F);
    reg_base[regaddr] = data;
}

inline uint8_t semaphore_read(const uint8_t index)
{
    return pc_buf_base[PC_BUF_SEMAPHORE_BASE + index];
}

inline void semaphore_post(const uint8_t index)
{
    pc_buf_base[PC_BUF_SEMAPHORE_BASE + index] = 0;
}

inline void semaphore_get(const uint8_t index)
{
    pc_buf_base[PC_BUF_SEMAPHORE_BASE + index] = 1;
}

// Tensix thread semaphore post optionally stalled
template <uint WaitRes = p_stall::NONE>
inline void t6_semaphore_post(const uint8_t index)
{
    if constexpr (WaitRes != p_stall::NONE)
        TTI_STALLWAIT(p_stall::STALL_SYNC, WaitRes);

    TTI_SEMPOST(semaphore::t6_sem(index));
}

// Tensix thread semaphore get optionally stalled
template <uint WaitRes = p_stall::NONE>
inline void t6_semaphore_get(const uint8_t index)
{
    if constexpr (WaitRes != p_stall::NONE)
        TTI_STALLWAIT(p_stall::STALL_SYNC, WaitRes);

    TTI_SEMGET(semaphore::t6_sem(index));
}

// Tensix thread semaphore get optionally stalled
inline void t6_semaphore_init(const uint8_t index, const uint8_t min_value, const uint8_t max_value)
{
    TTI_SEMINIT(max_value, min_value, semaphore::t6_sem(index));
}

inline void t6_mutex_acquire(const uint8_t index)
{
    TTI_ATGETM(index);
}

inline void t6_mutex_release(const uint8_t index)
{
    TTI_ATRELM(index);
}

// Return address of the current state ID register
inline uint cfg_addr(uint cfg_addr32)
{
    return (cfg_state_id == 0) ? cfg_addr32 : (CFG_STATE_SIZE * 4) + cfg_addr32;
}

inline void cfg_write(uint cfg_addr32, uint data)
{
    // Declared here instead of globally to prevent direct access, which might ignore current state ID
#if 0 // [-ISS]
    volatile uint tt_reg_ptr *cfg_regs = reinterpret_cast<volatile uint tt_reg_ptr *>(TENSIX_CFG_BASE);
#endif
    MEM_PTR_T cfg_regs = MAP_MEM_PTR(TENSIX_CFG_BASE);
    cfg_regs[cfg_addr(cfg_addr32)] = data;
}

inline uint cfg_read(uint cfg_addr32)
{
    // Declared here instead of globally to prevent direct access, which might ignore current state ID
#if 0 // [-ISS]
    volatile uint32_t tt_reg_ptr *cfg_regs = reinterpret_cast<volatile uint32_t tt_reg_ptr *>(TENSIX_CFG_BASE);
#endif
    MEM_PTR_T cfg_regs = MAP_MEM_PTR(TENSIX_CFG_BASE);
    return cfg_regs[cfg_addr(cfg_addr32)];
}

#if 0 // [-ISS]
// Return pointer to CFG with the right base address for the current state
inline volatile uint * tt_reg_ptr get_cfg_pointer()
{
    if (cfg_state_id == 0)
        return reinterpret_cast<volatile uint tt_reg_ptr *>(TENSIX_CFG_BASE);

    return reinterpret_cast<volatile uint tt_reg_ptr *>(TENSIX_CFG_BASE + CFG_STATE_SIZE * 16);
}
#endif

// Return pointer to CFG with the right base address for the current state
inline MEM_PTR_T get_cfg_pointer()
{
    if (cfg_state_id == 0)
        return MAP_MEM_PTR(TENSIX_CFG_BASE);

    return MAP_MEM_PTR(TENSIX_CFG_BASE + CFG_STATE_SIZE * 16);
}

#if 0 // TODO? (Currently unused)
inline volatile uint short * tt_reg_ptr get_cfg16_pointer()
{
    if (cfg_state_id == 0)
        return reinterpret_cast<volatile uint short tt_reg_ptr *>(TENSIX_CFG_BASE);

    return reinterpret_cast<volatile uint short tt_reg_ptr *>(TENSIX_CFG_BASE + CFG_STATE_SIZE * 16);
}
#endif

inline void flip_cfg_state_id()
{
    cfg_state_id = 1 - cfg_state_id;
    TT_SETC16(CFG_STATE_ID_StateID_ADDR32, cfg_state_id); // Program the current state ID
    TTI_NOP;
    TTI_NOP;
}

inline void reset_cfg_state_id()
{
    cfg_state_id = 0;
}

inline void reset_dest_offset_id()
{
    dest_offset_id = 0;
}

// MOP run version without zmask
inline void mop_run(const uint8_t type, const uint8_t count)
{
    TTI_MOP(type, count - 1, 0); // Run the MOP
}

inline __attribute__((always_inline)) uint32_t reg_read(uint32_t addr)
{
#if 0 // [-ISS]
    volatile uint tt_reg_ptr *p_reg = reinterpret_cast<volatile uint tt_reg_ptr *> (addr);
#endif
    MEM_PTR_T p_reg = MAP_MEM_PTR(addr);
    return p_reg[0];
}

inline void reg_write(uint32_t addr, uint32_t data)
{
#if 0 // [-ISS]
    volatile uint tt_reg_ptr *p_reg = reinterpret_cast<volatile uint tt_reg_ptr *> (addr);
#endif
    MEM_PTR_T p_reg = MAP_MEM_PTR(addr);
    p_reg[0] = data;
}

// SKIPPED: inline void wait(uint32_t cycles)

// Clear dest
inline void zeroacc() {
    // Clear dest
    addr_mod_t{
        .srca = {.incr = 0},
        .srcb = {.incr = 0},
        .dest = {.incr = 0},
    }.set(ADDR_MOD_1);
    TT_ZEROACC(p_zeroacc::CLR_ALL, ADDR_MOD_1, 0);
}

inline void zerosrc() {
    TTI_ZEROSRC(0,0,1,3); // Zero all srcA&B banks
}

inline void sync_regfile_write(const uint index)
{
    // nothing to do in case of ISS
#if 0 // [-ISS]
    volatile uint foo = 0x0;
    volatile uint *fooptr = &foo;
    *fooptr = regfile[index];
#endif
}

inline void cfg_rmw(uint32_t cfg_addr32, uint32_t cfg_shamt, uint32_t cfg_mask, uint32_t val)
{
    uint32_t wrdata = val;

    // Avoid multiplication of variables!
    //const uint32_t addr = (cfg_state_id * CFG_STATE_SIZE * 4) + cfg_addr32;
    const uint32_t addr = (cfg_state_id == 0) ? cfg_addr32 : (CFG_STATE_SIZE * 4) + cfg_addr32;

    // Declared here instead of globally to prevent direct access, which might ignore current state ID
#if 0 // [-ISS]
    volatile uint tt_reg_ptr *cfg_regs = reinterpret_cast<volatile uint tt_reg_ptr *>(TENSIX_CFG_BASE);
#endif
    MEM_PTR_T cfg_regs = MAP_MEM_PTR(TENSIX_CFG_BASE);
    uint32_t cfg_data = cfg_regs[addr];

    // Shift and mask wrdata to properly align withn 32-bit DWORD
    wrdata <<= cfg_shamt;
    wrdata &= cfg_mask;

    // Zero-out relevant bits in cfg data
    cfg_data &= ~cfg_mask;

    // Or new data bits
    cfg_data |= wrdata;

    //Update cfg regs
    cfg_regs[addr] = cfg_data;
}

inline void cfg_rmw_gpr(uint32_t cfg_addr32, uint32_t cfg_shamt, uint32_t cfg_mask, uint32_t gpr_index)
{
    const uint32_t wrdata = regfile[gpr_index];
    cfg_rmw(cfg_addr32, cfg_shamt, cfg_mask, wrdata);
}

template <uint CfgAddr32, uint Shamt, uint Mask>
inline void cfg_reg_rmw_tensix(uint32_t val)
{
    uint32_t wrdata = val<<Shamt;
    uint8_t mask_b0 = Mask & 0xff;

    if (mask_b0!=0){
        uint8_t data_b0 = wrdata & 0xff;
        TT_RMWCIB0(mask_b0, data_b0, CfgAddr32);
    }
    wrdata>>=8;
    uint8_t mask_b1 = (Mask>>8) & 0xff;

    if (mask_b1!=0){
        uint8_t data_b1 = (wrdata) & 0xff;
        TT_RMWCIB1(mask_b1, data_b1, CfgAddr32);
    }

    wrdata>>=8;
    uint8_t mask_b2 = (Mask>>16) & 0xff;

    if (mask_b2!=0){
        uint8_t data_b2 = (wrdata) & 0xff;
        TT_RMWCIB2(mask_b2, data_b2, CfgAddr32);
    }

    wrdata>>=8;
    uint8_t mask_b3 = (Mask>>24) & 0xff;
    if (mask_b3!=0){
        uint8_t data_b3 = (wrdata) & 0xff;
        TT_RMWCIB3(mask_b3, data_b3, CfgAddr32);
    }
}

inline void debug_dump(const uint8_t *data, uint32_t byte_size) {
    // does nothing in ISS version
}

inline void debug_dump_seek(uint8_t offset) {
    // does nothing in ISS version
}

// SKIPPED: Some functions until tensix_sync

inline void tensix_sync()
{
    // nothing to do in case of ISS
#if 0 // [-ISS]
    volatile uint foo = 0x0;
    volatile uint *fooptr = &foo;
    // Write to pc buffer to push all writes ahead of us.. otherwise, the pc buffer read can bypass older writes
    pc_buf_base[1] = foo;

    // Now read -- this read will block until we're idle
    *fooptr = pc_buf_base[1];
#endif
}

inline void mop_sync()
{
    // nothing to do in case of ISS
#if 0 // [-ISS]
    volatile uint foo = 0x0;
    volatile uint *fooptr = &foo;
    // Write to pc buffer to push all writes ahead of us.. otherwise, the pc buffer read can bypass older writes
    pc_buf_base[2] = foo;

    // Now read -- this read will block until mops are done
    *fooptr = pc_buf_base[2];
#endif
}

// SKIPPED: inline void llk_get_next_op_info(tt::op_info_t& op_info_struct)
//     (op_info_offset global is not yet supported by ISS)

} // namespace ckernel

