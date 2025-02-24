// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>
#include <algorithm>

#include "core/addr_mod.hpp"
#include "core/instr_params.hpp"
#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

using core::p_elwise;

namespace {

void fill_reg_rows(float *reg, uint32_t row_size, uint32_t row_count, float value) {
    uint32_t count = row_size * row_count;
    for (uint32_t i = 0; i < count; i++) {
        reg[i] = value;
    }
}

void fill_row(float *dst, float value, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        dst[i] = value;
    }    
}

void copy_row(float *dst, const float *src, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

struct AddOp {
    inline float operator()(float x, float y) const {
        return x + y;
    }
};

struct SubOp {
    inline float operator()(float x, float y) const {
        return x - y;
    }
};

struct MulOp {
    inline float operator()(float x, float y) const {
        return x * y;
    }
};

// 8 rows per eltwise op at a time
static constexpr uint32_t ROW_SIZE = 16;
static constexpr uint32_t ROW_COUNT = 8;
static constexpr uint32_t ROW_MASK = 0xf;

template<typename OP> 
void elwop_bcast_none(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] = op(srca[i], srcb[i]);
    }
}

template<typename OP> 
void elwop_bcast_none_acc(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] += op(srca[i], srcb[i]);
    }
}

template<typename OP> 
void elwop_bcast_col(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] = op(srca[i], srcb[i & ~ROW_MASK]);
    }
}

template<typename OP> 
void elwop_bcast_col_acc(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] += op(srca[i], srcb[i & ~ROW_MASK]);
    }
}

template<typename OP> 
void elwop_bcast_row(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] = op(srca[i], srcb[i & ROW_MASK]);
    }
}

template<typename OP> 
void elwop_bcast_row_acc(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] += op(srca[i], srcb[i & ROW_MASK]);
    }
}

template<typename OP> 
void elwop_bcast_all(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] = op(srca[i], srcb[0]);
    }
}

template<typename OP> 
void elwop_bcast_all_acc(const OP &op, const float *srca, const float *srcb, float *dest) {
    for (uint32_t i = 0; i < 8 * ROW_SIZE; i++) {
        dest[i] += op(srca[i], srcb[0]);
    }
}

template<typename OP> 
void elwop(
        const OP &op, 
        const float *srca, 
        const float *srcb, 
        float *dest,
        uint32_t dest_accum_en,
        uint32_t instr_mod19) {
    assert(dest_accum_en == p_elwise::DEST_ACCUM_EN || dest_accum_en == p_elwise::DEST_ACCUM_DIS);
    bool acc = (dest_accum_en == p_elwise::DEST_ACCUM_EN);
    if (instr_mod19 == p_elwise::SRCB_NO_BCAST) {
        if (acc) {
            elwop_bcast_none_acc(op, srca, srcb, dest);
        } else {
            elwop_bcast_none(op, srca, srcb, dest);
        }
    } else if (instr_mod19 == p_elwise::SRCB_BCAST_COL) {
        if (acc) {
            elwop_bcast_col_acc(op, srca, srcb, dest);
        } else {
            elwop_bcast_col(op, srca, srcb, dest);
        }
    } else if (instr_mod19 == p_elwise::SRCB_BCAST_ROW) {
        if (acc) {
            elwop_bcast_row_acc(op, srca, srcb, dest);
        } else {
            elwop_bcast_row(op, srca, srcb, dest);
        }
    } else if (instr_mod19 == p_elwise::SRCB_BCAST_ALL) {
        if (acc) {
            elwop_bcast_all_acc(op, srca, srcb, dest);
        } else {
            elwop_bcast_all(op, srca, srcb, dest);
        }
    } else {
        assert(false);
    }
}

