// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>
#include <cmath>

#ifdef _MSC_VER
#include <intrin.h>
#endif

#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

namespace {

union U32 {
    float f;
    uint32_t i;
};

float u16_as_fp16a(uint32_t x) {
    // float16
    // fp16a: [15] sign [14:10] exponent (bias 15) [9:0] mantissa
    uint32_t m = x & 0x03ff;
    uint32_t e = (x & 0x7c00) >> 10;
    uint32_t s = (x & 0x8000) >> 15;
    // float: [31] sign [30:23] exponent (bias 127) [22:0] mantissa
    m = m << 13;
    e = (e + (127 - 15)) << 23;
    s = s << 31;
    U32 u32;
    u32.i = s | e | m;
    return u32.f;
}

float u16_as_fp16b(uint32_t x) {
    // bfloat16
    U32 u32;
    u32.i = x << 16;
    return u32.f;
}

uint32_t u16_as_u32(uint32_t x) {
    return x;
}

uint32_t u16_as_s32(uint32_t x) {
    return ((x & 0x8000) != 0) ? (x | 0xffff0000) : x;
}

template<typename OP> 
void sfpu_op(const OP &op, uint32_t cc, uint32_t cc_en) {
    static constexpr uint32_t LROW_SIZE = 32;
    if (cc_en != 0 && cc == 0) {
        return;
    }
    if (cc_en == 0 || cc == ~uint32_t(0)) {
        for (uint32_t i = 0; i < LROW_SIZE; i++) {
            op(i);
        }
    } else {
        for (uint32_t i = 0; i < LROW_SIZE; i++) {
            if ((cc & 0x1) != 0) {
                op(i);
            }
            cc >>= 1;
        }
    }
}

float lut(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3) {
    // for encoding description, see SFPLUT below 
    U32 u32;
    // float: [31] sign [30:23] exponent (bias 127) [22:0] mantissa
    // biased exponent of x3
    uint32_t e3 = (x3 >> 23) & 0xff;
    uint32_t x = (e3 < 127) ? x0 : (e3 == 127) ? x1 : x2;
    float b = 0.0f;
    if ((x & 0xff) != 0xff) {
        uint32_t s = (x >> 7) & 0x1;
        uint32_t e = (x >> 4) & 0x7;
        uint32_t m = x & 0xf;
        u32.i = (s << 31) | ((127 - e) << 23) | (m << 19);
        b = u32.f;
    }
    x >>= 8;
    float a = 0.0f;
    if ((x & 0xff) != 0xff) {
        uint32_t s = (x >> 7) & 0x1;
        uint32_t e = (x >> 4) & 0x7;
        uint32_t m = x & 0xf;
        u32.i = (s << 31) | ((127 - e) << 23) | (m << 19);
        a = u32.f;
    }
    // abs value
    u32.i = x3 & 0x7fffffff;
    return a * u32.f + b;
}

uint32_t decode_mfp16(uint32_t x) {
    // for encoding description, see SFPLUTFP32 below 
    uint32_t m = (x & 0x3ff) << 13;
    uint32_t emod = (x >> 10) & 0x1f;
    uint32_t e = 0;
    if (emod != 0x1f) {
        uint32_t emod4 = (emod >> 4);
        e |= (emod4 << 7);
        if (emod4 == 0) {
            e |= (0x7 << 4);
        }
        e |= (emod & 0xf);
    }
    uint32_t s = (x >> 15) & 0x1;
    return (s << 31) | (e << 23) | m;
}

float decode_mfp16_hi(float x) {
    U32 u32;
    u32.f = x;
    u32.i = decode_mfp16(u32.i >> 16);
    return u32.f;
}

float decode_mfp16_lo(float x) {
    U32 u32;
    u32.f = x;
    u32.i = decode_mfp16(u32.i & 0xffff);
    return u32.f;
}

float lut_fp32(
        float x0, 
        float x1, 
        float x2, 
        float x3,
        float x4, 
        float x5, 
        float x6) {
    float x = (x3 >= 0.0f) ? x3 : -x3;
    float a = 0.0f;
    float b = 0.0f;
    if (x < 1.0f) {
        a = x0;
        b = x4;
    } else if (x < 2.0f) {
        a = x1;
        b = x5;
    } else {
        a = x2;
        b = x6;
    }
    return a * x + b;
}

float lut_mfp16_6(
        float x0, 
        float x1, 
        float x2, 
        float x3,
        float x4, 
        float x5, 
        float x6,
        uint32_t mode) {
    float x = (x3 >= 0.0f) ? x3 : -x3;
    float a = 0.0f;
    float b = 0.0f;
    float y = (mode == 2) ? 4.0f : 3.0f;
    if (x < 0.5f) {
        a = decode_mfp16_lo(x0);
        b = decode_mfp16_lo(x4);
    } else if (x < 1.0f) {
        a = decode_mfp16_hi(x0);
        b = decode_mfp16_hi(x4);
    } else if (x < 1.5f) {
        a = decode_mfp16_lo(x1);
        b = decode_mfp16_lo(x5);
    } else if (x < 2.0f) {
        a = decode_mfp16_hi(x1);
        b = decode_mfp16_hi(x5);
    } else if (x < y) {
        a = decode_mfp16_lo(x2);
        b = decode_mfp16_lo(x6);
    } else {
        a = decode_mfp16_hi(x2);
        b = decode_mfp16_hi(x6);
    }
    return a * x + b;
}

float lut_mfp16_3(float x0, float x1, float x2, float x3) {
    float x = (x3 >= 0.0f) ? x3 : -x3;
    float a = 0.0f;
    float b = 0.0f;
    if (x < 1.0f) {
        a = decode_mfp16_hi(x0);
        b = decode_mfp16_lo(x0);
    } else if (x < 2.0f) {
        a = decode_mfp16_hi(x1);
        b = decode_mfp16_lo(x1);
    } else {
        a = decode_mfp16_hi(x2);
        b = decode_mfp16_lo(x2);
    }
    return a * x + b;
}

void set_cc(uint32_t &cc, uint32_t i, uint32_t bit) {
    cc &= ~(uint32_t(1) << i);
    cc |= bit << i;
}

void msb_to_cc(uint32_t &cc, uint32_t i, uint32_t z) {
    set_cc(cc, i, (z >> 31));
}

void comp_msb_to_cc(uint32_t &cc, uint32_t i, uint32_t z) {
    set_cc(cc, i, (~z >> 31));
}

void comp_cc(uint32_t &cc, uint32_t i) {
    cc ^= uint32_t(1) << i;
}

uint32_t sext_u12(uint32_t x) {
    return ((x & 0x800) == 0) ? x : (x | 0xfffff000);
}

uint32_t lz(uint32_t x) {
#if defined(_MSC_VER)
    return __lzcnt(x);
#elif defined(__GNUC__)
    if (x == 0) {
        return 32;
    }
    return __builtin_clz(x);
#else
    uint32_t n = 32;
    uint32_t t;
    t = x >> 16; 
    if (t != 0) { 
        n -= 16; 
        x = t; 
    }
    t = x >> 8; 
    if (t != 0) { 
        n -= 8; 
        x = t; 
    }
    t = x >> 4; 
    if (t != 0) { 
        n -= 4; 
        x = t; 
    }
    t = x >> 2; 
    if (t != 0) { 
        n -= 2; 
        x = t; 
    }
    t = x >> 1; 
    if (t != 0) {
        return n - 2;
    }
    return n - x;
#endif
}

float round_to_nearest_even(float x) {
    return std::nearbyint( x * 0.5f ) * 2.0f;
}

uint32_t descale_u32(uint32_t x, uint32_t n) {
    return (x + (1 << (n - 1))) >> n;
}

int32_t descale_i32(int32_t x, uint32_t n) {
    if (x >= 0) {
        return (x + (1 << (n - 1))) >> n;
    } else {
        return (x - (1 << (n - 1))) >> n;
    }
}

} // namespace

//
//    Tensix
//

