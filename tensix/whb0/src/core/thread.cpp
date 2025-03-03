// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cassert>

#include "core/addr_mod.hpp"
#include "core/template.hpp"
#include "core/instr_params.hpp"
#include "core/memory.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Thread
//

Thread::Thread(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync):
            m_thread_type(thread_type),
            m_tensix(tensix),
            m_thread_sync(thread_sync), 
            m_local_base(0),
            m_mop_type(~uint32_t(0)),
            // TODO: Figure out correct value for replay buffer depth
            m_log2_replay_buffer_depth(5), 
            m_replay_buffer(size_t(1) << m_log2_replay_buffer_depth),
            m_replay_index(0),
            m_replay_left(0),
            m_replay_execute_while_loading(false) { 
    setup_memory_layout();
}

Thread::~Thread() { }

void Thread::setup_memory_layout() {
    // ACHTUNG: This is temporary code for standalone Tensix model only.
    //     More general solution for specifying user-specific thread memory
    //     layouts shall be introduced in the future.
    Memory *memory = m_tensix->memory();
    uint32_t memory_size = memory->size();
    uint32_t local_size = 16 * 1024;
    uint32_t offset = 
        (m_thread_type == ThreadType::Unpack) ? 3 :
        (m_thread_type == ThreadType::Math) ? 2 : 1;
    m_local_base = memory_size - offset * local_size;
}

ThreadType Thread::thread_type() {
    return m_thread_type;
}

uint8_t *Thread::map_addr(uint32_t addr) {
    Memory *memory = m_tensix->memory();
    return memory->map_addr(addr);
}

// NONE

void Thread::NOP() {
    m_tensix->NOP();
}

// SYNC

void Thread::ATGETM(uint32_t mutex_index) {
    uint32_t thread_id = uint32_t(m_thread_type);
    if (!m_tensix->is_mutex_free(mutex_index, thread_id)) {
        Tensix *tensix = m_tensix;
        m_thread_sync->wait([tensix, mutex_index, thread_id]() -> bool {
            return tensix->is_mutex_free(mutex_index, thread_id);
        });
    }
    set_thread_id();
    m_tensix->ATGETM(mutex_index);
}

void Thread::ATRELM(uint32_t mutex_index) {
    set_thread_id();
    m_tensix->ATRELM(mutex_index);
}

void Thread::SEMINIT(
        uint32_t max_value,
        uint32_t init_value,
        uint32_t sem_sel) {
    m_tensix->SEMINIT(max_value, init_value, sem_sel);
}

void Thread::SEMPOST(uint32_t sem_sel) {
    m_tensix->SEMPOST(sem_sel);
}

void Thread::SEMGET(uint32_t sem_sel) {
    m_tensix->SEMGET(sem_sel);
}

void Thread::SEMWAIT(
        uint32_t stall_res,
        uint32_t sem_sel,
        uint32_t wait_sem_cond) {
    // Simulator will stall issuing thread rather than specified resource
    Tensix *tensix = m_tensix;
    uint32_t sem_index = Tensix::decode_sem_sel(sem_sel);
    if (wait_sem_cond == 0x01) {
        // wait while zero
        m_thread_sync->wait([tensix, sem_index]() -> bool {
            return !tensix->is_sem_zero(sem_index);
        });
    } else if (wait_sem_cond == 0x02) {
        // wait while max
        m_thread_sync->wait([tensix, sem_index]() -> bool {
            return !tensix->is_sem_max(sem_index);
        });
    } else {
        assert(false);
    }
}

void Thread::STALLWAIT(uint32_t stall_res, uint32_t wait_res) {
    // All relevant stall functionality is handled at this level
    if (stall_res == p_stall::STALL_MATH) {
        bool srca_vld = ((wait_res & p_stall::SRCA_VLD) != 0);
        bool srcb_vld = ((wait_res & p_stall::SRCB_VLD) != 0);
        wait_dvalid(srca_vld, srcb_vld);
    }
}

