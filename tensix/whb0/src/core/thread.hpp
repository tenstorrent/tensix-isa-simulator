// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>

#include "core/addr_mod.hpp"
#include "core/template.hpp"
#include "core/tensix.hpp"

#define TT_OP(instr) [=]() { instr; }

// for use with REPEAT instruction
#define TT_RUN(instr) run_op(TT_OP(instr))

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

enum class ThreadType {
    Unpack,
    Math,
    Pack
};

class ThreadSync {
public:
    ThreadSync() { }
    virtual ~ThreadSync() { }
public:
    virtual void wait(std::function<bool ()> cond) = 0;
};

class Thread {
public:
    Thread(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync);
    virtual ~Thread();
protected:
    void setup_memory_layout();
    ThreadType thread_type();
    uint8_t *map_addr(uint32_t addr);
public:
    virtual void run() = 0;
public:
    // SYNC
    void ATGETM(uint32_t mutex_index);
    void ATRELM(uint32_t mutex_index);
    void SEMINIT(
        uint32_t max_value,
        uint32_t init_value,
        uint32_t sem_sel);
    void SEMPOST(uint32_t sem_sel);
    void SEMGET(uint32_t sem_sel);
    void SEMWAIT(
        uint32_t stall_res,
        uint32_t sem_sel,
        uint32_t wait_sem_cond);
    void STALLWAIT(uint32_t stall_res, uint32_t wait_res);
public:
    // NONE
    void NOP();
public:
    // MATH
    void ZEROACC(
        uint32_t clear_mode,
        uint32_t addr_mode,
        uint32_t dst);
    void ZEROSRC(
        uint32_t zero_val,
        uint32_t write_mode,
        uint32_t bank_mask,
        uint32_t src_mask);
    void MOVD2A(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst);
    void MOVD2B(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst);
    void MOVA2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst);
    void MOVB2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst);
    void MOVB2A(
        uint32_t srca,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t srcb);
    void MOVDBGA2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst);
    void TRNSPSRCA();
    void TRNSPSRCB();
    void ELWADD(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst);
    void ELWSUB(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst);
    void ELWMUL(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst);
    void MVMUL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst);
    void GMPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst);
    void GAPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst);
    void GATESRCRST(uint32_t reset_srcb_gate_control, uint32_t reset_srca_gate_control);
    void CLEARDVALID(uint32_t clear_dvalid, uint32_t reset);
    void SETDVALID(uint32_t set_dvalid);
    void RAREB();
    void SETRWC(
        uint32_t clear_ab_vld,
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a,
        uint32_t bit_mask);
    void INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a);
    void SETIBRWC(
        uint32_t rwc_cr,
        uint32_t rwc_bias,
        uint32_t set_inc_ctrl);
    void SHIFTXA(uint32_t log2_amount2, uint32_t shift_mode);
    void SHIFTXB(
        uint32_t addr_mode,
        uint32_t rot_shift,
        uint32_t shift_row);
    void SETASHRMH0(uint32_t reg_mask, uint32_t halo_mask);
    void SETASHRMH1(uint32_t reg_mask, uint32_t halo_mask);
    void SETASHRMV(uint32_t reg_mask2);
    void SETASHRMH(uint32_t reg_mask, uint32_t halo_mask);
    void SETPKEDGOF(
        uint32_t y_end,
        uint32_t y_start,
        uint32_t x_end,
        uint32_t x_start);
    void CLREXPHIST();
    void DOTPV(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst);
    void CONV3S1(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst);
    void CONV3S2(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst);
    void MPOOL3S1(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst);
    void MPOOL3S2(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst);
    void APOOL3S1(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst);
    void APOOL3S2(
        uint32_t clear_dvalid,
        uint32_t addr_mode,
        uint32_t index_en,
        uint32_t dst);
    void MFCONV3S1(
        uint32_t clear_dvalid,
        uint32_t rotate_weights,
        uint32_t addr_mode,
        uint32_t dst);
