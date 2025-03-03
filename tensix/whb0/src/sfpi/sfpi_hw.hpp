// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <cassert>
#include <string>
#include <memory>

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Global constants
//

constexpr uint32_t SFP_LREG_COUNT = 8;

// WH size is 1024 rows x 16 cols by 16 bits OR 512 rows by 16 cols by 32 bits
constexpr uint32_t 
    SFP_DESTREG_STRIDE = 2,
    SFP_DESTREG_MAX_SIZE = 1024,
    SFP_DESTREG_MAX_ADDR = SFP_DESTREG_MAX_SIZE / SFP_DESTREG_STRIDE;

constexpr uint32_t 
    SFPLOAD_MOD0_FMT_SRCB = 0,
    SFPLOAD_MOD0_FMT_FP16A = 1,
    SFPLOAD_MOD0_FMT_FP16B = 2,
    SFPLOAD_MOD0_FMT_FP32 = 3,
    SFPLOAD_MOD0_FMT_INT32_TO_SM = 12,
    SFPLOAD_ADDR_MODE_NOINC = 3;

constexpr uint32_t 
    SFPSTORE_MOD0_FMT_SRCB = 0,
    SFPSTORE_MOD0_FMT_FP16A = 1,
    SFPSTORE_MOD0_FMT_FP16B = 2,
    SFPSTORE_MOD0_FMT_FP32 = 3,
    SFPSTORE_MOD0_FMT_INT32_TO_SM = 12,
    SFPSTORE_ADDR_MODE_NOINC = 3;

constexpr uint32_t SFPMOV_MOD1_COMPSIGN = 1;

constexpr uint32_t SFPMAD_MOD1_OFFSET_NONE = 0;

constexpr uint32_t 
    SFPLOADI_MOD0_FLOATB = 0,
    SFPLOADI_MOD0_FLOATA = 1,
    SFPLOADI_MOD0_USHORT = 2,
    SFPLOADI_MOD0_SHORT = 4,
    SFPLOADI_MOD0_UPPER = 8,
    SFPLOADI_MOD0_LOWER = 10,
    SFPXLOADI_MOD0_32BIT_MASK = 16,
    SFPXLOADI_MOD0_INT32 = 16,
    SFPXLOADI_MOD0_UINT32 = 17,
    SFPXLOADI_MOD0_FLOAT = 18;

constexpr uint32_t 
    SFPEXMAN_MOD1_PAD8 = 0,
    SFPEXMAN_MOD1_PAD9 = 1;

constexpr uint32_t 
    SFPEXEXP_MOD1_DEBIAS = 0,
    SFPEXEXP_MOD1_NODEBIAS = 1,
    SFPEXEXP_MOD1_SET_CC_SGN_EXP = 2,
    SFPEXEXP_MOD1_SET_CC_COMP_EXP = 8,
    SFPEXEXP_MOD1_SET_CC_SGN_COMP_EXP = 10;

constexpr uint32_t 
    SFPABS_MOD1_INT = 0,
    SFPABS_MOD1_FLOAT = 1;

constexpr uint32_t 
    SFPIADD_MOD1_ARG_LREG_DST = 0,
    SFPIADD_MOD1_ARG_IMM = 1,
    SFPIADD_MOD1_ARG_2SCOMP_LREG_DST = 2,
    SFPIADD_MOD1_CC_LT0 = 0,
    SFPIADD_MOD1_CC_NONE = 4,
    SFPIADD_MOD1_CC_GTE0 = 8;

constexpr uint32_t 
    SFPXIADD_MOD1_SIGNED = 8,
    SFPXIADD_MOD1_IS_SUB = 16,
    SFPXIADD_MOD1_12BIT = 32,
    SFPXIADD_MOD1_16BIT = 64;

constexpr uint32_t 
    SFPXCMP_MOD1_CC_NONE = 0,
    SFPXCMP_MOD1_CC_LT = 1,
    SFPXCMP_MOD1_CC_EQ = 2,
    SFPXCMP_MOD1_CC_GTE = 3,
    SFPXCMP_MOD1_CC_NE = 4,
    SFPXCMP_MOD1_CC_LTE = 5,
    SFPXCMP_MOD1_CC_GT = 6,
    SFPXCMP_MOD1_CC_MASK = 7;

constexpr uint32_t 
    SFPXSCMP_MOD1_FMT_A = 8,
    SFPXSCMP_MOD1_FMT_B = 16,
    SFPXSCMP_MOD1_FMT_FLOAT = 32,
    SFPXSCMP_MOD1_FMT_MASK = 0x38;