void Thread::wait_dvalid(bool srca_vld, bool srcb_vld) {
    Tensix *tensix = m_tensix;
    if (srca_vld && srcb_vld) {
        m_thread_sync->wait([tensix]() -> bool {
            return (tensix->is_srca_dvalid() && tensix->is_srcb_dvalid());
        });
    } else if (srca_vld) {
        m_thread_sync->wait([tensix]() -> bool {
            return tensix->is_srca_dvalid();
        });
    } else if (srcb_vld) {
        m_thread_sync->wait([tensix]() -> bool {
            return tensix->is_srcb_dvalid();
        });
    }
}

// MATH

void Thread::ZEROACC(
        uint32_t clear_mode,
        uint32_t addr_mode,
        uint32_t dst) {
    m_tensix->ZEROACC(clear_mode, addr_mode, dst);
}

void Thread::ZEROSRC(
        uint32_t zero_val,
        uint32_t write_mode,
        uint32_t bank_mask,
        uint32_t src_mask) {
    m_tensix->ZEROSRC(zero_val, write_mode, bank_mask, src_mask);
}

void Thread::MOVD2A(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    m_tensix->MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst);
}

void Thread::MOVD2B(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    m_tensix->MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst);
}

void Thread::MOVA2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    wait_dvalid(true, false);
    m_tensix->MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst);
}

void Thread::MOVB2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    wait_dvalid(false, true);
    m_tensix->MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst);
}

void Thread::MOVB2A(
        uint32_t srca,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t srcb) {
    wait_dvalid(false, true);
    m_tensix->MOVB2A(srca, addr_mode, instr_mod, srcb);
}

void Thread::MOVDBGA2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    // TODO
}

void Thread::TRNSPSRCA() {
    // TODO
}

void Thread::TRNSPSRCB() {
    wait_dvalid(false, true);
    m_tensix->TRNSPSRCB();
}

void Thread::ELWADD(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    // Waiting for Dvalid is required for synchronization with Unpack
    // thread as long as emulator does not implement twin SrcA/SrcB banks.
    // Unpack thread will not yield until respective Src register is made valid,
    // therefore this implementation is semantically correct.
    wait_dvalid(true, true);
    m_tensix->ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst);
}

void Thread::ELWSUB(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    wait_dvalid(true, true);
    m_tensix->ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst);
}

void Thread::ELWMUL(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    wait_dvalid(true, true);
    m_tensix->ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst);
}

void Thread::MVMUL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    wait_dvalid(true, true);
    m_tensix->MVMUL(clear_dvalid, instr_mod19, addr_mode, dst);
}

void Thread::GMPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst) {
    wait_dvalid(true, true);
    m_tensix->GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst);
}

void Thread::GAPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst) {
    wait_dvalid(true, true);
    m_tensix->GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst);
}

void Thread::GATESRCRST(uint32_t reset_srcb_gate_control, uint32_t reset_srca_gate_control) {
    m_tensix->GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control);
}

void Thread::CLEARDVALID(uint32_t clear_dvalid, uint32_t reset) {
    m_tensix->CLEARDVALID(clear_dvalid, reset);
}

void Thread::SETDVALID(uint32_t set_dvalid) {
    m_tensix->SETDVALID(set_dvalid);
}

void Thread::RAREB() {
    // TODO
}

void Thread::SETRWC(
        uint32_t clear_ab_vld,
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a,
        uint32_t bit_mask) {
    m_tensix->SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, bit_mask);
}

void Thread::INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) {
    m_tensix->INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a);
}

void Thread::SETIBRWC(
        uint32_t rwc_cr,
        uint32_t rwc_bias,
        uint32_t set_inc_ctrl) {
    // TODO
}

void Thread::SHIFTXA(uint32_t log2_amount2, uint32_t shift_mode) {
    // TODO
}

void Thread::SHIFTXB(
        uint32_t addr_mode,
        uint32_t rot_shift,
        uint32_t shift_row) {
    // TODO
}

void Thread::SETASHRMH0(uint32_t reg_mask, uint32_t halo_mask) {
    // TODO
}

void Thread::SETASHRMH1(uint32_t reg_mask, uint32_t halo_mask) {
    // TODO
}