//
//    SFPLOAD: Load data from Dest into LREG.
//        Specified dest_reg_addr is added to BASE, x_counter, and DEST_TARGE_REG_CFG
//        registers to compute final Dest register address.
//
//    lreg_ind          LREG to be written
//    instr_mod0        format of loaded data, read only corresponding number of bits from Dest
//                          0: default, determined by SrcB exp width of
//                                 ALU_FORMAT_SPEC_REG and ACC_CTRL_SFPU_Fp32
//                          1: fp16 (fp16_a)
//                          2: bfloat16 (fp16_b)
//                          3: fp32, 32b
//                          4: int32
//                          5: int8 (but read 16b)
//                          6: uint16
//                          7: raw 16b (HI16) to LREG[31:16], LREG[15:0] padded with 0
//                          8: int16
//                          9: raw 16b (LO16) to LREG[15:0], LREG[31:16] padded with 0
//                          10: int32 not conditional upon CC state
//                          11: reserved
//                          12: int32 2's complement format converted to sign + magnitude
//                          13: int8 2's complement format converted to sign + magnitude (16b)
//                          14: raw 16b (LO16_ONLY) to LREG[15:0], LREG[31:16] unmodified
//                          15: raw 16b (HI16_ONLY) to LREG [31:16], LREG[15:0] unmodified 
//    sfpu_addr_mode    address mode for updates to SrcA (?) and Dest (?) addresses
//    dest_reg_addr     immediate offset into Dest
//

void Tensix::SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    // NOTE: Current implementation always represents Dest contents as Float32.
    //     Therefore non-zero values of 'instr_mod0' are not supported and zero
    //     is treated as Float32. This is sufficient for current LLK implementation
    //     as it is always sets 'instr_mod0' to zero.
    assert(instr_mod0 == 0);
    if (lreg_ind >= 8) {
        return;
    }
    struct {
        U32 *lreg;
        float *dest;
        void operator()(uint32_t i) const {
            lreg[i].f = dest[i];
        }
    } op;
    op.lreg = m_lreg[lreg_ind];
    op.dest = m_dest[m_dest_offset + dest_reg_addr + m_dest_rwc];
    sfpu_op(op, m_cc, m_cc_en);
    math_update_counters(sfpu_addr_mode);
}

//
//    SFPLOADI: Load immediate value into LREG.
//
//    lreg_ind      LREG to be written
//    instr_mod0    data format
//                      0: bfloat16 (fp16_b), left justified and padded with 0 on LSBs
//                      1: fp16 (fp16_a), imm16[15] sign, imm16[14:10] exp, imm16[9:0] mantissa
//                      2: uint, right justified and padded with 0 on MSBs
//                      3: reserved
//                      4: int, right justified and padded with imm16[15] on MSBs
//                      5 - 7: reserved
//                      8: to LREG[31:16], LREG[15:0] unmodified
//                      9: reserved
//                      10: to LREG[15:0], LREG[31:16] unmodified
//                      11 - 15: reserved
//    imm16         immediate value
//

void Tensix::SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) {
    if (lreg_ind >= 8) {
        return;
    }
    imm16 &= 0xffff;
    switch (instr_mod0) {
    case 0:
    case 1:
        {
            struct {
                U32 *lreg;
                float imm;
                void operator()(uint32_t i) const {
                    lreg[i].f = imm;
                }
            } op;
            op.lreg = m_lreg[lreg_ind];
            op.imm = (instr_mod0 == 0) ? u16_as_fp16b(imm16) : u16_as_fp16a(imm16);
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
    case 4:
        {
            struct {
                U32 *lreg;
                uint32_t imm;
                void operator()(uint32_t i) const {
                    lreg[i].i = imm;
                }
            } op;
            op.lreg = m_lreg[lreg_ind];
            op.imm = (instr_mod0 == 2) ? u16_as_u32(imm16) : u16_as_s32(imm16);
            sfpu_op(op, m_cc, m_cc_en);
        }    
        break;
    case 8:
        {
            struct {
                U32 *lreg;
                uint32_t imm;
                void operator()(uint32_t i) const {
                    lreg[i].i = (lreg[i].i & 0xffff) | imm;
                }
            } op;
            op.lreg = m_lreg[lreg_ind];
            op.imm = imm16 << 16;
            sfpu_op(op, m_cc, m_cc_en);
        }    
        break;
    case 10:
        {
            struct {
                U32 *lreg;
                uint32_t imm;
                void operator()(uint32_t i) const {
                    lreg[i].i = (lreg[i].i & 0xffff0000) | imm;
                }
            } op;
            op.lreg = m_lreg[lreg_ind];
            op.imm = imm16;
            sfpu_op(op, m_cc, m_cc_en);
        }    
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSTORE: Store value from LREG into Dest.
//        Specified dest_reg_addr is added to BASE, x_counter, and DEST_TARGE_REG_CFG
//        registers to compute final Dest register address.
//
//    lreg_ind          LREG to be stored
//    instr_mod0        format of stored data, write only corresponding number of bits to Dest 
//                          0: default, determined by SrcB exp width of
//                                 ALU_FORMAT_SPEC_REG and ACC_CTRL_SFPU_Fp32
//                          1: fp16 (fp16_a), LREG exp rebased to 5b, LREG mantissa trunc to 10b
//                          2: bloat16 (fp16_b), LREG exp unmodified, LREG mantissa trunc to 7b
//                          3: fp32
//                          4: int32 (sign + magnitude), reorder LREG[22:16] with LREG[30:23]
//                          5: int8 (sign + magnitude), shift magnitude by 5b to left,
//                              set lower 5b to decimal 16
//                          6: uint16
//                          7: raw 32b (HI16), LREG passed through unmodified
//                          8: int16 (sign + magnitude)
//                          9: raw 32b (LO16), LREG[31:16] reordered with [15:0]
//                          10: int32 (sign + magnitude), reorder LREG[22:16] with LREG[30:23]
//                              not conditional on CC state
//                          11: reserved
//                          12: int32 2's complement format, convert to (sign + magnitude),
//                              reorder LREG[22:16] with LREG[30:23]
//                          13: int8 2's complement format, convert to (sign + magnitude),
//                              shift magnitude by 5b to left, set lower 5b to decimal 16
//                          14: raw 16b [LO16_ONLY], write LREG[15:0] to Dest register
//                          15: raw 16b [HI16_ONLY], write LREG[31:16] to Dest register
//    sfpu_addr_mode    address mode for updates to Src and Dest registers as side effect
//    dest_reg_addr     immediate offset into Dest register
//

void Tensix::SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    // NOTE: Current implementation always represents Dest contents as Float32.
    //     Therefore non-zero values of 'instr_mod0' are not supported and zero
    //     is treated as Float32. This is sufficient for current LLK implementation
    //     as it is always sets 'instr_mod0' to zero.
    assert(instr_mod0 == 0);
    struct {
        U32 *lreg;
        float *dest;
        void operator()(uint32_t i) const {
            dest[i] = lreg[i].f;
        }
    } op;
    op.lreg = m_lreg[lreg_ind];
    op.dest = m_dest[m_dest_offset + dest_reg_addr + m_dest_rwc];
    sfpu_op(op, m_cc, m_cc_en);
    math_update_counters(sfpu_addr_mode);
}

//
//    SFPLUT: Perform lookup table operation based on value in LREG entry 3.
//        Table values must be loaded into LREG entries 0, 1, 2.
//
//    Lookup table indexed by exp in LREG3 and get encoded values, A and B,
//    then perform FP calculation (A * ABS(LREG3)) + B.
//
//    Table format: A = LREGx[15:8], B = LREGx[7:0] where x = 0, 1, 2
//
//    Read A, B:
//        from LREG0 if unbiased exp in LREG3 < 0
//        from LREG1 if unbiased exp in LREG3 = 0
//        from LREG2 if unbiased exp in LREG3 > 0
//
//    Encoding 8-bit scheme for table values:
//        * 0xFF represents zero
//        * bit[7] is sign, bit[6:4] is exp, bit[3:0] is mantissa
//
//    Final 32-bit value composed as:
//        * sign bit
//        * 8-bit exp calculated as (127 - exp_extender) where exp_extender is unsigned 3b
//        * 23-bit mantissa formed as 4 mantissa bits with 19b zero padding on LSBs
//
//    Requires 2 cycles.
//
//    lreg_ind         LREG for result
//    instr_mod0       instruction modifier:
//                         0: unmodified LUT behavior
//                         1 - 3: reserved
//                         4: retain LREG3 sign
//                         5 - 7: reserved
//                         8: result LREG given by LREG7[0:3]
//                         9 - 11: reserved
//                         12: retain LREG3 sign, result LREG given by LREG7[0:3]
//                         13 - 15: reserved
//    dest_reg_addr    unused
//

void Tensix::SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) {
    if (lreg_ind >= 8) {
        return;
    }
    // ACHTUNG: Currently only 0 and 4 values of 'instr_mod0' are supported.
    //     (Current LLKs use only 4.)
    // TODO: Clarify semantics of other 'instr_mod0' values.
    switch (instr_mod0) {
    case 0:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg;
                float *dest;
                void operator()(uint32_t i) const {
                    lreg[i].f = lut(lreg0[i].i, lreg1[i].i, lreg2[i].i, lreg3[i].i);
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg = m_lreg[lreg_ind];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg;
                float *dest;
                void operator()(uint32_t i) const {
                    uint32_t s = lreg3[i].i & 0x80000000;
                    lreg[i].f = lut(lreg0[i].i, lreg1[i].i, lreg2[i].i, lreg3[i].i);
                    lreg[i].i &= 0x7fffffff;
                    lreg[i].i |= s;
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg = m_lreg[lreg_ind];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPMULI: Floating point multiplication of LREG and 16-bit immediate value.
//        Immediate value interpreted as bfloat with 16-bit left justified in FP32 and
//        16 LSBs of mantissa padded with 0. Requires 2 cycles.
//
//    imm16_math    immediate value in bfloat format
//    lreg_dest     LREG for initial value and result (but see instr_mod1 field)
//    instr_mod1    instruction modifier
//                      0 - 3: reserved
//                      4: initial value in LREG pointed by LREG7[3:0], result in lreg_dest
//                      5 - 7: reserved
//                      8: initial value in lreg_dest, result in LREG pointed by LREG7[3:0]
//                      9 - 11: reserved
//                      12: initial value and result in LREG pointed by LREG7[3:0]
//                      13 - 15: reserved

void Tensix::SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: Description of 'instr_mod1' in SFPU spec is apparently ambiguous.
    //     It lists value 0 as reserved although this is the only value currently used in LLKs.
    //     Only 0 is supported here until further clarification.
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    imm16_math &= 0xffff;
    struct {
        float imm;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f *= imm;
        }
    } op;
    op.imm = u16_as_fp16b(imm16_math);
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//    SFPADDI: Floating point addition of LREG and 16-bit immediate value.
//        Immediate value interpreted as bfloat with 16-bit left justified in FP32 and
//        16 LSBs of mantissa padded with 0. Requires 2 cycles.
//
//    Arguments: see SFPMULI above
//

void Tensix::SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: See comment to SFPADDI above
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    imm16_math &= 0xffff;
    struct {
        float imm;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f += imm;
        }
    } op;
    op.imm = u16_as_fp16b(imm16_math);
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPDIVP2: Manipulate exponent field of value in LREG (lreg_c) using immediate field,
//        write result back to LREG (lreg_dest). Result sign and mantissa are taken from lreg_c.
//        If lreg_c contains value NaN or Infinity, result is this value without modification.
//
//    imm12_math    immediate field
//    lreg_c        LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0: new exp taken directly from immediate field
//                      1: new exp is result of adding of immediate field and original exp
//                      2 - 15: reserved
//

void Tensix::SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    uint32_t imm = imm12_math & 0xfff;
    switch (instr_mod1) {
    case 0:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    U32 &x = c[i];
                    if (std::isnan(x.f) || std::isinf(x.f)) {
                        return;
                    }
                    dest[i].i = (x.i & 0x807fffff) | (imm << 23);
                }
            } op;
            op.imm = imm & 0xff;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    U32 &x = c[i];
                    if (std::isnan(x.f) || std::isinf(x.f)) {
                        return;
                    }
                    // TODO: Figure out how to handle overflows
                    uint32_t e = (((x.i >> 23) & 0xff) + imm) & 0xff;
                    dest[i].i = (x.i & 0x807fffff) | (e << 23);
                }
            } op;
            // TODO: Figure out whether 'imm' shall be interpreted as signed integer
            op.imm = imm & 0xff;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPEXEXP: Extract 8-bit exponent from LREG (lreg_c), write result to LREG (lreg_dest).