constexpr uint32_t 
    SFPXBOOL_MOD1_OR = 1,
    SFPXBOOL_MOD1_AND = 2,
    SFPXBOOL_MOD1_NOT = 3;

constexpr uint32_t 
    SFPSETCC_MOD1_LREG_LT0 = 0,
    SFPSETCC_MOD1_IMM_BIT0 = 1,
    SFPSETCC_MOD1_LREG_NE0 = 2,
    SFPSETCC_MOD1_LREG_GTE0 = 4,
    SFPSETCC_MOD1_LREG_EQ0 = 6,
    SFPSETCC_MOD1_COMP = 8;

// EU: enable unmodified, EC: complement, EI: immediate
// R1: result set, RI: immediate
constexpr uint32_t 
    SFPENCC_IMM12_NEITHER = 0,    // Imm value to clear both enable/result
    SFPENCC_IMM12_BOTH = 3;       // Imm value to set both enable/result

constexpr uint32_t 
    SFPENCC_MOD1_EU_R1 = 0,
    SFPENCC_MOD1_EC_R1 = 1,
    SFPENCC_MOD1_EI_R1 = 2,
    SFPENCC_MOD1_EU_RI = 8,
    SFPENCC_MOD1_EC_RI = 9,
    SFPENCC_MOD1_EI_RI = 10;

constexpr uint32_t 
    SFPPUSHC_MOD1_PUSH = 0,
    SFPPUSHC_MOD1_REPLACE = 1;

constexpr uint32_t 
    SFPLZ_MOD1_CC_NONE = 0,
    SFPLZ_MOD1_CC_NE0 = 2,
    SFPLZ_MOD1_CC_COMP = 8,
    SFPLZ_MOD1_CC_EQ0 = 10,
    SFPLZ_MOD1_NOSGN_MASK = 4,
    SFPLZ_MOD1_NOSGN_CC_NONE = 4,
    SFPLZ_MOD1_NOSGN_CC_NE0 = 6,
    SFPLZ_MOD1_NOSGN_CC_COMP = 12,
    SFPLZ_MOD1_NOSGN_CC_EQ0 = 14;

constexpr uint32_t SFPSDIVP2_MOD1_ADD = 1;

constexpr uint32_t 
    SFPLUT_MOD0_SGN_UPDATE = 0,
    SFPLUT_MOD0_SGN_RETAIN = 4;

constexpr uint32_t 
    SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE = 0,
    SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 = 2,
    SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2 = 3,
    SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE = 10,
    SFPLUTFP32_MOD0_SGN_UPDATE = 0,
    SFPLUTFP32_MOD0_SGN_RETAIN = 4;

constexpr uint32_t 
    SFPCAST_MOD1_RND_EVEN = 0,
    SFPCAST_MOD1_RND_STOCH = 1;

constexpr uint32_t 
    SFPSTOCHRND_RND_EVEN = 0,
    SFPSTOCHRND_RND_STOCH = 1,
    SFPSTOCHRND_MOD1_FP32_TO_FP16A = 0,
    SFPSTOCHRND_MOD1_FP32_TO_FP16B = 1,
    SFPSTOCHRND_MOD1_FP32_TO_UINT8 = 2,
    SFPSTOCHRND_MOD1_FP32_TO_INT8 = 3,
    SFPSTOCHRND_MOD1_INT32_TO_UINT8 = 4,
    SFPSTOCHRND_MOD1_INT32_TO_INT8 = 5,
    SFPSTOCHRND_MOD1_FP32_TO_UINT16 = 6,
    SFPSTOCHRND_MOD1_FP32_TO_INT16 = 7,
    SFPSTOCHRND_MOD1_CONV_MASK = 7,
    SFPSTOCHRND_MOD1_IMM8 = 8;

constexpr uint32_t 
    SFPSHFT2_MOD1_COPY4 = 0,
    SFPSHFT2_MOD1_SUBVEC_CHAINED_COPY4 = 1,
    SFPSHFT2_MOD1_SUBVEC_SHFLROR1_AND_COPY4 = 2,
    SFPSHFT2_MOD1_SUBVEC_SHFLROR1 = 3,
    SFPSHFT2_MOD1_SUBVEC_SHFLSHR1 = 4,
    SFPSHFT2_MOD1_SHFT_IMM = 5,
    SFPSHFT2_MOD1_SHFT_LREG = 6;