void Thread::SETASHRMV(uint32_t reg_mask2) {
    // TODO
}

void Thread::SETASHRMH(uint32_t reg_mask, uint32_t halo_mask) {
    // TODO
}

void Thread::SETPKEDGOF(
        uint32_t y_end,
        uint32_t y_start,
        uint32_t x_end,
        uint32_t x_start) {
    // TODO
}

void Thread::CLREXPHIST() {
    // TODO
}

void Thread::DOTPV(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    // TODO
}

void Thread::CONV3S1(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst) {
    // TODO
}

void Thread::CONV3S2(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst) {
    // TODO
}

void Thread::MPOOL3S1(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst) {
    // TODO
}

void Thread::MPOOL3S2(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst) {
    // TODO
}

void Thread::APOOL3S1(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst) {
    // TODO
}

void Thread::APOOL3S2(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst) {
    // TODO
}

void Thread::MFCONV3S1(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst) {
    // TODO
}

// XMOV

void Thread::XMOV(uint32_t mov_block_selection, uint32_t last) {
    // TODO
}

// PACK

void Thread::PACR(
        uint32_t addr_mode,
        uint32_t zero_write,
        uint32_t pack_sel,
        uint32_t ovrd_thread_id,
        uint32_t concat,
        uint32_t flush,
        uint32_t last) {
    m_tensix->PACR(
        addr_mode,
        zero_write,
        pack_sel,
        ovrd_thread_id,
        concat,
        flush,
        last);
}

void Thread::PACR_SETREG(
        uint32_t push,
        uint32_t addr_sel,
        uint32_t wr_data,
        uint32_t pack_sel,
        uint32_t stream_id,
        uint32_t flush,
        uint32_t last) {
    // TODO
}

// UNPACK

void Thread::UNPACR(
        uint32_t unpack_block_selection,
        uint32_t addr_mode,
        uint32_t cfg_context_cnt_inc,
        uint32_t cfg_context_id,
        uint32_t addr_cnt_context_id,
        uint32_t ovrd_thread_id,
        uint32_t set_dat_valid,
        uint32_t rareb_en,
        uint32_t zero_write2,
        uint32_t auto_inc_context_id,
        uint32_t row_search,
        uint32_t search_cache_flush,
        uint32_t last) {
    // synchronize on Dvalid
    // wait until Math thread clears Dvalid for requested Src register
    // (required as long as emulator does not implement twin SrcA/SrcB banks)
    Tensix *tensix = m_tensix;
    if (unpack_block_selection == 0) {
        m_thread_sync->wait([tensix]() -> bool {
            return !tensix->is_srca_dvalid();
        });
    } else if (unpack_block_selection == 1) {
        m_thread_sync->wait([tensix]() -> bool {
            return !tensix->is_srcb_dvalid();
        });
    }

    m_tensix->UNPACR(
        unpack_block_selection,
        addr_mode,
        cfg_context_cnt_inc,
        cfg_context_id,
        addr_cnt_context_id,
        ovrd_thread_id,
        set_dat_valid,
        rareb_en,
        zero_write2,
        auto_inc_context_id,
        row_search,
        search_cache_flush,
        last);
}

void Thread::UNPACR_NOP(uint32_t unpack_block_selection, uint32_t no_op) {
    Tensix *tensix = m_tensix;
    if (unpack_block_selection == 0) {
        m_thread_sync->wait([tensix]() -> bool {
            return !tensix->is_srca_dvalid();
        });
    } else if (unpack_block_selection == 1) {
        m_thread_sync->wait([tensix]() -> bool {
            return !tensix->is_srcb_dvalid();
        });
    }

    m_tensix->UNPACR_NOP(unpack_block_selection, no_op);
}

// ADDRMOD

void Thread::SETADC(
        uint32_t cnt_set_mask,
        uint32_t channel_index,
        uint32_t dimension_index,
        uint32_t value) {
    m_tensix->SETADC(
        cnt_set_mask,
        channel_index,
        dimension_index,
        value);
}