//
//    imm12_math    unused
//    lreg_c        LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0: exponent debiased then sign extended, CC not modified
//                      1: exponent not debiased then zero extended, CC not modified
//                      2: exponent debiased then sign extended, CC set to exponent sign
//                      3: extracted exponent not debiased then zero extended, CC set to 0
//                      4 - 7: reserved
//                      8: exponent debiased then sign extended, CC flipped
//                      9: exponent not debiased then zero extended, CC flipped
//                      10: exponent debiased then sign extended, CC set to flipped exponent sign
//                      11: exponent not debiased then zero extended, CC set to 1
//                      12 - 15: reserved
//

void Tensix::SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = ((c[i].i >> 23) & 0xff) - 127;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i >> 23) & 0xff;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = ((c[i].i >> 23) & 0xff) - 127;
                    dest[i].i = z;
                    msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 3:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i >> 23) & 0xff;
                    set_cc(*cc, i, 0);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = ((c[i].i >> 23) & 0xff) - 127;
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 9:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i >> 23) & 0xff;
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 10:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = ((c[i].i >> 23) & 0xff) - 127;
                    dest[i].i = z;
                    comp_msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 11:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i >> 23) & 0xff;
                    set_cc(*cc, i, 1);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPEXMAN: Extract mantissa from LREG (lreg_c), write result into LREG (lreg_dest).
//
//    imm12_math    unused
//    lreg_c        LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier
//                      0: extend mantissa to include hidden bit, pad with 8 zeroes in MSBs
//                      1: pad mantissa with 9 zeroes in MSBs
//                      2 - 15: reserved
//

void Tensix::SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x7fffff) | 0x800000;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = c[i].i & 0x7fffff;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPIADD: Perform integer addition between LREG (lreg_c) and either immediate or
//        LREG (lreg_dest), write result into LREG (lreg_dest)
//
//    imm12_math    immediate field
//    lreg_c        LREG for input of integer addition
//    lreg_dest     LREG for result and optionally LREG for input
//    instr_mod1    instruction modifier
//                      0: add lreg_c and lreg_dest, set CC to MSB of result
//                      1: add lreg_c and sign-extended imm12_math, set CC to MSB ...
//                      2: add lreg_c and 2's complement to lreg_dest, set CC to MSB ...
//                      3: reserved
//                      4: add lreg_c and lreg_dest, CC not modified
//                      5: add lreg_c and sign-extended imm12_math, CC not modified
//                      6: add lreg_c and 2's complement to lreg_dest, CC not modified
//                      7: reserved
//                      8: add lreg_c and lreg_dest, set CC to flipped MSB of result
//                      9: add lreg_c and sign-extended imm12_math, set CC to flipped MSB ...
//                      10: add lreg_c and 2's complement to lreg_dest, set CC to fliped MSB ... 
//                      11: reserved
//                      12: add lreg_c and lreg_dest, CC flipped
//                      13: add lreg_c and sign-extended imm12_math, CC flipped
//                      14: add lreg_c and 2's complement to lreg_dest, CC flipped
//                      15: reserved
//

void Tensix::SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    uint32_t imm = imm12_math & 0xfff;
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + dest[i].i;
                    dest[i].i = z;
                    msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + imm;
                    dest[i].i = z;
                    msb_to_cc(*cc, i, z);
                }
            } op;
            op.imm = sext_u12(imm);
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i - dest[i].i;
                    dest[i].i = z;
                    msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + dest[i].i;
                    dest[i].i = z;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 5:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + imm;
                    dest[i].i = z;
                }
            } op;
            op.imm = sext_u12(imm);
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i - dest[i].i;
                    dest[i].i = z;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + dest[i].i;
                    dest[i].i = z;
                    comp_msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 9:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + imm;
                    dest[i].i = z;
                    comp_msb_to_cc(*cc, i, z);
                }
            } op;
            op.imm = sext_u12(imm);
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 10:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i - dest[i].i;
                    dest[i].i = z;
                    comp_msb_to_cc(*cc, i, z);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 12:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + dest[i].i;
                    dest[i].i = z;
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 13:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i + imm;
                    dest[i].i = z;
                    comp_cc(*cc, i);
                }
            } op;
            op.imm = sext_u12(imm);
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 14:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t z = c[i].i - dest[i].i;
                    dest[i].i = z;
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSHFT: Shift LREG (lreg_dest) by amount in either immediate field or LREG (lreg_c).
//        Right shift is sign bit of shift amount is 1, left shift otherwise.
//
//    imm12_math    immediate field (2's complement int)
//    lreg_c        LREG optionally for shift amount (2's complement int)
//    lreg_dest     LREG for result as well as initial value
//    instr_mod1    instruction modifier
//                      0: lreg_c contains shift amount
//                      1: imm12_math contains shift amount
//