constexpr uint32_t 
    SFPSWAP_MOD1_SWAP = 0,
    SFPSWAP_MOD1_VEC_MIN_MAX = 1,
    SFPSWAP_MOD1_SUBVEC_MIN01_MAX23 = 2,
    SFPSWAP_MOD1_SUBVEC_MIN02_MAX13 = 3,
    SFPSWAP_MOD1_SUBVEC_MIN03_MAX12 = 4,
    SFPSWAP_MOD1_SUBVEC_MIN0_MAX123 = 5,
    SFPSWAP_MOD1_SUBVEC_MIN1_MAX023 = 6,
    SFPSWAP_MOD1_SUBVEC_MIN2_MAX013 = 7,
    SFPSWAP_MOD1_SUBVEC_MIN3_MAX012 = 8;

constexpr uint32_t 
    SFPCONFIG_DEST_MACRO_INST0 = 0,
    SFPCONFIG_DEST_MACRO_INST1 = 1,
    SFPCONFIG_DEST_MACRO_INST2 = 2,
    SFPCONFIG_DEST_MACRO_INST3 = 3,
    SFPCONFIG_DEST_MACRO_SEQ0 = 4,
    SFPCONFIG_DEST_MACRO_SEQ1 = 5,
    SFPCONFIG_DEST_MACRO_SEQ2 = 6,
    SFPCONFIG_DEST_MACRO_SEQ3 = 7,
    SFPCONFIG_DEST_MACRO_CTRL = 8,
    SFPCONFIG_DEST_LREG11 = 11,
    SFPCONFIG_DEST_LREG12 = 12,
    SFPCONFIG_DEST_LREG13 = 13,
    SFPCONFIG_DEST_LREG14 = 14,
    SFPCONFIG_DEST_SFPU_CTRL = 15;

constexpr uint32_t SFPCONFIG_MOD1_SRC_R0_LREG0 = 0;

constexpr uint32_t 
    CREG_IDX_0P837300003 = 8,
    CREG_IDX_0 = 9,
    CREG_IDX_1 = 10,
    CREG_IDX_PRGM0 = 11,
    CREG_IDX_PRGM1 = 12,
    CREG_IDX_PRGM2 = 13,
    CREG_IDX_PRGM3 = 14,
    CREG_IDX_NEG_1 = CREG_IDX_PRGM0,
    CREG_IDX_TILEID = 15;

//
//    Hw
//

class Hw {
public:
    Hw() { }
    virtual ~Hw() { }
public:
    virtual void INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) = 0;
    virtual void SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) = 0;
    virtual void SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) = 0;
    virtual void SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) = 0;
    virtual void SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) = 0;
    virtual void SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPNOP() = 0;
    virtual void SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) = 0;
    virtual void SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) = 0;
    virtual void SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) = 0;
    virtual void SFPDIAG(const std::string &text) = 0;
};

//
//    Vec
//
     
class Vec {
public:
    Vec():
        m_val(0xffffffff) { }
    Vec(const Vec &v):
        m_val(v.m_val) { }
    explicit Vec(uint32_t val):
        m_val(val) { }
    ~Vec() { }
public:
    Vec &operator=(const Vec &v) {
        m_val = v.m_val;
        return *this;
    }
    uint32_t get() const {
        return m_val;
    }
private:
    uint32_t m_val;
};

//
//    Rvtt
//