void Thread::SETADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask) {
    m_tensix->SETADCXY(cnt_set_mask, ch1_y, ch1_x, ch0_y, ch0_x, bit_mask);
}

void Thread::INCADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x) {
    m_tensix->INCADCXY(cnt_set_mask, ch1_y, ch1_x, ch0_y, ch0_x);
}

void Thread::ADDRCRXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask) {
    m_tensix->ADDRCRXY(cnt_set_mask, ch1_y, ch1_x, ch0_y, ch0_x, bit_mask);
}

void Thread::SETADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask) {
    m_tensix->SETADCZW(cnt_set_mask, ch1_w, ch1_z, ch0_w, ch0_z, bit_mask);
}

void Thread::INCADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z) {
    m_tensix->INCADCZW(cnt_set_mask, ch1_w, ch1_z, ch0_w, ch0_z);
}

void Thread::ADDRCRZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask) {
    m_tensix->ADDRCRZW(cnt_set_mask, ch1_w, ch1_z, ch0_w, ch0_z, bit_mask);
}

void Thread::SETADCXX(
        uint32_t cnt_set_mask,
        uint32_t x_end2,
        uint32_t x_start) {
    m_tensix->SETADCXX(cnt_set_mask, x_end2, x_start);
}

// TDMA

void Thread::RSTDMA() {
    set_thread_id();
    m_tensix->RSTDMA();
}

void Thread::SETDMAREG(
        uint32_t payload_sig_sel_size,
        uint32_t payload_sig_sel,
        uint32_t set_signals_mode,
        uint32_t reg_index_16b) {
    set_thread_id();
    m_tensix->SETDMAREG(payload_sig_sel_size, payload_sig_sel, set_signals_mode, reg_index_16b);
}

void Thread::ADDDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->ADDDMAREG(op_bis_const, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::SUBDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->SUBDMAREG(op_bis_const, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::MULDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->MULDMAREG(op_bis_const, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::BITWOPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->BITWOPDMAREG(op_bis_const, op_sel, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::SHIFTDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->SHIFTDMAREG(op_bis_const, op_sel, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::CMPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index) {
    set_thread_id();
    m_tensix->CMPDMAREG(op_bis_const, op_sel, result_reg_index, op_breg_index, op_areg_index);
}

void Thread::ATINCGET(
        uint32_t mem_hier_sel,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->ATINCGET(mem_hier_sel, wrap_val, sel_32b, data_reg_index, addr_reg_index);
}

void Thread::ATINCGETPTR(
        uint32_t mem_hier_sel,
        uint32_t no_incr,
        uint32_t incr_val,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->ATINCGETPTR(
        mem_hier_sel, 
        no_incr, 
        incr_val, 
        wrap_val, 
        sel_32b, 
        data_reg_index, 
        addr_reg_index);
}

void Thread::ATSWAP(
        uint32_t mem_hier_sel,
        uint32_t swap_mask,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->ATSWAP(mem_hier_sel, swap_mask, data_reg_index, addr_reg_index);
}

void Thread::ATCAS(
        uint32_t mem_hier_sel,
        uint32_t swap_val,
        uint32_t cmp_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->ATCAS(mem_hier_sel, swap_val, cmp_val, sel_32b, data_reg_index, addr_reg_index);
}

void Thread::REG2FLOP(
        uint32_t size_sel,
        uint32_t target_sel,
        uint32_t byte_offset,
        uint32_t context_id_2,
        uint32_t flop_index,
        uint32_t reg_index) {
    set_thread_id();
    m_tensix->REG2FLOP(size_sel, target_sel, byte_offset, context_id_2, flop_index, reg_index);
}

void Thread::LOADIND(
        uint32_t size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->LOADIND(size_sel, offset_index, auto_inc_spec, data_reg_index, addr_reg_index);
}

void Thread::STOREIND(
        uint32_t mem_hier_sel,
        uint32_t size_sel,
        uint32_t reg_size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index) {
    set_thread_id();
    m_tensix->STOREIND(
        mem_hier_sel, 
        size_sel, 
        reg_size_sel, 
        offset_index, 
        auto_inc_spec, 
        data_reg_index, 
        addr_reg_index);
}

void Thread::LOADREG(uint32_t tdma_data_reg_index, uint32_t reg_addr) {
    set_thread_id();
    m_tensix->LOADREG(tdma_data_reg_index, reg_addr);
}

void Thread::STOREREG(uint32_t tdma_data_reg_index, uint32_t reg_addr) {
    set_thread_id();
    m_tensix->STOREREG(tdma_data_reg_index, reg_addr);
}

void Thread::FLUSHDMA(uint32_t flush_spec) {
    set_thread_id();
    m_tensix->FLUSHDMA(flush_spec);
}

void Thread::DMANOP() {
    set_thread_id();
    m_tensix->DMANOP();
}

void Thread::TBUFCMD() {
    set_thread_id();
    m_tensix->TBUFCMD();
}

// SFPU

void Thread::SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_tensix->SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void Thread::SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) {
    m_tensix->SFPLOADI(lreg_ind, instr_mod0, imm16);
}

void Thread::SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_tensix->SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void Thread::SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) {
    m_tensix->SFPLUT(lreg_ind, instr_mod0, dest_reg_addr);
}

void Thread::SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPMULI(imm16_math, lreg_dest, instr_mod1);
}

void Thread::SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPADDI(imm16_math, lreg_dest, instr_mod1);
}