void Tensix::SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    uint32_t imm = imm12_math & 0xfff;
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t shift = c[i].i;
                    if ((shift & 0x80000000) == 0) {
                        dest[i].i <<= shift;
                    } else {
                        shift = ~shift + 1;
                        dest[i].i >>= shift;
                    }
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        if ((imm & 0x800) == 0) {
            struct {
                uint32_t imm;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i <<= imm;
                }
            } op;
            op.imm = imm;
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        } else {
            struct {
                uint32_t imm;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i >>= imm;
                }
            } op;
            op.imm = 0x1000 - imm;
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSETCC: Set CC based on lreg_c or immediate value.
//        If CC enable not set, set CC to 0. Otherwise if CC is zero, keep CC unmodified.  
//
//        imm12_math    immediate field
//        lreg_c        LREG for value used to set CC
//        lreg_dest     unused
//        instr_mod1    instruction modifier
//                          0: CC set to sign bit in lreg_c
//                          1: CC set to bit 0 of imm12_math
//                          2: CC set to 1 if lreg_c is non-zero, otherwise CC set to 0
//                          3: reserved
//                          4: CC set to complement of sign bit in lreg_c
//                          5: reserved
//                          6: CC set to 1 if lreg_c is zero, otherwise CC set to 0
//                          7: reserved
//                          8: CC flipped
//                          9 - 15: reserved
//

void Tensix::SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    msb_to_cc(*cc, i, c[i].i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t bit;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    set_cc(*cc, i, bit);
                }
            } op;
            op.bit = imm12_math & 0x1;
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t bit = uint32_t(c[i].i != 0);
                    set_cc(*cc, i, bit);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *c;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    comp_msb_to_cc(*cc, i, c[i].i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *c;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t bit = uint32_t(c[i].i == 0);
                    set_cc(*cc, i, bit);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            struct {
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    comp_cc(*cc, i);
                }
            } op;
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPMOV: Move LREG or config register specified by (instr_mod1, lreg_c)
//        into LREG (lreg_dest).
//
//    imm12_math    unused
//    lreg_c        LREG for value to be copied
//    lreg_dest     LREG to be written
//    instr_mod1    instruction modifier:
//                      0: copy lreg_c unmodified
//                      1: copy lreg_c with compilment of sign bit of lreg_c
//                      2: copy lreg_c unmodified, unconditional upon CC
//                      3 - 7: reserved
//                      8: copy config register specified by lreg_c:
//                          0: LOADMACRO instruction register 0
//                          1: LOADMACRO instruction register 1
//                          2: LOADMACRO instruction register 2
//                          3: LOADMACRO instruction register 3
//                          4: LOADMACRO sequence register 0
//                          5: LOADMACRO sequence register 1
//                          6: LOADMACRO sequence register 2
//                          7: LOADMACRO sequence register 3
//                          8: LOADMACRO control register
//                          9: pseudo-random counter value
//                          10 - 14: zero
//                          15: SFPU control register
//                      9 - 15: reserved
//

void Tensix::SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = c[i].i;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = c[i].i ^ 0x80000000;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            U32 *c = m_lreg[lreg_c];
            U32 *dest = m_lreg[lreg_dest];
            for (uint32_t i = 0; i < LROW_SIZE; i++) {
                dest[i].i = c[i].i;
            }
        }
        break;
    case 8:
        // ACHTUNG: Copying config registers is not yet implemented
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPABS: Return absolute value of LREG (lreg_c).
//
//    imm12_math    unused
//    lreg_c        LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0: return absolute value of integer format value
//                      1: return absolute value of FP format value
//                      2 - 15: reserved
//

void Tensix::SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    int32_t x = int32_t(c[i].i);
                    x = (x >= 0) ? x : -x;
                    dest[i].i = uint32_t(x);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float x = c[i].f;
                    dest[i].f = (x >= 0.0f) ? x : -x;
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPAND: Perform bitwise AND between LREG (lreg_c) and LREG (lreg_dest).
//
//    imm12_math    unused
//    lreg_c        LREG for input value
//    lreg_dest     LREG for input value and result
//    instr_mod1    instruction modifier:
//                      0 - 15: reserved
//

void Tensix::SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].i &= c[i].i;
        }
    } op;
    op.c = m_lreg[lreg_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPOR: Perform bitwise OR between LREG (lreg_c) and LREG (lreg_dest).
//
//    Arguments: see SFPAND above
//

void Tensix::SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].i |= c[i].i;
        }
    } op;
    op.c = m_lreg[lreg_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPNOT: Perform bitwise complement on LREG (lreg_c).
//
//    imm12_math    unused
//    lreg_c        LREG for input value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0 - 15: reserved
//

void Tensix::SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].i = ~c[i].i;
        }
    } op;
    op.c = m_lreg[lreg_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPLZ: Count leading zeroes in LREG (lreg_c), write result into LREG (lreg_dest)
//
//    imm12_math    unused
//    lreg_c        LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0: LZ count without masking bit 31, CC unmodified
//                      1: reserved
//                      2: LZ count without masking bit 31, CC set to 1 if lreg_c is nonzero
//                      3: reserved
//                      4: LZ count after masking bit 31 to 0, CC unmodified
//                      5: reserved
//                      6: LZ count after masking bit 31 to 0, CC set to 1 if masked lreg_c is nonzero
//                      7: reserved
//                      8: LZ count without masking bit 31, CC flipped
//                      9: reserved
//                      10: LZ count without masking bit 31, CC set to 1 if lreg_c is zero
//                      11: reserved
//                      12: LZ count after masking bit 31 to 0, CC flipped
//                      13: reserved
//                      14: LZ count after masking bit 31 to 0, CC set to 1 if masked lreg_c is zero
//                      15: reserved
//

void Tensix::SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = lz(c[i].i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t x = c[i].i;
                    dest[i].i = lz(x);
                    if (x != 0) {
                        set_cc(*cc, i, 1);
                    }
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = lz(c[i].i & 0x7fffffff);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t x = c[i].i & 0x7fffffff;
                    dest[i].i = lz(x);
                    if (x != 0) {
                        set_cc(*cc, i, 1);
                    }
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = lz(c[i].i);
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 10:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t x = c[i].i;
                    dest[i].i = lz(x);
                    if (x == 0) {
                        set_cc(*cc, i, 1);
                    }
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 12:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    dest[i].i = lz(c[i].i & 0x7fffffff);
                    comp_cc(*cc, i);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 14:
        {
            struct {
                U32 *c;
                U32 *dest;
                uint32_t *cc;
                void operator()(uint32_t i) const {
                    uint32_t x = c[i].i & 0x7fffffff;
                    dest[i].i = lz(x);
                    if (x == 0) {
                        set_cc(*cc, i, 1);
                    }
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            op.cc = &m_cc;
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSETEXP: Combine sign and mantissa of LREG (lreg_c) with either imm12_math or
//        LREG (lreg_dest) as exp, write result into LREG (lreg_dest).
//
//    imm12_math    immediate field
//    lreg_c        LREG for sign and mantissa
//    lreg_dest     LREG for result and optionally for exp
//    instr_mod1    instruction modifier:
//                      0: exp taken from lreg_dest[7:0]
//                      1: exp taken from imm12_math[7:0]
//                      2: exp taken from lreg_dest[30:23]
//                      3 - 15: reserved
//

void Tensix::SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x807fffff) | ((dest[i].i & 0xff) << 23);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x807fffff) | imm;
                }
            } op;
            op.imm = (imm12_math & 0xff) << 23;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x807fffff) | (dest[i].i & 0x7f800000);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSETMAN: Combine sign and exp of LREG (lreg_c) with either imm12_math or
//        LREG (lreg_dest) as mantissa, write result into LREG (lreg_dest).
//
//    imm12_math    immediate field
//    lreg_c        LREG for sign and mantissa
//    lreg_dest     LREG for result and optionally for exp
//    instr_mod1    instruction modifier:
//                      0: mantissa taken from lreg_dest[22:0]
//                      1: mantissa[22:11] taken from imm12_math[11:0], [10:0] padded with zeroes
//                      2 - 15: reserved
//

void Tensix::SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0xff800000) | (dest[i].i & 0x7fffff);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0xff800000) | imm;
                }
            } op;
            op.imm = (imm12_math & 0xfff) << 11;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPMAD: Perform Fp fused multiply-add: (A * B) + C with LREGs A (lreg_src_a),
//        B (lreg_src_b), and C (lreg_src_c). Write result into LREG (lreg_dest).
//        Detect condition and set status bit for Infinity, NaN, Subnormal, and Overflow.
//        Requires 2 cycles.
//
//    lreg_src_a    LREG for A
//    lreg_src_b    LREG for B
//    lreg_src_c    LREG for C
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0 - 3: reserved
//                      4: A given by LREG pointed to by LREG7[3:0],
//                          result written to lreg_dest
//                      5 - 7: reserved
//                      8: A given by lreg_dest
//                          result written to LREG pointed to by LREG7[3:0]
//                      9 - 11: reserved
//                      12: A given by LREG pointed to by LREG7[3:0],
//                          result written to LREG pointed to by LREG7[3:0]
//

