#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

#include "core/tensix_types.hpp"
#include "core/addr_mod.hpp"
#include "core/memory.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

class Tensix {
public:
    Tensix(Memory *memory);
    ~Tensix();
public:
    void reset();
    Memory *memory() {
        return m_memory;
    }
    void set_thread_id(uint32_t thread_id) {
        assert(thread_id <= 2);
        m_thread_id = thread_id;
    }
public:
    // NONE
    void NOP();
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
        uint32_t wr128b, 
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
    // CFG primitives
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
public:
    // state queries
    bool is_srca_dvalid() {
        return m_srca_dvalid;
    }
    bool is_srcb_dvalid() {
        return m_srcb_dvalid;
    }
#if 1 // TODO: Remove this (debugging only)
    void set_dvalid_ab() {
        m_srca_dvalid = true;
        m_srcb_dvalid = true;
    }
#endif
public:
    uint32_t read_cfg_reg(uint32_t reg);
    void write_cfg_reg(uint32_t reg, uint32_t value);
public:
    bool is_mutex_free(uint32_t mutex_index, uint32_t thread_id);
    bool is_sem_zero(uint32_t sem_index);
    bool is_sem_max(uint32_t sem_index);
    uint32_t read_sem(uint32_t sem_index);
    void incr_sem(uint32_t sem_index);
    void decr_sem(uint32_t sem_index);
    static uint32_t decode_sem_sel(uint32_t sem_sel);
public:
    // ACHTUNG: Will be deprecated (genuine semaphore will be used instead)
    void set_math_pack(uint32_t value) {
        m_sem_math_pack = value;
    }
    uint32_t get_math_pack() {
        return m_sem_math_pack;
    }
public:
    uint32_t read_dma_reg(uint32_t reg);
    void write_dma_reg(uint32_t reg, uint32_t val);
public:
    // diagnostics methods
    float *dest_row_data(uint32_t row_index);
    void diag_dump_dest(uint32_t row_index, uint32_t row_count);
    void diag_dump_reg(uint32_t reg, uint32_t start, uint32_t count);
    void get_lreg_int(uint32_t lreg, uint32_t *data);
    void get_lreg_float(uint32_t lreg, float *data);
private:
    void math_update_counters(uint32_t addr_mode);
    void math_wrap_counters();
    void math_update_dvalid(uint32_t clear_dvalid);
private:
    void unpack_update_strides(uint32_t unp_id);
    void unpack_tileized(float *dst, uint32_t src, uint32_t count, DataFormat df);
    void unpack(float *dst, uint32_t src, uint32_t count, DataFormat df);
private:
    void pack_update_strides();
    void pack_row(
        uint32_t packer, 
        uint32_t dst, 
        const float *src, 
        uint32_t count, 
        DataFormat df);
    void pack(uint32_t dst, const float *src, uint32_t count, DataFormat df);
    void pack_em(uint32_t mask, uint32_t dst, const float *src, DataFormat df);
    void pack_update_counters(uint32_t addr_mode);
private:
    void setadc(
        uint32_t context_id,
        uint32_t counter_id,
        uint32_t dimension_index,
        uint32_t channel_index,
        uint32_t value);
private:
    void setc16(uint32_t reg, uint32_t value);
    void wrcfg8(uint32_t reg, uint32_t value, uint32_t offset);
    void wrcfg16(uint32_t reg, uint32_t value, uint32_t offset);
    void wrcfg32(uint32_t reg, uint32_t value);
    void wrcfg128(uint32_t reg, const uint32_t *value);
    void rmwcib(uint32_t reg, uint32_t offset, uint32_t mask, uint32_t value);
    void on_setc16(uint32_t reg);
    void on_wrcfg32(uint32_t reg);
    void on_wrcfg128(uint32_t reg);
    void on_wrcfg(uint32_t reg, uint32_t mask);
    void set_reg_addr_mod_ab(int index, uint32_t value);
    void set_reg_addr_mod_bias(int index, uint32_t value);
    void set_reg_addr_mod_dst(int index, uint32_t value);
    void set_reg_addr_mod_pack(int index, uint32_t value);
private:
    static uint32_t get_df_item_size(DataFormat df);
private:
    Memory *m_memory;
    uint32_t m_thread_id;
private:
    static constexpr uint32_t
        SRCA_ROWS = 64,
        SRCB_ROWS = 64,
        DEST_ROWS = 1024,
        ROW_SIZE = 16;
private:
    float m_srca[SRCA_ROWS][ROW_SIZE];
    float m_srcb[SRCB_ROWS][ROW_SIZE];
    float m_dest[DEST_ROWS][ROW_SIZE];
    bool m_srca_dvalid;
    bool m_srcb_dvalid;
private:
    uint32_t m_srca_rwc;
    uint32_t m_srca_rwc_cr;
    uint32_t m_srcb_rwc;
    uint32_t m_srcb_rwc_cr;
    uint32_t m_dest_rwc;
    uint32_t m_dest_rwc_cr;
    uint32_t m_fidelity;
    uint32_t m_bias;
private:
    AddrMod m_addr_mod[8];
    AddrModPack m_addr_mod_pack[8];
    uint32_t m_dest_offset;
    uint32_t m_haloize_mode;    // applies to SrcA only
    uint32_t m_tileize_mode;
    uint32_t m_shift_amount;
private:
    union U32 {
        float f;
        uint32_t i;
    };
    static constexpr uint32_t
        LREG_COUNT = 16,
        LROW_SIZE = 32,
        CC_STACK_SIZE = 8;
private:
    U32 m_lreg[LREG_COUNT][LROW_SIZE];
    uint32_t m_cc;
    uint32_t m_cc_en;
    uint32_t m_cc_stack[CC_STACK_SIZE];
    uint32_t m_cc_en_stack[CC_STACK_SIZE];
    uint32_t m_cc_sp;
private:
    static constexpr uint32_t DMA_REG_COUNT = 64;
private:
    uint32_t m_dma_reg[3][DMA_REG_COUNT];
private:
    static constexpr uint32_t
        THD_REG_COUNT = 57,
        CFG_REG_COUNT = 47 * 4;
    static constexpr uint32_t
        THCON_CFG_REG_BASE = 52,
        GLOBAL_CFG_REG_BASE = 152;
private:
    // THD regs: one bank per thread
    // CFG regs: one bank per config state
    // Global CFG regs (starting at GLOBAL_CFG_REG_BASE) are replicated across config states
    // Each thread has its own config state ID (0 or 1), stored in CFG_STATE_ID_StateID CFG reg
    uint32_t m_thd_reg[3][THD_REG_COUNT];    // 16-bit
    uint32_t m_cfg_reg[2][CFG_REG_COUNT];
    uint32_t m_cfg_state_id[3];
private:
    static constexpr uint32_t MUTEX_NONE = 0xFF;
private:
    uint32_t m_mutex[8];
    uint32_t m_sem[8];
    uint32_t m_sem_max[8];
private:
    // override thread ID values: generally context-specific
    // but assumed same for all contexts in this release
    uint32_t m_tile_x_dim;
    uint32_t m_ovrd_ch0_x_stride;
    uint32_t m_ovrd_ch0_y_stride;
    uint32_t m_ovrd_ch0_z_stride;
    uint32_t m_ovrd_ch0_w_stride;
    uint32_t m_ovrd_ch1_y_stride;
    uint32_t m_ovrd_ch1_z_stride;
    uint32_t m_ovrd_ch1_w_stride;
private:
    // config context
    struct CfgCntx {
        uint32_t base_addr;
        uint32_t offset_addr;
    };
    // address controller
    struct AddrCtrl {
        // controlled by CFG registers
        DataFormat tile_in_data_format;
        uint32_t tile_x_dim;
        uint32_t tile_y_dim;
        uint32_t tile_z_dim;
        uint32_t tile_w_dim;
        uint32_t ch0_x_stride;
        uint32_t ch0_y_stride;
        uint32_t ch0_z_stride;
        uint32_t ch0_w_stride;
        // no ch1_x_stride
        uint32_t ch1_y_stride;
        uint32_t ch1_z_stride;
        uint32_t ch1_w_stride;
        CfgCntx cntx[8];
        uint32_t unp_cntx_offset;
        // controlled by SETADC* instructions
        uint32_t ch0_x;
        uint32_t ch0_y;
        uint32_t ch0_z;
        uint32_t ch0_w;
        uint32_t ch1_x;
        uint32_t ch1_y;
        uint32_t ch1_z;
        uint32_t ch1_w;
        uint32_t x_end;
        uint32_t ch0_x_cr;
        uint32_t ch0_y_cr;
        uint32_t ch0_z_cr;
        uint32_t ch0_w_cr;
        uint32_t ch1_x_cr;
        uint32_t ch1_y_cr;
        uint32_t ch1_z_cr;
        uint32_t ch1_w_cr;
        // state flags
        bool update_strides;
    };
private:
    // ACHTUNG: TPM specifies 3 TDMA thread contexts (one for each TRISC)
    //     Declaration below corresponds to only one thread context
    //     Shall it be replaced by m_adc[3][3] in the future?
    AddrCtrl m_adc[3];    // unpacker0, unpacker1, packer0
private:
    bool m_pack_fp32_dest;
    DataFormat m_pack_src_format; 
    DataFormat m_pack_dst_format;
    uint32_t m_pack_x_dim; 
    uint32_t m_pack_y_dim; 
    uint32_t m_pack_z_dim;
    // no m_pack_ch0_x_stride;
    uint32_t m_pack_ch0_y_stride;
    uint32_t m_pack_ch0_z_stride;
    uint32_t m_pack_ch0_w_stride;
    uint32_t m_pack_ch1_x_stride;
    uint32_t m_pack_ch1_y_stride;
    uint32_t m_pack_ch1_z_stride;
    uint32_t m_pack_ch1_w_stride;
    uint32_t m_pack_l1_offset;
    uint32_t m_pack_edge_offset_mask[4]; 
    uint32_t m_pack_edge_row_set_select[4];
    uint32_t m_tile_row_set_mapping[4];
    uint32_t m_relu_type;
    uint32_t m_relu_threshold;
    uint32_t m_pack_dest_offset[4];
    uint32_t m_pack_dest_addr;
    bool m_pack_l1_acc;
    bool m_pack_update_strides;
private:
    uint32_t m_sem_math_pack;
private:
    float m_neginf;
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