void Thread::SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void Thread::SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPNOP() {
    m_tensix->SFPNOP();
}

void Thread::SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPCAST(lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPCONFIG(imm16_math, config_dest, instr_mod1);
}

void Thread::SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_tensix->SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void Thread::SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_tensix->SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void Thread::SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) {
    m_tensix->SFPLUTFP32(lreg_dest, instr_mod1);
}

// CFG

void Thread::SETC16(uint32_t setc16_reg, uint32_t setc16_value) {
    set_thread_id();
    m_tensix->SETC16(setc16_reg, setc16_value);
}

void Thread::WRCFG(
        uint32_t gpr_address,
        uint32_t wr_128b,
        uint32_t cfg_reg) {
    set_thread_id();
    m_tensix->WRCFG(gpr_address, wr_128b, cfg_reg);
}

void Thread::RDCFG(uint32_t gpr_address, uint32_t cfg_reg) {
    set_thread_id();
    m_tensix->RDCFG(gpr_address, cfg_reg);
}

void Thread::RMWCIB0(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr) {
    set_thread_id();
    m_tensix->RMWCIB0(mask, data, cfg_reg_addr);
}

void Thread::RMWCIB1(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr) {
    set_thread_id();
    m_tensix->RMWCIB1(mask, data, cfg_reg_addr);
}

void Thread::RMWCIB2(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr) {
    set_thread_id();
    m_tensix->RMWCIB2(mask, data, cfg_reg_addr);
}

void Thread::RMWCIB3(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr) {
    set_thread_id();
    m_tensix->RMWCIB3(mask, data, cfg_reg_addr);
}

// Macro-ops

namespace {

void run_templ_op(TemplateOp op) {
    if (op != nullptr) {
        op();
    }
}

} // namespace

//
//    MOP: Macro-Op. Run template programmed into the MOP decoder.
//        This instruction should never reach Tensix itself, but can only
//        be pushed in through C kernels into MOP decoder.
//
//    mop_type      0 - unpack z-mask loop, 1 - double math/unpack loop 
//    loop_count    (loop count - 1) for unpacker loop MOP operation
//    zmask_lo16    Low 16-bit zmask for unpacker loop 
//

void Thread::MOP(
        uint32_t mop_type,
        uint32_t loop_count,
        uint32_t zmask_lo16) {
    assert(mop_type == m_mop_type);
    if (mop_type == 0) {
        run_mop_0(loop_count, zmask_lo16);
    } else if (mop_type == 1) {
        run_mop_1();
    } else {
        assert(false);
    }
}

void Thread::MOP_CFG(uint32_t zmask_hi16) {
    // TODO
}