void Tensix::SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: See comment to SFPADDI above
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *src_a;
        U32 *src_b;
        U32 *src_c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f = src_a[i].f * src_b[i].f + src_c[i].f;
        }
    } op;
    op.src_a = m_lreg[lreg_src_a];
    op.src_b = m_lreg[lreg_src_b];
    op.src_c = m_lreg[lreg_src_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPADD: Perform FP add: A + C with LREGs A (lreg_src_a) and C (lreg_src_c).
//        Write result into LREG (lreg_dest). Detect condition and set status bit for
//        Infinity, NaN, Subnormal, and Overflow. Requires 2 cycles.
//
//    Arguments: see SFPMAD above (assume B = 0).
//

void Tensix::SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: See comment to SFPADDI above
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *src_a;
        U32 *src_c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f = src_a[i].f + src_c[i].f;
        }
    } op;
    op.src_a = m_lreg[lreg_src_a];
    op.src_c = m_lreg[lreg_src_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPMUL: Perform FP multiply: A * B with LREGs A (lreg_src_a) and B (lreg_src_b).
//        Write result into LREG (lreg_dest). Detect condition and set status bit for
//        Infinity, NaN, Subnormal, and Overflow. Requires 2 cycles.
//
//    Arguments: see SFPMAD above (assume C = 0).
//

void Tensix::SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: See comment to SFPADDI above
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *src_a;
        U32 *src_b;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f = src_a[i].f * src_b[i].f;
        }
    } op;
    op.src_a = m_lreg[lreg_src_a];
    op.src_b = m_lreg[lreg_src_b];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPPUSHC: Push current state of CC and CC enable onto CC stack
//
//    imm12_math    unused
//    lreg_c        unused
//    lreg_dest     unused
//    instr_mod1    instruction modifier:
//                      0: normal push
//                      1: replace top.result = CC, top.enable = CC_enable
//                      2: replace top.result = NOT CC, top.enable = CC_enable
//                      3: replace top.result = top.result AND CC, top.enable = CC_enable
//                      4: replace top.result = top.result OR CC, top.enable = CC_enable
//                      5: replace top.result = top.result AND NOT CC, top.enable = CC_enable
//                      6: replace top.result = top.result OR NOT CC, top.enable = CC_enable
//                      7: replace top.result = NOT top.result AND CC, top.enable = CC_enable
//                      8: replace top.result = NOT top.result OR CC, top.enable = CC_enable
//                      9: replace top.result = NOT top.result AND NOT CC, top.enable = CC_enable
//                      10: replace top.result = NOT top.result OR NOT CC, top.enable = CC_enable
//                      11: replace top.result = top.result XOR CC, top.enable = CC_enable
//                      12: replace top.result = top.result XNOR CC, top.enable = CC_enable
//                      13: replace top.result = NOT CC, top.enable = CC_enable
//                          update CC = NOT CC, CC_enable unmodified
//                      14: replace top.result = 1, top.enable = 1
//                      15: replace top.result = 0, top.enable = 1
//

void Tensix::SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: SFPU specification does not define
    //     what to do in case of stack overflow or underflow
    // ACHTUNG: Although SFPU specification does not list SFPPUSHC among
    //     instructions that affect all lanes unconditionally regardless their CC settings,
    //     intuition suggests that this shall be the case (as for SFPPOPC)
    if (instr_mod1 == 0) {
        assert(m_cc_sp < CC_STACK_SIZE);
        m_cc_stack[m_cc_sp] = m_cc;
        m_cc_en_stack[m_cc_sp] = m_cc_en;
        m_cc_sp++;
        return;
    }
    assert(m_cc_sp > 0);
    uint32_t top = m_cc_sp - 1;
    switch (instr_mod1) {
    case 1: 
        m_cc_stack[top] = m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 2: 
        m_cc_stack[top] = ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 3: 
        m_cc_stack[top] &= m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 4: 
        m_cc_stack[top] |= m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 5: 
        m_cc_stack[top] &= ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 6: 
        m_cc_stack[top] |= ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 7: 
        m_cc_stack[top] = ~m_cc_stack[top] & m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 8: 
        m_cc_stack[top] = ~m_cc_stack[top] | m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 9: 
        m_cc_stack[top] = ~m_cc_stack[top] & ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 10: 
        m_cc_stack[top] = ~m_cc_stack[top] | ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 11: 
        m_cc_stack[top] ^= m_cc;
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 12: 
        m_cc_stack[top] = ~(m_cc_stack[top] ^ m_cc);
        m_cc_en_stack[top] = m_cc_en;
        break;
    case 13: 
        m_cc_stack[top] = ~m_cc;
        m_cc_en_stack[top] = m_cc_en;
        m_cc = ~m_cc;
        break;
    case 14: 
        m_cc_stack[top] = ~(uint32_t(0));
        m_cc_en_stack[top] = ~(uint32_t(0));
        break;
    case 15: 
        m_cc_stack[top] = 0;
        m_cc_en_stack[top] = ~(uint32_t(0));
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPPOPC: Pop CC and CC enable state from top of CC stack. No state update is stack is empty.
//
//    imm12_math    unused
//    lreg_c        unused
//    lreg_dest     unused
//    instr_mod1    instruction modifier:
//                      0: normal pop
//                      1: retain top, CC = top.result, CC_enable = top.enable
//                      2: retain top, CC = NOT top.result, CC_enable = top.enable
//                      3: retain top, CC = top.result AND CC, CC_enable = top.enable
//                      4: retain top, CC = top.result OR CC, CC_enable = top.enable
//                      5: retain top, CC = NOT top.result AND CC, CC_enable = top.enable
//                      6: retain top, CC = NOT top.result OR CC, CC_enable = top.enable
//                      7: retain top, CC = top.result AND NOT CC, CC_enable = top.enable
//                      8: retain top, CC = top.result OR NOT CC, CC_enable = top.enable
//                      9: retain top, CC = NOT top.result AND NOT CC, CC_enable = top.enable
//                      10: retain top, CC = NOT top.result OR NOT CC, CC_enable = top.enable
//                      11: retain top, CC = top.result XOR CC, CC_enable = top.enable
//                      12: retain top, CC = top.result XNOR CC, CC_enable = top.enable
//                      13: retain top, CC = 0, CC_enable = 0
//                      14: retain top, CC = 1, CC_enable = 1
//                      15: retain top, CC = 0, CC_enable = 1
//

void Tensix::SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (m_cc_sp == 0) {
        return;
    }
    uint32_t top = m_cc_sp - 1;
    switch (instr_mod1) {
    case 0:
        m_cc = m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        m_cc_sp = top;
        break;
    case 1:
        m_cc = m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 2:
        m_cc = ~m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 3:
        m_cc &= m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 4:
        m_cc |= m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 5:
        m_cc &= ~m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 6:
        m_cc |= ~m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 7:
        m_cc = ~m_cc & m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 8:
        m_cc = ~m_cc | m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 9:
        m_cc = ~m_cc & ~m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 10:
        m_cc = ~m_cc | ~m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 11:
        m_cc ^= m_cc_stack[top];
        m_cc_en = m_cc_en_stack[top];
        break;
    case 12:
        m_cc = ~(m_cc ^ m_cc_stack[top]);
        m_cc_en = m_cc_en_stack[top];
        break;
    case 13:
        m_cc = 0;
        m_cc_en = 0;
        break;
    case 14:
        m_cc = ~uint32_t(0);
        m_cc_en = ~(uint32_t(0));
        break;
    case 15:
        m_cc = 0;
        m_cc_en = ~(uint32_t(0));
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPSETSGN: Combine exp and mantissa of LREG (lreg_c) with either imm12_math or
//        LREG (lreg_dest) as sign, write result into LREG (lreg_dest).
//
//    imm12_math    immediate field
//    lreg_c        LREG for exp and mantissa
//    lreg_dest     LREG for result and optionally for sign
//    instr_mod1    instruction modifier:
//                      0: sign taken from lreg_dest[31]
//                      1: sign taken from imm12_math[0]
//                      2 - 15: reserved
//

void Tensix::SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x7fffffff) | (dest[i].i & 0x80000000);
                }
            } op;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = (c[i].i & 0x7fffffff) | imm;
                }
            } op;
            op.imm = (imm12_math & 0x1) << 31;
            op.c = m_lreg[lreg_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPENCC: Set CC enable and CC
//
//    imm12_math    immediate field
//    lreg          unused
//    lreg_dest     unused
//    instr_mod1    instruction modifier:
//                      0: CC_enable unmodified, CC = 1
//                      1: CC_enable = NOT CC_enable, CC = 1
//                      2: CC_enable = imm12_math[0], CC = 1
//                      3 - 7: reserved
//                      8: CC_enable unmodified, CC = imm12_math[1]
//                      9: CC_enable = NOT CC_enable, CC = imm12_math[1]
//                      10: CC_enable = imm12_math[0], CC = imm12_math[1]
//                      11 - 15: reserved
//

void Tensix::SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    switch (instr_mod1) {
    case 0:
        m_cc = ~uint32_t(0);
        break;
    case 1:
        m_cc = ~uint32_t(0);
        m_cc_en = ~m_cc_en;
        break;
    case 2:
        m_cc = ~uint32_t(0);
        m_cc_en = ~(imm12_math & 0x1) + 1;
        break;        
    case 8:
        m_cc = ~((imm12_math >> 1) & 0x1) + 1;
        break;
    case 9:
        m_cc = ~((imm12_math >> 1) & 0x1) + 1;
        m_cc_en = ~m_cc_en;
        break;
    case 10:
        m_cc = ~((imm12_math >> 1) & 0x1) + 1;
        m_cc_en = ~(imm12_math & 0x1) + 1;
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPCOMPC: Complement CC state when certain conditions are met:
//        if CC_enable = 0, set CC = 0
//        if CC_enable = 1:
//            if CC stack is empty, set CC = NOT CC
//            if CC stack is not empty:
//                if top.CC_enable = 1 and top.CC = 1, set CC = NOT CC
//                else set CC = 0
//
//    imm12_math    unused
//    lreg_c        unused
//    lreg_dest     unused
//    instr_mod1    instruction modifier:
//                      0 - 15: reserved
//

void Tensix::SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (m_cc_en == 0) {
        m_cc = 0;
    } else if (m_cc_sp == 0) {
        m_cc = ~m_cc;
    } else {
        uint32_t top = m_cc_sp - 1;
        if (m_cc_en_stack[top] == 0) {
            m_cc = 0;
        } else {
            // old CC  top.CC  new CC
            //    0       0       0
            //    0       1       1
            //    1       0       0
            //    1       1       0
            m_cc = ~m_cc & m_cc_stack[top];
        }
    }
}

//
//    SFPTRANSP: Transpose LREG values using 4x4 interpretation of LREG structure
//        within SFPU instance. One instance has 4 rows of 8 LREGs, split in two
//        groups, LREG[3:0] and LREG[7:4]. This split forms two 4x4 grids of LREGs
//        that are transposed in parallel.
//
//    Grid 0:    Row[0][3:0], Row[1][3:0], Row[2][3:0], Row[3][3:0]
//    Grid 1:    Row[0][7:4], Row[1][7:4], Row[2][7:4], Row[3][7:4]
//
//    for x, y in [0 : 3]:
//        transpose grid 0: Row[y][x] = Row[x][y] 
//        transpose grid 1: Row[y][x + 4] = Row[x][y + 4]
//
//    CC bits configured to disable execution prevent new values be written to respective rows
//    (but read is allowed for such rows).
//
//    imm12_math    unused
//    lreg_c        unused
//    lreg_dest     unused
//    instr_mod1    instruction modifier:
//                      0 - 15: reserved
//

void Tensix::SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // Row[inst][x][y] where inst in 0..7, x in 0..3, y in 0..7
    //     corresponds to lreg[y][x * 8 + inst]
    for (uint32_t inst = 0; inst < 8; inst++) {
        for (uint32_t x = 0; x < 4; x++) {
            for (uint32_t y = 0; y < x; y++) {
                U32 *lreg1_g0 = m_lreg[y];
                U32 *lreg1_g1 = m_lreg[y + 4];
                uint32_t i1 = x * 8 + inst;
                U32 *lreg2_g0 = m_lreg[x];
                U32 *lreg2_g1 = m_lreg[x + 4];
                uint32_t i2 = y * 8 +  inst;
                uint32_t temp_g0 = lreg1_g0[i1].i;
                uint32_t temp_g1 = lreg1_g1[i1].i;
                if (m_cc_en == 0 || (m_cc & (1 << i1)) != 0) {
                    lreg1_g0[i1].i = lreg2_g0[i2].i;
                    lreg1_g1[i1].i = lreg2_g1[i2].i;
                }
                if (m_cc_en == 0 || (m_cc & (1 << i2)) != 0) {
                    lreg2_g0[i2].i = temp_g0;
                    lreg2_g1[i2].i = temp_g1;
                }
            }
        }
    }
}