public:
    // XMOV
    void XMOV(uint32_t mov_block_selection, uint32_t last);
public:
    // PACK
    void PACR(
        uint32_t addr_mode,
        uint32_t zero_write,
        uint32_t pack_sel,
        uint32_t ovrd_thread_id,
        uint32_t concat,
        uint32_t flush,
        uint32_t last);
    void PACR_SETREG(
        uint32_t push,
        uint32_t addr_sel,
        uint32_t wr_data,
        uint32_t pack_sel,
        uint32_t stream_id,
        uint32_t flush,
        uint32_t last);
public:
    // UNPACK
    void UNPACR(
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
        uint32_t last);
    void UNPACR_NOP(uint32_t unpack_block_selection, uint32_t no_op);
public:
    // ADDRMOD
    void SETADC(
        uint32_t cnt_set_mask,
        uint32_t channel_index,
        uint32_t dimension_index,
        uint32_t value);
    void SETADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask);
    void INCADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x);
    void ADDRCRXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask);
    void SETADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask);
    void INCADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z);
    void ADDRCRZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask);
    void SETADCXX(
        uint32_t cnt_set_mask,
        uint32_t x_end2,
        uint32_t x_start);
public:
    // TDMA
    void RSTDMA();
    void SETDMAREG(
        uint32_t payload_sig_sel_size,
        uint32_t payload_sig_sel,
        uint32_t set_signals_mode,
        uint32_t reg_index_16b);
    void ADDDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void SUBDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void MULDMAREG(
        uint32_t op_bis_const,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void BITWOPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void SHIFTDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void CMPDMAREG(
        uint32_t op_bis_const,
        uint32_t op_sel,
        uint32_t result_reg_index,
        uint32_t op_breg_index,
        uint32_t op_areg_index);
    void ATINCGET(
        uint32_t mem_hier_sel,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void ATINCGETPTR(
        uint32_t mem_hier_sel,
        uint32_t no_incr,
        uint32_t incr_val,
        uint32_t wrap_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void ATSWAP(
        uint32_t mem_hier_sel,
        uint32_t swap_mask,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void ATCAS(
        uint32_t mem_hier_sel,
        uint32_t swap_val,
        uint32_t cmp_val,
        uint32_t sel_32b,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void REG2FLOP(
        uint32_t size_sel,
        uint32_t target_sel,
        uint32_t byte_offset,
        uint32_t context_id_2,
        uint32_t flop_index,
        uint32_t reg_index);
    void LOADIND(
        uint32_t size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void STOREIND(
        uint32_t mem_hier_sel,
        uint32_t size_sel,
        uint32_t reg_size_sel,
        uint32_t offset_index,
        uint32_t auto_inc_spec,
        uint32_t data_reg_index,
        uint32_t addr_reg_index);
    void LOADREG(uint32_t tdma_data_reg_index, uint32_t reg_addr);
    void STOREREG(uint32_t tdma_data_reg_index, uint32_t reg_addr);
    void FLUSHDMA(uint32_t flush_spec);
    void DMANOP();
    void TBUFCMD();
public:
    // SFPU
    void SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr);
    void SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16);
    void SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr);
    void SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr);
    void SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPNOP();
    void SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1);
    void SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr);
    void SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1);
    void SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1);
public:
    // CFG
    void SETC16(uint32_t setc16_reg, uint32_t setc16_value);
    void WRCFG(
        uint32_t gpr_address,
        uint32_t wr_128b,
        uint32_t cfg_reg);
    void RDCFG(uint32_t gpr_address, uint32_t cfg_reg);
    void RMWCIB0(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr);
    void RMWCIB1(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr);
    void RMWCIB2(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr);
    void RMWCIB3(
        uint32_t mask,
        uint32_t data,
        uint32_t cfg_reg_addr);