//
//    REPLAY: Used to load or run instructions in the replay buffer.
//        This instruction should never reach Tensix itself.
//
//    start_idx                replay_buffer[start_idx +: len] are targeted by this replay command.
//                             The hardware will only use the log2(replay_buffer_depth) LSBs
//                             of this field.
//    len                      Indicates how many replay buffer instructions
//                             are targeted by this replay command. The hardware will only
//                             use the log2(replay_buffer_depth) LSBs of this field.
//    execute_while_loading    When load_mode == 0, this bit has no effect.
//                             When load_mode == 1:
//                                 - When this bit is 1, instructions that are loaded into
//                                   the replay buffer are also executed by the Tensix core
//                                   (takes as many cycles as it would normally take to execute
//                                   the loaded Tensix instructions)
//                                 - When this bit is 0, instructions are only loaded into
//                                   the replay buffer (guaranteed to take only one cycle
//                                   per loaded Tensix instruction) 
//    load_mode                If 1, the next len instructions will be saved to 
//                             replay_buffer[start_idx +: len] AND WILL NOT REACH THE TENSIX CORE.
//                             If 0, this instruction will instead issue the instructions
//                             saved in replay_buffer[start_idx +: len] to the tensix core.
//

void Thread::REPLAY(
        uint32_t start_idx,
        uint32_t len,
        uint32_t execute_while_loading,
        uint32_t load_mode) {
    assert(load_mode == 0 || load_mode == 1);
    uint32_t mask = (1 << m_log2_replay_buffer_depth) - 1;
    start_idx &= mask;
    len &= mask;
    assert(size_t(start_idx + len) <= m_replay_buffer.size());
    if (load_mode == 0) {
        for (uint32_t i = 0; i < len; i++) {
            run_templ_op(m_replay_buffer[start_idx + i]);
        }
    } else {
        m_replay_index = start_idx;
        m_replay_left = len;
        m_replay_execute_while_loading = execute_while_loading;
    }
}

void Thread::set_mop_templ_0(const MopTemplate0 &templ) {
    m_mop_type = 0;
    m_mop_templ_0 = templ;
}

void Thread::set_mop_templ_1(const MopTemplate1 &templ) {
    m_mop_type = 1;
    m_mop_templ_1 = templ;
}

void Thread::run_op(TemplateOp op) {
    if (m_replay_left == 0) {
        run_templ_op(op);
    } else {
        if (m_replay_execute_while_loading) {
            run_templ_op(op);
        }
        m_replay_buffer[m_replay_index] = op;
        m_replay_index++;
        m_replay_left--;
    }
}