//
//    SFPXOR: Perform bitwise XOR between LREG (lreg_c) and LREG (lreg_dest).
//
//    Arguments: see SFPAND above
//

void Tensix::SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].i ^= c[i].i;
        }
    } op;
    op.c = m_lreg[lreg_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFP_STOCH_RND: Perform format conversion using either stochastic rounding or
//        round to nearest even mode.
//
//    rnd_mode      rounding mode:
//                      0: round to nearest even
//                      1: stochastic rounding
//    imm8_math     immediate field
//    lreg_src_b    LREG optionally for descale value
//    lreg_src_c    LREG for initial value
//    lreg_dest     LREG for result value
//    instr_mod1    instruction modifier:
//                      0: fp32 -> fp16 (fp16_a) using lreg_src_b as descale
//                      1: fp32 -> bfloat (fp16_b) using lreg_src_b as descale
//                      2: fp32 -> uint8 using lreg_src_b as descale
//                      3: fp32 -> int8 using lreg_src_b as descale
//                      4: int32 -> uint8 using lreg_src_b as descale
//                      5: int32 -> int8 using lreg_src_b as descale
//                      6: fp32 -> uint16 using lreg_src_b as descale 
//                      7: fp32 -> int16 using lreg_src_b as descale 
//                      8: fp32 -> fp16 (fp16_a) using imm8_math as descale
//                      9: fp32 -> bfloat (fp16_b) using imm8_math as descale
//                      10: fp32 -> uint8 using imm8_math as descale
//                      11: fp32 -> int8 using imm8_math as descale
//                      12: int32 -> uint8 using imm8_math as descale
//                      13: int32 -> int8 using imm8_math as descale
//                      14: fp32 -> uint16 using imm8_math as descale 
//                      15: fp32 -> int16 using imm8_math as descale 
//

void Tensix::SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // TODO: Figure out exact rounding algorithms for all modes
    // ACHTUNG: Stochastic rounding is not supported
    // ACHTUNG: Descale is respected for int -> int modes only (4, 5, 12, 13)
    //     and ignored for all other modes - is this correct?
    if (lreg_dest >= 8) {
        return;
    }
    uint32_t imm = (imm8_math & 0xff);
    switch (instr_mod1) {
    case 0:
        {
            // fp32: [31] sign [30:34] exponent (bias 127) [22:0] mantissa
            // fp16a: [15] sign [14:10] exponent (bias 15) [9:0] mantissa
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t temp = c[i].i;
                    temp &= ~uint32_t(0x1fff);
                    // TODO: Handle out-of-range exponent values
                    dest[i].i &= temp;
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = c[i].i & 0xffff0000;
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < 0) ? 0 : (iv > 0xff) ? 0xff : uint8_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 3:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < -128) ? -128 : (iv > 127) ? 127 : int8_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t uv = descale_u32(c[i].i, b[i].i);
                    dest[i].i = (uv > 0xff) ? 0xff : uint8_t(uv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 5:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    int32_t iv = descale_i32(int32_t(c[i].i), b[i].i);
                    dest[i].i = (iv < -128) ? -128 : (iv > 127) ? 127 : int8_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < 0) ? 0 : (iv > 0xffff) ? 0xffff : uint16_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 7:
        {
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < -32768) ? -32768 : (iv > 32767) ? 32767 : int16_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            // same as 0 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t temp = c[i].i;
                    temp &= ~uint32_t(0x1fff);
                    // TODO: Handle out-of-range exponent values
                    dest[i].i &= temp;
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 9:
        {
            // same as 1 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i = c[i].i & 0xffff0000;
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 10:
        {
            // same as 2 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < 0) ? 0 : (iv > 0xff) ? 0xff : uint8_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 11:
        {
            // same as 3 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < -128) ? -128 : (iv > 127) ? 127 : int8_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 12:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t uv = descale_u32(c[i].i, imm);
                    dest[i].i = (uv > 0xff) ? 0xff : uint8_t(uv);
                }
            } op;
            op.imm = imm;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 13:
        {
            struct {
                uint32_t imm;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    int32_t iv = descale_i32(int32_t(c[i].i), imm);
                    dest[i].i = (iv < - 128) ? -128 : (iv > 127) ? 127 : int8_t(iv);
                }
            } op;
            op.imm = imm;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 14:
        {
            // same as 6 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < 0) ? 0 : (iv > 0xffff) ? 0xffff : uint16_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 15:
        {
            // same as 7 (no descale)
            struct {
                U32 *b;
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float fv = round_to_nearest_even(c[i].f);
                    int32_t iv = int32_t(fv);
                    dest[i].i = (iv < -32768) ? -32768 : (iv > 32767) ? 32767 : int16_t(iv);
                }
            } op;
            op.b = m_lreg[lreg_src_b];
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPNOP: Perform no operation.
//

void Tensix::SFPNOP() {
    // nothing to do
}

//
//    SFPCAST: Perform format conversion from int32 to fp32 using either stochastic rounding or
//        round to nearest even mode.
//
//    lreg_src_c    LREG for initial value
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      0: round to nearest even
//                      1: stochastic rounding
//                      2 - 15: reserved
//

void Tensix::SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: Stochastic rounding is not supported
    assert(instr_mod1 == 0);
    if (lreg_dest >= 8) {
        return;
    }
    struct {
        U32 *c;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].f = float(c[i].i);
        }
    } op;
    op.c = m_lreg[lreg_src_c];
    op.dest = m_lreg[lreg_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPCONFIG: Performs write to one of configuration SFPU registers.
//        Each SFPU instance has its own set of configuration registers shared by all rows.
//        Values to be written come either from immediate field or from Row[0][0] LREG
//        of each instance.
//
//    imm16_math     immediate field
//    config_dest    configuration register to be updated:
//                       0: LOADMACRO instruction 0
//                       1: LOADMACRO instruction 1
//                       2: LOADMACRO instruction 2
//                       3: LOADMACRO instruction 3
//                       4: LOADMACRO sequence 0
//                       5: LOADMACRO sequence 1
//                       6: LOADMACRO sequence 2
//                       7: LOADMACRO sequence 3
//                       8: LOADMACRO control
//                       9 - 10: reserved
//                       11: LREG[11]
//                       12: LREG[12]
//                       13: LREG[13]
//                       14: LREG[14]
//                       15: SFPU control
//    instr_mod1     instruction modifier:
//                       ** TODO **
//

void Tensix::SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) {
    // ACHTUNG: Only these values of 'instr_mod1' and 'config_dest' are supported
    // TODO: It is more complex than this: must load separately for each SFPU instance
    assert(instr_mod1 == 0);
    assert(config_dest >= 11 && config_dest <= 14);
    struct {
        uint32_t value;
        U32 *dest;
        void operator()(uint32_t i) const {
            dest[i].i = value;
        }
    } op;
    op.value = m_lreg[0][0].i;
    op.dest = m_lreg[config_dest];
    sfpu_op(op, m_cc, m_cc_en);
}

//
//    SFPSWAP: Exchange values in two LREGs, either unconditionally or based upon comparison
//        of values held in two specified LREGs. Requires 2 cycles.
//
//    imm12_math    unused
//    lreg_src_c    LREG for first value
//    lreg_dest     LREG for second value
//    instr_mod1    instruction modifier:
//                      ** TODO **
//

void Tensix::SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_src_c >= 8 || lreg_dest >= 8) {
        return;
    }
    // TODO: Clarify what is "sign + magnitude integer format".
    //     Currently fp32 format with 0 exponent is assumed.
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float temp = c[i].f;
                    c[i].f = dest[i].f;
                    dest[i].f = temp;
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 1:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (td > tc) {
                        // larger -> c
                        c[i].f = td;
                        dest[i].f = tc;
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 0 || row == 1) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 3:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 0 || row == 2) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 0 || row == 3) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 5:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 0) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 1) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 7:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 2) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 8:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t row = i / 8;
                    float tc = c[i].f;
                    float td = dest[i].f;
                    if (row == 3) {
                        if (td > tc) {
                            // larger -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    } else {
                        if (td < tc) {
                            // smaller -> c
                            c[i].f = td;
                            dest[i].f = tc;
                        }
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPLOADMACRO: Load data from Dest register into LREG (lreg_ind).
//        Specified dest_reg_addr is added to BASE, x_counter, and DEST_TARGE_REG_CFG
//        registers to compute final Dest register address.
//        In addition to load, execute one of four preconfigured macro sequences.
//
//    lreg_ind          LREG index, combined with LSB of dest_reg_addr to specify
//                      which LREG to write and which macro sequence to execute.
//                          ** TODO **
//    instr_mod0        instruction modifier:
//                          ** TODO **
//    sfpu_addr_mode    address mode for updates to Src and Dest register addresses as side effect
//    dest_reg_addr     immediate offset into Dest register
//

void Tensix::SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    // TODO
}

//
//    SFPSFHT2: Shift values within or between LREGs, either within SFPU instance or
//        globally across instances. Global shift variants require 2 cycles.
//
//    imm12_math    unused
//    lreg_src_c    LREG for number of bits to shift for bitwise variants
//                  or value to shift for global variants
//    lreg_dest     LREG for result
//    instr_mod1    instruction modifier:
//                      ** TODO **
//

void Tensix::SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    uint32_t imm = imm12_math & 0xfff;
    // Row[inst][x][y] where inst in 0..7, x in 0..3, y in 0..7
    //     corresponds to lreg[y][x * 8 + inst]
    switch (instr_mod1) {
    case 0:
        {
            // ACHTUNG: Shall CC be taken in account here?
            U32 *lreg0 = m_lreg[0];
            U32 *lreg1 = m_lreg[1];
            U32 *lreg2 = m_lreg[2];
            U32 *lreg3 = m_lreg[3];
            for (uint32_t i = 0; i < 32; i++) {
                lreg0[i].i = lreg1[i].i;
                lreg1[i].i = lreg2[i].i;
                lreg2[i].i = lreg3[i].i;
                lreg3[i].i = 0;            
            }
        }
        break;
    case 1:
        {
            // ACHTUNG: Shall CC be taken in account here?
            U32 *lreg0 = m_lreg[0];
            U32 *lreg1 = m_lreg[1];
            U32 *lreg2 = m_lreg[2];
            U32 *lreg3 = m_lreg[3];
            for (uint32_t inst = 0; inst < 8; inst++) {
                for (uint32_t row = 0; row < 4; row++) {
                    uint32_t i = row * 8 + inst;
                    lreg0[i].i = lreg1[i].i;
                    lreg1[i].i = lreg2[i].i;
                    lreg2[i].i = lreg3[i].i;
                    lreg3[i].i = (row < 3) ? lreg0[i + 8].i : 0;
                }
            }
        }
        break;
    case 2:
        {
            // ACHTUNG: Shall CC be taken in account here?
            uint32_t temp[32];
            U32 *c = m_lreg[lreg_src_c];
            U32 *lreg0 = m_lreg[0];
            U32 *lreg1 = m_lreg[1];
            U32 *lreg2 = m_lreg[2];
            U32 *lreg3 = m_lreg[3];
            for (uint32_t i = 0; i < 32; i++) {
                temp[i] = c[i].i;
                lreg0[i].i = lreg1[i].i;
                lreg1[i].i = lreg2[i].i;
                lreg2[i].i = lreg3[i].i;
            }
            for (uint32_t inst = 0; inst < 8; inst++) {
                for (uint32_t row = 0; row < 4; row++) {
                    uint32_t i = row * 8 + inst;
                    uint32_t k = row * 8 + (inst + 1) % 8;
                    lreg3[k].i = temp[i];
                }
            }
        }
        break;
    case 3:
        {
            // ACHTUNG: Shall CC be taken in account here?
            U32 *c = m_lreg[lreg_src_c];
            U32 *dest = m_lreg[lreg_dest];
            uint32_t temp[4];
            temp[3] = c[3 * 8 + 7].i;
            temp[2] = c[2 * 8 + 7].i;
            temp[1] = c[1 * 8 + 7].i;
            temp[0] = c[0 * 8 + 7].i;
            for (uint32_t i = 0; i < 7; i++) {
                uint32_t inst = 7 - i; // 7 .. 1
                dest[3 * 8 + inst].i = c[3 * 8 + inst - 1].i;
                dest[2 * 8 + inst].i = c[2 * 8 + inst - 1].i;
                dest[1 * 8 + inst].i = c[1 * 8 + inst - 1].i;
                dest[0 * 8 + inst].i = c[0 * 8 + inst - 1].i;
            }
            dest[3 * 8].i = temp[3]; 
            dest[2 * 8].i = temp[2]; 
            dest[1 * 8].i = temp[1]; 
            dest[0 * 8].i = temp[0]; 
        }
        break;
    case 4:
        {
            // ACHTUNG: Shall CC be taken in account here?
            U32 *c = m_lreg[lreg_src_c];
            U32 *dest = m_lreg[lreg_dest];
            uint32_t temp[4];
            temp[3] = c[3 * 8 + 7].i;
            temp[2] = c[2 * 8 + 7].i;
            temp[1] = c[1 * 8 + 7].i;
            temp[0] = c[0 * 8 + 7].i;
            for (uint32_t i = 0; i < 7; i++) {
                uint32_t inst = 7 - i; // 7 .. 1
                dest[3 * 8 + inst].i = c[3 * 8 + inst - 1].i;
                dest[2 * 8 + inst].i = c[2 * 8 + inst - 1].i;
                dest[1 * 8 + inst].i = c[1 * 8 + inst - 1].i;
                dest[0 * 8 + inst].i = c[0 * 8 + inst - 1].i;
            }
            dest[3 * 8].i = 0; 
            dest[2 * 8].i = 0; 
            dest[1 * 8].i = 0; 
            dest[0 * 8].i = 0; 
        }
        break;
    case 5:
        {
            struct {
                U32 *c;
                U32 *dest;
                void operator()(uint32_t i) const {
                    uint32_t shift = c[i].i;
                    if ((shift & 0x80000000) == 0) {
                        dest[i].i <<= shift;
                    } else {
                        shift = ~shift + 1;
                        dest[i].i >>= shift;
                    }
                }
            } op;
            op.c = m_lreg[lreg_src_c];
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        if ((imm & 0x800) == 0) {
            struct {
                uint32_t imm;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i <<= imm;
                }
            } op;
            op.imm = imm;
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        } else {
            struct {
                uint32_t imm;
                U32 *dest;
                void operator()(uint32_t i) const {
                    dest[i].i >>= imm;
                }
            } op;
            op.imm = 0x1000 - imm;
            op.dest = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

//
//    SFPLUTFP32: Perform lookup table operation based on value in LREG entry 3.
//        Table values must be loaded into LREG entries 0, 1, 2, 4, 5, 6.
//
//    Lookup table indexed by LREG3 and get encoded values, A and B,
//    then perform FP calculation (A * ABS(LREG3)) + B.
//
//    Table format FP32:
//        let X = ABS(LREG3)
//        A = LREG0, B = LREG4 when 0.0 <= X < 1.0
//        A = LREG1, B = LREG5 when 1.0 <= X < 2.0
//        A = LREG2, B = LREG6 when 2.0 <= X
//
//    Modified FP16 6-entry table format mode 1:
//        let X = ABS(LREG3)
//        A = LREG0[15:0], B = LREG4[15:0] when 0.0 <= X < 0.5
//        A = LREG0[31:16], B = LREG4[31:16] when 0.5 <= X < 1.0
//        A = LREG1[15:0], B = LREG5[15:0] when 1.0 <= X < 1.5
//        A = LREG1[31:16], B = LREG5[31:16] when 1.5 <= X < 2.0
//        A = LREG2[15:0], B = LREG6[15:0] when 2.0 <= X < 3.0
//        A = LREG2[31:16], B = LREG6[31:16] when 3.0 <= X
//
//    Modified FP16 6-entry table format mode 1:
//        let X = ABS(LREG3)
//        A = LREG0[15:0], B = LREG4[15:0] when 0.0 <= X < 0.5
//        A = LREG0[31:16], B = LREG4[31:16] when 0.5 <= X < 1.0
//        A = LREG1[15:0], B = LREG5[15:0] when 1.0 <= X < 1.5
//        A = LREG1[31:16], B = LREG5[31:16] when 1.5 <= X < 2.0
//        A = LREG2[15:0], B = LREG6[15:0] when 2.0 <= X < 4.0
//        A = LREG2[31:16], B = LREG6[31:16] when 4.0 <= X
//
//    Modified FP16 3-entry format:
//        let X = ABS(LREG3)
//        A = LREG0[31:16], B = LREG0[15:0] when 0.0 <= X < 1.0
//        A = LREG1[31:16], B = LREG1[15:0] when 1.0 <= X < 2.0
//        A = LREG2[31:16], B = LREG2[15:0] when 2.0 <= X
//
//    Modified FP16 value stored as (sign, modified_exp[4:0], mantissa[9:0]).
//    Conversion to FP32:
//        * extend 10-bit mantissa to 23-bit by paddig LSBs with 13 zeroes
//        * if 5-bit modified_exp is all 1's, then 8-bit exp will be zero
//        * if 5-bit modified_exp is all 0's, it represents unbiased exp -15
//        * otherwise 5-bit modified_exp converted to 8-bit exp:
//            exp[7] = modified_exp[4]
//            exp[6:4] = modified_exp[4] ? 3'b000 : 3'b111
//            exp[3:0] = modified_exp[3:0]
//
//    Requires 2 cycles.
//
//    lreg_dest     LREG for result
//    instr_mod1    Instruction modifier:
//                      0: use FP32 table format
//                      1: reserved
//                      2: use modified FP16 6-entry format mode 1
//                      3: use modified FP16 6-entry format mode 2
//                      4: use FP32 table format, result retains initial LREG3 sign
//                      5: reserved
//                      6: use modified FP16 6-entry format mode 1, retain initial LREG3 sign
//                      7: use modified FP16 6-entry format mode 2, retain initial LREG3 sign
//                      8 - 9 reserved
//                      10: use modified FP16 3-entry format, result LREG given by LREG7[3:0]
//                      11 - 13: reserved
//                      14: use modified FP16 3-entry format, result retains initial LREG3 sign,
//                          result LREG given by LREG7[3:0]
//                      15: reserved
//

void Tensix::SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) {
    if (lreg_dest >= 8) {
        return;
    }
    switch (instr_mod1) {
    case 0:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    lreg[i].f = 
                        lut_fp32(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f);
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 2:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    lreg[i].f = 
                        lut_mfp16_6(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f,
                            1);
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 3:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    lreg[i].f = 
                        lut_mfp16_6(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f,
                            2);
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 4:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    uint32_t s = lreg3[i].i & 0x80000000;
                    lreg[i].f = 
                        lut_fp32(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f);
                    lreg[i].i &= 0x7fffffff;
                    lreg[i].i |= s;
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 6:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    uint32_t s = lreg3[i].i & 0x80000000;
                    lreg[i].f = 
                        lut_mfp16_6(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f,
                            1);
                    lreg[i].i &= 0x7fffffff;
                    lreg[i].i |= s;
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 7:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg4;
                U32 *lreg5;
                U32 *lreg6;
                U32 *lreg;
                void operator()(uint32_t i) const {
                    uint32_t s = lreg3[i].i & 0x80000000;
                    lreg[i].f = 
                        lut_mfp16_6(
                            lreg0[i].f, 
                            lreg1[i].f, 
                            lreg2[i].f, 
                            lreg3[i].f,
                            lreg4[i].f,
                            lreg5[i].f,
                            lreg6[i].f,
                            2);
                    lreg[i].i &= 0x7fffffff;
                    lreg[i].i |= s;
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg4 = m_lreg[4];
            op.lreg5 = m_lreg[5];
            op.lreg6 = m_lreg[6];
            op.lreg = m_lreg[lreg_dest];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 10:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg7;
                void operator()(uint32_t i) const {
                    U32 *dest = lreg0 + (lreg7[i].i & 0xf) * LROW_SIZE;
                    dest[i].f = lut_mfp16_3(lreg0[i].f, lreg1[i].f, lreg2[i].f, lreg3[i].f);
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg7 = m_lreg[7];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    case 14:
        {
            struct {
                U32 *lreg0;
                U32 *lreg1;
                U32 *lreg2;
                U32 *lreg3;
                U32 *lreg7;
                void operator()(uint32_t i) const {
                    uint32_t s = lreg3[i].i & 0x80000000;
                    U32 *dest = lreg0 + (lreg7[i].i & 0xf) * LROW_SIZE;
                    dest[i].f = lut_mfp16_3(lreg0[i].f, lreg1[i].f, lreg2[i].f, lreg3[i].f);
                    dest[i].i &= 0x7fffffff;
                    dest[i].i |= s;
                }
            } op;
            op.lreg0 = m_lreg[0];
            op.lreg1 = m_lreg[1];
            op.lreg2 = m_lreg[2];
            op.lreg3 = m_lreg[3];
            op.lreg7 = m_lreg[7];
            sfpu_op(op, m_cc, m_cc_en);
        }
        break;
    default:
        assert(false);
        break;
    }
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