public:
    // Macro-ops
    void MOP(
        uint32_t mop_type,
        uint32_t loop_count,
        uint32_t zmask_lo16);
    void MOP_CFG(uint32_t zmask_hi16);
    void REPLAY(
        uint32_t start_idx,
        uint32_t len,
        uint32_t execute_while_loading,
        uint32_t load_mode);
    void set_mop_templ_0(const MopTemplate0 &templ);
    void set_mop_templ_1(const MopTemplate1 &templ);
    void run_op(TemplateOp op);
private:
    void run_mop_0(uint32_t loop_count, uint32_t zmask_lo16);
    void run_mop_1();
public:
    // SYNC primitives
    uint32_t read_sem(uint32_t sem_index);
    void incr_sem(uint32_t sem_index);
    void decr_sem(uint32_t sem_index);
public:
    // TDMA primitives
    uint32_t read_dma_reg(uint32_t reg);
    void write_dma_reg(uint32_t reg, uint32_t val);
public:
    // CFG primitives
    uint32_t read_cfg_reg(uint32_t reg);
    void write_cfg_reg(uint32_t reg, uint32_t value);
public:
    // CFG primitives (API)
    void set_addr_mod_base(uint32_t base);
    void set_addr_mod(int index, const AddrMod &addr_mod);
    void set_addr_mod_pack(int index, const AddrModPack &addr_mod_pack);
    void set_dest_offset(uint32_t dst_index);
    void set_tile_descriptor(
        uint32_t unp_id,
        DataFormat in_data_format,
        uint32_t x_dim,
        uint32_t y_dim,
        uint32_t z_dim);
    void set_haloize_mode(uint32_t haloize_mode);
    void set_tileize_mode(uint32_t tileize_mode);
    void set_shift_amount(uint32_t shift_amount);
    void set_tile_x_dim(uint32_t tile_x_dim);
    void set_unp_cntx_offset(uint32_t unp0, uint32_t unp1);
    void set_base_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr);
    void set_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr);
    void incr_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t incr);
    void set_pack_fp32_dest(bool en);
    void set_pack_config(
        DataFormat src_format, 
        DataFormat dst_format, 
        uint32_t x_dim, 
        uint32_t y_dim, 
        uint32_t z_dim);
    void set_pack_edge_offset_mask(int index, uint32_t value); 
    void set_pack_edge_row_set_select(int index, uint32_t value);
    void set_tile_row_set_mapping(int index, uint32_t value);
    void set_relu_config(uint32_t relu_type, uint32_t relu_threshold);
    void set_pack_dest_offset(
        uint32_t offset0, 
        uint32_t offset1, 
        uint32_t offset2, 
        uint32_t offset3);
    void set_pack_dest_addr(uint32_t addr);
    void set_pack_l1_acc(bool en);
protected:
    void wait_dvalid(bool srca_vld, bool srcb_vld);
protected:
    void wait_sem_math_pack(uint32_t wait_sem_cond);
    void get_sem_math_pack();
    void post_sem_math_pack();
protected:
    static uint32_t get_df_item_size(DataFormat df);
    static uint32_t get_l1_tile_size(DataFormat df);
public:
    void diag_dump_dest(uint32_t row, uint32_t count);
    void diag_dump_lreg_int(const char *label, uint32_t lreg);
    void diag_dump_lreg_hex(const char *label, uint32_t lreg);
    void diag_dump_lreg_float(const char *label, uint32_t lreg);
protected:
    void set_thread_id();
protected:
    ThreadType m_thread_type;
    Tensix *m_tensix;
    ThreadSync *m_thread_sync;
    uint32_t m_local_base;
private:
    uint32_t m_mop_type;
    MopTemplate0 m_mop_templ_0;
    MopTemplate1 m_mop_templ_1;
    uint32_t m_log2_replay_buffer_depth;
    std::vector<TemplateOp> m_replay_buffer;
    uint32_t m_replay_index;
    uint32_t m_replay_left;
    bool m_replay_execute_while_loading;
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