void Thread::run_mop_0(uint32_t loop_count, uint32_t zmask_lo16) {
    MopTemplate0 &templ = m_mop_templ_0;
    // assume loop_count < 16 ?
    uint32_t mask = 1;
    for (uint32_t i = 0; i <= loop_count; i++) {
#if 0 // TODO: Figure out correct interpretation of zmask
        if ((mask & zmask_lo16) != 0) {
#endif
        if ((mask & zmask_lo16) == 0) {
            run_templ_op(templ.a0_instr);
            if (templ.unpack_halo) {
                run_templ_op(templ.a1_instr);
                run_templ_op(templ.a2_instr);
                run_templ_op(templ.a3_instr);
            }
            if (templ.unpack_b) {
                run_templ_op(templ.b_instr);
            }
        } else {
            run_templ_op(templ.skip_a_instr);
            if (templ.unpack_b) {
                run_templ_op(templ.skip_b_instr);
            }
        }
        mask <<= 1;
    }
}

void Thread::run_mop_1() {
    MopTemplate1 &templ = m_mop_templ_1;
    TemplateOp loop_op0 = templ.loop_op0;
    TemplateOp loop_op1 = templ.loop_op1;
    if (loop_op1 == nullptr) {
        loop_op1 = loop_op0;
        loop_op0 = nullptr;
    }
    TemplateOp last_inner_op = templ.last_inner_op;
    if (last_inner_op == nullptr) {
        last_inner_op = loop_op1;
    }
    TemplateOp last_outer_op = templ.last_outer_op;
    if (last_outer_op == nullptr) {
        last_outer_op = last_inner_op;
    }
    for (uint32_t i = 0; i < templ.loop_outer; i++) {
        run_templ_op(templ.start_op);
        for (uint32_t k = 0; k < templ.loop_inner; k++) {
            run_templ_op(loop_op0);
            if (k + 1 < templ.loop_inner) {
                run_templ_op(loop_op1);
            } else if (i + 1 < templ.loop_outer) {
                run_templ_op(last_inner_op);
            } else {
                run_templ_op(last_outer_op);
            }
        }
        run_templ_op(templ.end_op0);
        run_templ_op(templ.end_op1);
    }
}

// SYNC primitives

uint32_t Thread::read_sem(uint32_t sem_index) {
    return m_tensix->read_sem(sem_index);
}

void Thread::incr_sem(uint32_t sem_index) {
    m_tensix->incr_sem(sem_index);
}

void Thread::decr_sem(uint32_t sem_index) {
    m_tensix->decr_sem(sem_index);
}

// TDMA primitives

uint32_t Thread::read_dma_reg(uint32_t reg) {
    set_thread_id();
    return m_tensix->read_dma_reg(reg);
}

void Thread::write_dma_reg(uint32_t reg, uint32_t val) {
    set_thread_id();
    m_tensix->write_dma_reg(reg, val);
}

// CFG primitives

uint32_t Thread::read_cfg_reg(uint32_t reg) {
    set_thread_id();
    return m_tensix->read_cfg_reg(reg);
}

void Thread::write_cfg_reg(uint32_t reg, uint32_t value) {
    set_thread_id();
    m_tensix->write_cfg_reg(reg, value);
}

// CFG primitives (API)

void Thread::set_addr_mod_base(uint32_t base) {
    m_tensix->set_addr_mod_base(base);
}

void Thread::set_addr_mod(int index, const AddrMod &addr_mod) {
    m_tensix->set_addr_mod(index, addr_mod);
}

void Thread::set_addr_mod_pack(int index, const AddrModPack &addr_mod_pack) {
    m_tensix->set_addr_mod_pack(index, addr_mod_pack);
}

void Thread::set_dest_offset(uint32_t dst_index) {
    m_tensix->set_dest_offset(dst_index);
}

void Thread::set_tile_descriptor(
        uint32_t unp_id,
        DataFormat in_data_format,
        uint32_t x_dim,
        uint32_t y_dim,
        uint32_t z_dim) {
    m_tensix->set_tile_descriptor(unp_id, in_data_format, x_dim, y_dim, z_dim);
}

void Thread::set_haloize_mode(uint32_t haloize_mode) {
    m_tensix->set_haloize_mode(haloize_mode);
}

void Thread::set_tileize_mode(uint32_t tileize_mode) {
    m_tensix->set_tileize_mode(tileize_mode);
}

void Thread::set_shift_amount(uint32_t shift_amount) {
    m_tensix->set_shift_amount(shift_amount);
}

void Thread::set_tile_x_dim(uint32_t tile_x_dim) {
    m_tensix->set_tile_x_dim(tile_x_dim);
}

void Thread::set_unp_cntx_offset(uint32_t unp0, uint32_t unp1) {
    m_tensix->set_unp_cntx_offset(unp0, unp1);
}

void Thread::set_base_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr) {
    m_tensix->set_base_addr(unp_id, cntx_id, addr);
}

void Thread::set_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr) {
    m_tensix->set_offset_addr(unp_id, cntx_id, addr);
}

void Thread::incr_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t incr) {
    m_tensix->incr_offset_addr(unp_id, cntx_id, incr);
}

void Thread::set_pack_fp32_dest(bool en) {
    m_tensix->set_pack_fp32_dest(en);
}

void Thread::set_pack_config(
        DataFormat src_format, 
        DataFormat dst_format, 
        uint32_t x_dim, 
        uint32_t y_dim, 
        uint32_t z_dim) {
    m_tensix->set_pack_config(src_format, dst_format, x_dim, y_dim, z_dim);
}