void matmul(const float *srca, const float *srcb, float *dest, uint32_t mode) {
    int row_count = (mode == 0) ? 8 : 1;
    for (int i = 0; i < row_count; i++) {
        for (int k = 0; k < 16; k++) {
            float acc = 0.0f;
            for (int p = 0; p < 16; p++) {
                acc += srcb[i * 16 + p] * srca[p * 16 + k];
            }
            // TODO: Figure out how accumulation is controlled
            dest[i * 16 + k] += acc;
        }
    }
}

void math_update_rwc(
        uint32_t incr,
        uint32_t clr,
        uint32_t cr,
        uint32_t c_to_cr,
        uint32_t &rwc,
        uint32_t &rwc_cr) {
    if (c_to_cr != 0) {
        // special case, Dest only
        // update Counter
        if (clr != 0) {
            rwc = rwc_cr;
        }
        rwc += incr;
        // also reset Counter_CR
        rwc_cr = rwc;
        // done, ignore 'cr' field
        return;
    }

    // common case
    if (cr == 0) {
        // update Counter only
        if (clr != 0) {
            rwc = rwc_cr;
        }
        rwc += incr;
    } else {
        // update Counter_CR
        if (clr != 0) {
            rwc_cr = 0;
        }
        rwc_cr += incr;
        // also reset Counter
        rwc = rwc_cr;
    }
}

} // namespace

//
//    Tensix
//

//
//    ZEROACC: Zero out accumulator.
//
//    Single mode clears one register and updates counters using the addressing mode.
//    For other modes, zero-flags are set to emulate clearing of the dest memory,
//    and dest_index is used to determine which portion is to be cleared.
//
//    clear_mode    Clear modes
//                      when full dest is available (16-bit data in dest)
//                          p_zeroacc::CLR_SPECIFIC
//                              clear only specified dest register
//                          p_zeroacc::CLR_16
//                              dest_index specifies block of 16 registers to clear
//                          p_zeroacc::CLR_HALF
//                              dest_index specifies upper or lower half to clear
//                          p_zeroacc::CLR_ALL
//                              clear all registers
//                      when half dest is available (32-bit data in dest)
//                          p_zeroacc::CLR_SPECIFIC_32B
//                              clear only specified dest register
//                          p_zeroacc::CLR_16_32B
//                              dest_index specifies block of 16 registers to clear
//                          p_zeroacc::CLR_HALF_32B
//                              dest_index specifies upper or lower half to clear
//                          p_zeroacc::CLR_ALL_32B
//                              clear all registers
//    addr_mode     Addressing mode OR bit3 = set zero flag to zero mode
//                      (TODO: Clarify what this means)
//    dst           Dest register index 
//