class Rvtt {
public:
    Rvtt() { }
    virtual ~Rvtt() { }
public:
    virtual void set_hw(Hw *hw) = 0;
    virtual void reset() = 0;
    virtual void compile() = 0;
public:
    virtual void sfpincrwc(uint32_t cr, uint32_t d, uint32_t b, uint32_t a) = 0;
    virtual Vec sfpassign_lv(Vec v, Vec in) = 0;
    virtual Vec sfpload(uint32_t mod0, uint32_t mode, uint32_t addr) = 0;
    virtual Vec sfpassignlreg(int lr) = 0;
    virtual void sfppreservelreg(Vec x, int n) = 0;
    virtual Vec sfpxloadi(uint32_t mod0, uint32_t imm) = 0;
    virtual void sfpstore(Vec src, uint32_t mod0, uint32_t mode, uint32_t addr) = 0;
    virtual Vec sfpmov(Vec src, uint32_t mod1) = 0;
    virtual void sfpnop() = 0;
    virtual void sfpillegal() = 0;
    virtual void sfpencc(uint32_t imm, uint32_t mod1) = 0;
    virtual void sfppushc() = 0;
    virtual void sfppopc() = 0;
    virtual void sfpsetcc_v(Vec src, uint32_t mod1) = 0;
    virtual void sfpsetcc_i(uint32_t imm, uint32_t mod1) = 0;
    virtual int sfpxfcmps(Vec v, uint32_t f, uint32_t mod1) = 0;
    virtual int sfpxfcmpv(Vec v1, Vec v2, uint32_t mod1) = 0;
    virtual int sfpxicmps(Vec v, uint32_t i, uint32_t mod1) = 0;
    virtual int sfpxicmpv(Vec v1, Vec v2, uint32_t mod1) = 0;
    virtual int sfpxvif() = 0;
    virtual int sfpxbool(int t, int a, int b) = 0;
    virtual void sfpxcondb(int s, int i) = 0;
    virtual Vec sfpxcondi(int i) = 0;
    virtual void sfpcompc() = 0;
    virtual Vec sfpadd(Vec va, Vec vb, uint32_t mod1) = 0;
    virtual Vec sfpmul(Vec va, Vec vb, uint32_t mod1) = 0;
    virtual Vec sfpmad(Vec va, Vec vb, Vec vc, uint32_t mod1) = 0;
    virtual Vec sfpexexp(Vec src, uint32_t mod1) = 0;
    virtual Vec sfpexman(Vec src, uint32_t mod1) = 0;
    virtual Vec sfpsetexp_i(uint32_t imm, Vec src) = 0;
    virtual Vec sfpsetexp_v(Vec dst, Vec src) = 0;
    virtual Vec sfpsetman_i(uint32_t imm, Vec src, uint32_t mod) = 0;
    virtual Vec sfpsetman_v(Vec dst, Vec src) = 0;
    virtual Vec sfpabs(Vec src, uint32_t mod1) = 0;
    virtual Vec sfpand(Vec dst, Vec src) = 0;
    virtual Vec sfpor(Vec dst, Vec src) = 0;
    virtual Vec sfpxor(Vec dst, Vec src) = 0;
    virtual Vec sfpnot(Vec src) = 0;
    virtual Vec sfpdivp2(uint32_t imm, Vec src, uint32_t mod1) = 0;
    virtual Vec sfplz(Vec src, uint32_t mod1) = 0;
    virtual Vec sfpshft_i(Vec dst, uint32_t imm) = 0;
    virtual Vec sfpshft_v(Vec dst, Vec src) = 0;
    virtual Vec sfpxiadd_i(Vec src, uint32_t imm, uint32_t mod1) = 0;
    virtual Vec sfpxiadd_v(Vec dst, Vec src, uint32_t mod1) = 0;
    virtual Vec sfpsetsgn_i(uint32_t imm, Vec src) = 0;
    virtual Vec sfpsetsgn_v(Vec dst, Vec src) = 0;
    virtual Vec sfplut(Vec l0, Vec l1, Vec l2, Vec dst, uint32_t mod0) = 0;
    virtual Vec sfplutfp32_3r(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod0) = 0;
    virtual Vec sfplutfp32_6r(
        Vec l0, 
        Vec l1, 
        Vec l2, 
        Vec l4, 
        Vec l5, 
        Vec l6, 
        Vec l3, 
        uint32_t mod0) = 0;
    virtual Vec sfpcast(Vec src, uint32_t mod1) = 0;
    virtual Vec sfpstochrnd_i(uint32_t mode, uint32_t imm, Vec srcc, uint32_t mod1) = 0;
    virtual Vec sfpstochrnd_v(uint32_t mode, Vec srcb, Vec srcc, uint32_t mod1) = 0;
    virtual void sfptransp(Vec l0, Vec l1, Vec l2, Vec l3) = 0;
    virtual Vec sfpshft2_i(Vec dst, uint32_t imm) = 0;
    virtual Vec sfpshft2_v(Vec dst, Vec src) = 0;
    virtual void sfpshft2_g(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod) = 0;
    virtual void sfpshft2_ge(Vec src, Vec l0, Vec l1, Vec l2, Vec l3) = 0;
    virtual Vec sfpshft2_e(Vec src, uint32_t mod) = 0;
    virtual void sfpswap(Vec dst, Vec src, uint32_t mod) = 0;
    virtual void sfpconfig_v(Vec l0, uint32_t config_dest) = 0;
public:
    virtual void print_text(const char *fmt, ...) = 0;
};

extern std::unique_ptr<Rvtt> rvtt;

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