void Thread::set_pack_edge_offset_mask(int index, uint32_t value) {
    m_tensix->set_pack_edge_offset_mask(index, value);
}

void Thread::set_pack_edge_row_set_select(int index, uint32_t value) {
    m_tensix->set_pack_edge_row_set_select(index, value);
}

void Thread::set_tile_row_set_mapping(int index, uint32_t value) {
    m_tensix->set_tile_row_set_mapping(index, value);
}

void Thread::set_relu_config(uint32_t relu_type, uint32_t relu_threshold) {
    m_tensix->set_relu_config(relu_type, relu_threshold);
}

void Thread::set_pack_dest_offset(
        uint32_t offset0, 
        uint32_t offset1, 
        uint32_t offset2, 
        uint32_t offset3) {
    m_tensix->set_pack_dest_offset(offset0, offset1, offset2, offset3);
}

void Thread::set_pack_dest_addr(uint32_t addr) {
    m_tensix->set_pack_dest_addr(addr);
}

void Thread::set_pack_l1_acc(bool en) {
    m_tensix->set_pack_l1_acc(en);
}

//
//    Semaphore primirives
//

void Thread::wait_sem_math_pack(uint32_t wait_sem_cond) {
    assert(wait_sem_cond == p_stall::STALL_ON_ZERO || wait_sem_cond == p_stall::STALL_ON_MAX);
    Tensix *tensix = m_tensix;
    uint32_t value = (wait_sem_cond == p_stall::STALL_ON_ZERO) ? 0 : 1;
    m_thread_sync->wait([tensix, value]() -> bool {
        return (tensix->get_math_pack() != value);
    });
}

void Thread::get_sem_math_pack() {
    m_tensix->set_math_pack(0);
}

void Thread::post_sem_math_pack() {
    m_tensix->set_math_pack(1);
}

//
//    Utility methods
//

uint32_t Thread::get_df_item_size(DataFormat df) {
    DataFormat base_df = DataFormat(uint32_t(df) & 0x3);
    return (base_df == DataFormat::Float32) ? 4 : 
        (base_df == DataFormat::Float16) ? 2 : 1;
}

uint32_t Thread::get_l1_tile_size(DataFormat df) {
    // size in 16B words
    return (1024 * get_df_item_size(df)) >> 4; 
}

//
//    Diagnostics
//

void Thread::diag_dump_dest(uint32_t row, uint32_t count) {
    printf("# Dest[%d : %d]\n", int(row), int(count));
    float *ptr = m_tensix->dest_row_data(row);
    for (uint32_t i = 0; i < 2 * count; i++) {
        printf("[%3d]", int(i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %g", ptr[i * 8 + k]);
        }
        printf("\n");
    }
}

void Thread::diag_dump_lreg_int(const char *label, uint32_t lreg) {
    uint32_t buf[32];
    m_tensix->get_lreg_int(lreg, buf);
    printf("# [%s] Lreg[%d].int\n", label, int(lreg));
    for (uint32_t i = 0; i < 4; i++) {
        printf("[%3d]", int(i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %d", buf[i * 8 + k]);
        }
        printf("\n");
    }
}

void Thread::diag_dump_lreg_hex(const char *label, uint32_t lreg) {
    uint32_t buf[32];
    m_tensix->get_lreg_int(lreg, buf);
    printf("# [%s] Lreg[%d].hex\n", label, int(lreg));
    for (uint32_t i = 0; i < 4; i++) {
        printf("[%3d]", int(i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %x", buf[i * 8 + k]);
        }
        printf("\n");
    }
}

void Thread::diag_dump_lreg_float(const char *label, uint32_t lreg) {
    float buf[32];
    m_tensix->get_lreg_float(lreg, buf);
    printf("# [%s] Lreg[%d].float\n", label, int(lreg));
    for (uint32_t i = 0; i < 4; i++) {
        printf("[%3d]", int(i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %g", buf[i * 8 + k]);
        }
        printf("\n");
    }
}

//
//    Private helpers
//

void Thread::set_thread_id() {
    m_tensix->set_thread_id(uint32_t(m_thread_type));
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