void Tensix::ZEROACC(
        uint32_t clear_mode,
        uint32_t addr_mode,
        uint32_t dst) {
    switch (clear_mode) {
    case p_zeroacc::CLR_SPECIFIC:
        assert(dst < DEST_ROWS);
        fill_reg_rows(m_dest[dst], ROW_SIZE, 1, 0.0f); 
        break;
    case p_zeroacc::CLR_16:
        assert(dst < DEST_ROWS / 16);
        fill_reg_rows(m_dest[dst * 16], ROW_SIZE, 16, 0.0f); 
        break;
    case p_zeroacc::CLR_HALF:
        assert(dst == 0 || dst == 1);
        fill_reg_rows(m_dest[dst * (DEST_ROWS / 2)], ROW_SIZE, DEST_ROWS / 2, 0.0f);
        break;
    case p_zeroacc::CLR_ALL:
        fill_reg_rows(m_dest[dst], ROW_SIZE, DEST_ROWS, 0.0f); 
        break;
    case p_zeroacc::CLR_SPECIFIC_32B:
        assert(dst < DEST_ROWS / 2);
        fill_reg_rows(m_dest[dst], ROW_SIZE, 1, 0.0f); 
        break;
    case p_zeroacc::CLR_16_32B:
        assert(dst < DEST_ROWS / 32);
        fill_reg_rows(m_dest[dst * 16], ROW_SIZE, 16, 0.0f); 
        break;
    case p_zeroacc::CLR_HALF_32B:
        assert(dst == 0 || dst == 1);
        fill_reg_rows(m_dest[dst * (DEST_ROWS / 2)], ROW_SIZE, DEST_ROWS / 2, 0.0f);
        break;
    case p_zeroacc::CLR_ALL_32B:
        fill_reg_rows(m_dest[dst], ROW_SIZE, DEST_ROWS / 2, 0.0f); 
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
}

//
//    ZEROSRC: Zero out source A or B registers
//
//    zero_val      Zero value:
//                      0 - initialize registers with zero
//                      1 - initialize registers with -inf (all ones) 
//    write_mode    Write mode:
//                      1 - current bank is the current read bank (i.e. math kernel)
//                      0 - current bank is the current write bank (i.e. unpacker kernel) 
//    bank_mask     Bank selection mask:
//                      0 - zero out only current bank
//                      1 - zero out all banks
//    src_mask      Source register selection mask:
//                      p_zerosrc::CLR_A
//                      p_zerosrc::CLR_B
//                      p_serosrc::CLR_AB
//

void Tensix::ZEROSRC(
        uint32_t zero_val,
        uint32_t write_mode,
        uint32_t bank_mask,
        uint32_t src_mask) {
    float value = (zero_val == 1) ? m_neginf : 0.0f;
    // SrcA / SrcB banking is not simulated - ignore 'write_mode' and 'bank_mask'
    if ((src_mask & p_zerosrc::CLR_A) != 0) {
        fill_reg_rows(m_srca[0], ROW_SIZE, SRCA_ROWS, value);
    }
    if ((src_mask & p_zerosrc::CLR_B) != 0) {
        fill_reg_rows(m_srcb[0], ROW_SIZE, SRCB_ROWS, value);
    }
}

//
//    MOVD2A: Move Dest register to SrcA.
//
//    dest_32b_lo    address dest in 32-bit mode, but use the ext bits (Lower bits) (1-bit) 
//    src            SrcA address spec 
//    addr_mode      2-bit index of cfg register ADDR_MOD_REG to use for describing addressing mode
//    instr_mod      instruction modifier:
//                       p_movd2a::MOV_1_ROW     move single row
//                       p_movd2a::MOV_4_ROWS    move 4 rows
//    dst            Dest address spec
//

void Tensix::MOVD2A(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    // dest_32b_lo is not supported (TODO: Clarify semantics)
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    float *p_srca = m_srca[src + m_srca_rwc];
    switch (instr_mod) {
    case p_movd2a::MOV_1_ROW:
        copy_row(p_srca, p_dest, ROW_SIZE);
        break;
    case p_movd2a::MOV_4_ROWS:
        for (uint32_t i = 0; i < 4; i++) {
            copy_row(p_srca + i * ROW_SIZE, p_dest + i * ROW_SIZE, ROW_SIZE);
        }
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
    m_srca_dvalid = true;
}

//
//    MOVD2B: Move Dest register to SrcB.
//
//    dest_32b_lo    address dest in 32-bit mode, but use the ext bits (Lower bits) (1-bit) 
//    src            SrcB address spec 
//    addr_mode      2-bit index of cfg register ADDR_MOD_REG to use for describing addressing mode
//    instr_mod      instruction modifier:
//                       p_movd2b::MOV_1_ROW     move single row
//                       p_movd2b::MOV_4_ROWS    move 4 rows
//    dst            Dest address spec
//

void Tensix::MOVD2B(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    // dest_32b_lo is not supported (TODO: Clarify semantics)
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    float *p_srcb = m_srcb[src + m_srcb_rwc];
    switch (instr_mod) {
    case p_movd2b::MOV_1_ROW:
        copy_row(p_srcb, p_dest, ROW_SIZE);
        break;
    case p_movd2b::MOV_4_ROWS:
        for (uint32_t i = 0; i < 4; i++) {
            copy_row(p_srcb + i * ROW_SIZE, p_dest + i * ROW_SIZE, ROW_SIZE);
        }
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
    m_srcb_dvalid = true;
}

//
//    MOVA2D: Move SrcA register to Dest
//
//    dest_32b_lo    Address dest in 32-bit mode, but use the ext bits (lower bits) (1-bit) 
//    src            SrcA address spec
//    addr_mode      2-bit index of cfg register ADDR_MOD_REG to use for describing addressing mode 
//    instr_mod      Instruction modifier:
//                       p_mova2d::MOV_1_ROW
//                       p_mova2d::MOV_8_ROWS
//    dst            Dest address spec 
//

void Tensix::MOVA2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    // dest_32b_lo is not supported (TODO: Clarify semantics)
    float *p_srca = m_srca[src + m_srca_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    switch (instr_mod) {
    case p_mova2d::MOV_1_ROW:
        copy_row(p_dest, p_srca, ROW_SIZE);
        break;
    case p_mova2d::MOV_8_ROWS:
        for (uint32_t i = 0; i < 8; i++) {
            copy_row(p_dest + i * ROW_SIZE, p_srca + i * ROW_SIZE, ROW_SIZE);
        }
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
}

//
//    MOVB2D: Move SrcB register to Dest
//
//    dest_32b_lo    Address dest in 32-bit mode, but use the ext bits (lower bits) (1-bit)
//    src            SrcB address spec
//    addr_mode      2-bit index of cfg register ADDR_MOD_REG
//                   to use for describing addressing mode
//    instr_mod      Instruction modifier (TODO: Provide better description of values):
//                       p_movb2d::MOV_1_ROW
//                           single row
//                       p_movb2d::MOV_1_ROW_D0_BRCST
//                           single row with broadcasted datum
//                       p_movb2d::MOV_8_ROW_BRCST
//                           broadcast single word
//                       p_movb2d::MOV_8_ROW_BRCST_D0_BRCST
//                           broadcast single word with broadcasted datum
//                       p_movb2d::MOV_4_ROWS
//                           multiple rows
//                       p_movb2d::MOV_4_ROWS_D0_BRCST
//                           multiple rows with broadcasted datum
//    dst            Dest address spec
//

void Tensix::MOVB2D(
        uint32_t dest_32b_lo,
        uint32_t src,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t dst) {
    // dest_32b_lo is not supported (TODO: Clarify semantics)
    float *p_srcb = m_srcb[src + m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    switch (instr_mod) {
    case p_movb2d::MOV_1_ROW:
        copy_row(p_dest, p_srcb, ROW_SIZE);
        break;
    case p_movb2d::MOV_1_ROW_D0_BRCST:
        fill_row(p_dest, p_srcb[0], ROW_SIZE);
        break;
    case p_movb2d::MOV_8_ROW_BRCST:
        for (uint32_t i = 0; i < 8; i++) {
            copy_row(p_dest + i * ROW_SIZE, p_srcb, ROW_SIZE);
        }
        break;
    case p_movb2d::MOV_8_ROW_BRCST_D0_BRCST:
        for (uint32_t i = 0; i < 8; i++) {
            fill_row(p_dest + i * ROW_SIZE, p_srcb[0], ROW_SIZE);
        }
        break;
    case p_movb2d::MOV_4_ROWS:
        for (uint32_t i = 0; i < 4; i++) {
            copy_row(p_dest + i * ROW_SIZE, p_srcb + i * ROW_SIZE, ROW_SIZE);
        }
        break;
    case p_movb2d::MOV_4_ROWS_D0_BRCST:
        for (uint32_t i = 0; i < 4; i++) {
            fill_row(p_dest + i * ROW_SIZE, p_srcb[i * ROW_SIZE], ROW_SIZE);
        }
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
}

//
//    MOVB2A: Move SrcB register to SrcA.
//
//    srca         SrcA address spec 
//    addr_mode    2-bit index of cfg register ADDR_MOD_REG to use for describing addressing mode
//    instr_mod    instruction modifier:
//                     p_movb2a::MOV_1_ROW     move single row
//                     p_movb2a::MOV_4_ROWS    move 4 rows
//    srcb         SrcB address spec 
//

void Tensix::MOVB2A(
        uint32_t srca,
        uint32_t addr_mode,
        uint32_t instr_mod,
        uint32_t srcb) {
    float *p_srcb = m_srcb[srcb + m_srcb_rwc];
    float *p_srca = m_srca[srca + m_srca_rwc];
    switch (instr_mod) {
    case p_movb2a::MOV_1_ROW:
        copy_row(p_srca, p_srcb, ROW_SIZE);
        break;
    case p_movb2a::MOV_4_ROWS:
        for (uint32_t i = 0; i < 4; i++) {
            copy_row(p_srca + i * ROW_SIZE, p_srcb + i * ROW_SIZE, ROW_SIZE);
        }
        break;
    default:
        assert(false);
        break;
    }
    math_update_counters(addr_mode);
    m_srca_dvalid = true;
}

//
//    TRNSPSRCB: transpose SrcB registers. Works on rows SrcB 16 - 31
//

void Tensix::TRNSPSRCB() {
    static_assert(ROW_SIZE == 16);
    float *p_srcb = m_srcb[16];
    for (uint32_t i = 0; i < 16; i++) {
        for (uint32_t k = 0; k < i; k++) {
            uint32_t pos1 = i * 16 + k;
            uint32_t pos2 = k * 16 + i;
            float temp = p_srcb[pos1];
            p_srcb[pos1] = p_srcb[pos2];
            p_srcb[pos2] = temp;
        }
    }
}

//
//    ELWADD: Elementwise add operation
//
//    clear_dvalid     Clear data valid bits (A, B) after math is done                 
//                         p_setrwc::CLR_NONE
//                         p_setrwc::CLR_A
//                         p_setrwc::CLR_B
//                         p_setrwc::CLR_AB
//    dest_accum_en    If zero, dest value will be overwritten with math result. 
//                     If 1, result is added to existing dest value. 
//                         p_elwise::DEST_ACCUM_DIS
//                         p_elwise::DEST_ACCUM_EN
//    instr_mod19      Instruction modifier
//                         p_elwise::SRCB_NO_BCAST
//                         p_elwise::SRCB_BCAST_COL
//                         p_elwise::SRCB_BCAST_ROW
//                         p_elwise::SRCB_CAST_ALL
//    addr_mode        2-bit index of cfg register ADDR_MOD_REG 
//                     to use for describing addressing mode 
//    dst              Dest address spec 
//

void Tensix::ELWADD(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    AddOp op;
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    elwop(op, p_srca, p_srcb, p_dest, dest_accum_en, instr_mod19);
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    ELWSUB: Elementwise subtract operation (A - B)
//

void Tensix::ELWSUB(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    SubOp op;
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    elwop(op, p_srca, p_srcb, p_dest, dest_accum_en, instr_mod19);
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    ELWMUL: Elementwise multiply operation
//

void Tensix::ELWMUL(
        uint32_t clear_dvalid,
        uint32_t dest_accum_en,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    MulOp op;
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    elwop(op, p_srca, p_srcb, p_dest, dest_accum_en, instr_mod19);
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    MVMUL: Vector-matrix multiply. Outputs 8 x 16-wide row.
//        Computes D[8, 16] = B[8, 16] * A[16, 16]
//        Hardware will pair cols of A with rows of B
//
//    clear_dvalid    Clear data valid bits (B, A) after math is done.
//                         p_setrwc::CLR_NONE
//                         p_setrwc::CLR_A
//                         p_setrwc::CLR_B
//                         p_setrwc::CLR_AB
//    instr_mod19     Instruction modifier, 0 - compute 8 rows (full BW); 1 - compute 1 row (1/8 of BW)
//    addr_mode       2-bit index of cfg register ADDR_MOD_REG to use for describing addressing mode
//    dst             Dest address spec 
//

void Tensix::MVMUL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t dst) {
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    matmul(p_srca, p_srcb, p_dest, instr_mod19);
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    GMPOOL: Global max pool with primitive size 16x16.
//
//    clear_dvalid         Clear data valid bits (B,A) after math is done:
//                             p_setrwc::CLR_NONE
//                             p_setrwc::CLR_A
//                             p_setrwc::CLR_B
//                             p_setrwc::CLR_AB
//    instr_mod19          Instruction modifier:
//                             p_gpool::DIM_1X16     single row wide
//                             p_gpool::DIM_16X16    16x16 wide
//    addr_mode            2-bit index of cfg register ADDR_MOD_REG
//                         to use for describing addressing mode
//    max_pool_index_en    Enable index computation for max pooling:
//                             p_gpool::INDEX_DIS
//                             p_gpool::INDEX_EN
//    dst                  Dest address spec
//

void Tensix::GMPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst) {
    // ACHTUNG: Unsupported operand values (semantics unclear, no use cases)
    assert(instr_mod19 != p_gpool::DIM_1X16);
    assert(max_pool_index_en != p_gpool::INDEX_EN);
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    for (uint32_t c = 0; c < ROW_SIZE; c++) {
        float scaler = p_srcb[c];
        float acc = p_dest[c];
        for (uint32_t r = 0; r < 16; r++) {
            acc = std::max(acc, p_srca[r * ROW_SIZE + c] * scaler);
        }
        p_srcb[c] = acc;
    }
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    GAPOOL: Global average pool with primitive size 16x16.
//
//    clear_dvalid         Clear data valid bits (B,A) after math is done:
//                             p_setrwc::CLR_NONE
//                             p_setrwc::CLR_A
//                             p_setrwc::CLR_B
//                             p_setrwc::CLR_AB
//    instr_mod19          Instruction modifier:
//                             p_gpool::DIM_1X16     single row wide
//                             p_gpool::DIM_16X16    16x16 wide
//    addr_mode            2-bit index of cfg register ADDR_MOD_REG
//                         to use for describing addressing mode
//    max_pool_index_en    Enable index computation for max pooling:
//                             p_gpool::INDEX_DIS
//                             p_gpool::INDEX_EN
//    dst                  Dest address spec
//

void Tensix::GAPOOL(
        uint32_t clear_dvalid,
        uint32_t instr_mod19,
        uint32_t addr_mode,
        uint32_t max_pool_index_en,
        uint32_t dst) {
    // ACHTUNG: Unsupported operand values (semantics unclear, no use cases)
    assert(instr_mod19 != p_gpool::DIM_1X16);
    assert(max_pool_index_en != p_gpool::INDEX_EN);
    float *p_srca = m_srca[m_srca_rwc];
    float *p_srcb = m_srcb[m_srcb_rwc];
    float *p_dest = m_dest[m_dest_offset + dst + m_dest_rwc];
    for (uint32_t c = 0; c < ROW_SIZE; c++) {
        float scaler = p_srcb[c];
        float acc = p_dest[c];
        for (uint32_t r = 0; r < 16; r++) {
            acc += p_srca[r * ROW_SIZE + c] * scaler;
        }
        p_dest[c] = acc;
    }
    math_update_counters(addr_mode);
    math_update_dvalid(clear_dvalid);
}

//
//    GATESRCRST: Reset SrcA / SrcB gate control.
//
//    reset_srcb_gate_control    When set, the state of SrcB pipeline gating mechanism
//                               will reset to dont gate
//    reset_srca_gate_control    When set, the state of SrcA pipeline gating mechanism
//                               will reset to dont gate
//

void Tensix::GATESRCRST(uint32_t reset_srcb_gate_control, uint32_t reset_srca_gate_control) {
    // nothing to do
}

//
//    CLEARDVALID: Clear data valid bits, or reset data valid mechanism completely.
//
//    cleardvalid    Clear {B,A} data valid (2 bits):
//                             p_setrwc::CLR_NONE
//                             p_setrwc::CLR_A
//                             p_setrwc::CLR_B
//                             p_setrwc::CLR_AB
//    reset          bit 0: When set, clears all data valid bits, and reset write and read IDs to 0
//                   bit 1: Clear dvalid but dont switch bank
//

void Tensix::CLEARDVALID(uint32_t clear_dvalid, uint32_t reset) {
    // reset is ignored (switching SrcA / SrcB banks is not similated)
    math_update_dvalid(clear_dvalid);
}

//
//    SETDVALID: Sets A/B data valid for current write ID. TDMA instruction.
//
//    set_dvalid    Set {B,A} data valid (2 bits):
//                      p_setrwc::SET_A
//                      p_setrwc::SET_B
//                      p_setrwc::SET_AB
//

void Tensix::SETDVALID(uint32_t set_dvalid) {
    if ((set_dvalid & p_setrwc::SET_A) != 0) {
        m_srca_dvalid = true;
    }
    if ((set_dvalid & p_setrwc::SET_B) != 0) {
        m_srcb_dvalid = true;
    }
}

//
//    SETRWC: Set register word counters or CR counters
//
//    clear_ab_vld    clear srcA and srcB valids indicating to unpacker that
//                    new data can be written in
//                         p_setrwc::CLR_NONE
//                         p_setrwc::CLR_A
//                         p_setrwc::CLR_B
//                         p_setrwc::CLR_AB
//    rwc_cr          bit mask: apply values specified to RWC CR counters
//                    (Dest CToCRMode, Dest, SrcB, srcA)
//                         p_setrwc::CR_A
//                         p_setrwc::CR_B
//                         p_setrwc::CR_AB
//                         p_setrwc::CR_D
//                         p_setrwc::CR_AD
//                         p_setrwc::CR_BD
//                         p_setrwc::CR_ABD
//                         p_setrwc::C_TO_CR_MODE (OR-ed with any of above values)
//    rwc_d           rwc DST value
//    rwc_b           rwc B value
//    rwc_a           rwc A value 
//    bit_mask        bit mask (fidelity_phase_clear, rwc_d, rwc_b, rwc_a)
//                         p_setrwc::SET_A
//                         p_setrwc::SET_B
//                         p_setrwc::SET_AB
//                         p_setrwc::SET_D
//                         p_setrwc::SET_AD
//                         p_setrwc::SET_BD
//                         p_setrwc::SET_ABD
//                         p_setrwc::SET_F
//                         p_setrwc::SET_A_F
//                         p_setrwc::SET_B_F
//                         p_setrwc::SET_AB_F
//                         p_setrwc::SET_D_F
//                         p_setrwc::SET_AD_F
//                         p_setrwc::SET_BD_F
//                         p_setrwc::SET_ABD_F
//

void Tensix::SETRWC(
        uint32_t clear_ab_vld,
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a,
        uint32_t bit_mask) {
    bool set_a = ((bit_mask & p_setrwc::SET_A) != 0);
    bool set_b = ((bit_mask & p_setrwc::SET_B) != 0);
    bool set_d = ((bit_mask & p_setrwc::SET_D) != 0);
    bool cr_a = ((rwc_cr & p_setrwc::CR_A) != 0);
    bool cr_b = ((rwc_cr & p_setrwc::CR_B) != 0);
    bool cr_d = ((rwc_cr & p_setrwc::CR_D) != 0);
    bool c_to_cr = ((rwc_cr & p_setrwc::C_TO_CR_MODE) != 0);

    if (cr_a) {
        m_srca_rwc_cr += rwc_a;
        m_srca_rwc = m_srca_rwc_cr;
    } else if (set_a) {
        m_srca_rwc = rwc_a;
        m_srca_rwc_cr = m_srca_rwc;
    }

    if (cr_b) {
        m_srcb_rwc_cr += rwc_b;
        m_srcb_rwc = m_srcb_rwc_cr;
    } else if (set_b) {
        m_srcb_rwc = rwc_b;
        m_srcb_rwc_cr = m_srcb_rwc;
    }

    if (c_to_cr) {
        m_dest_rwc_cr = m_dest_rwc + rwc_d;
        m_dest_rwc = m_dest_rwc_cr;
    } else if (cr_d) {
        m_dest_rwc_cr += rwc_d;
        m_dest_rwc = m_dest_rwc_cr;
    } else if (set_d) {
        m_dest_rwc = rwc_d;
        m_dest_rwc_cr = m_dest_rwc;
    }

    if ((bit_mask & p_setrwc::SET_F) != 0) {
        m_fidelity = 0;
    }

    math_wrap_counters();

    math_update_dvalid(clear_ab_vld);
}

//
//    INCRWC: Increment RWC counters or CR counters
//
//    rwc_cr    apply values specified to RWC CR counters
//    rwc_d     rwc DST value 
//    rwc_b     rwc B value 
//    rwc_a     rwc A value 
//

void Tensix::INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) {
    if (rwc_cr == 0) {
        m_srca_rwc += rwc_a;
        m_srcb_rwc += rwc_b;
        m_dest_rwc += rwc_d;
    } else {
        m_srca_rwc_cr += rwc_a;
        m_srcb_rwc_cr += rwc_b;
        m_dest_rwc_cr += rwc_d;
        // TODO: Reset *_rwc to *_rwc_cr? 
    }
    math_wrap_counters();
}

// implementation

void Tensix::math_update_counters(uint32_t addr_mode) {
    assert(addr_mode >= 0 && addr_mode < 4);
    if (m_bias != 0) {
        addr_mode += 4;
    }
    AddrMod &m = m_addr_mod[addr_mode];
    auto &srca = m.srca; 
    math_update_rwc(
        srca.incr,
        srca.clr,
        srca.cr,
        0,  // c_to_cr
        m_srca_rwc,
        m_srca_rwc_cr);
    auto &srcb = m.srcb; 
    math_update_rwc(
        srcb.incr,
        srcb.clr,
        srcb.cr,
        0,  // c_to_cr
        m_srcb_rwc,
        m_srcb_rwc_cr);
    auto &dest = m.dest; 
    math_update_rwc(
        dest.incr,
        dest.clr,
        dest.cr,
        dest.c_to_cr,
        m_dest_rwc,
        m_dest_rwc_cr);
    auto &fidelity = m.fidelity;
    if (fidelity.clr != 0) {
        m_fidelity = 0;
    }
    m_fidelity += fidelity.incr;
    // wrap fidelity?
    auto &bias = m.bias;
    if (bias.clr != 0) {
        m_bias = 0;
    }
    m_bias += bias.incr;
    m_bias &= 1;
    math_wrap_counters();
}

void Tensix::math_wrap_counters() {
    m_srca_rwc %= SRCA_ROWS;
    m_srca_rwc_cr %= SRCA_ROWS;
    m_srcb_rwc %= SRCB_ROWS;
    m_srcb_rwc_cr %= SRCB_ROWS;
    m_dest_rwc %= DEST_ROWS;
    m_dest_rwc_cr %= DEST_ROWS;
}

void Tensix::math_update_dvalid(uint32_t clear_dvalid) {
    if ((clear_dvalid & p_setrwc::CLR_A) != 0) {
        m_srca_dvalid = false;
    }
    if ((clear_dvalid & p_setrwc::CLR_B) != 0) {
        m_srcb_dvalid = false;
    }
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

