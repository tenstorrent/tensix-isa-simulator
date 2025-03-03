// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>
#include <string>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

namespace {

unsigned int cmp_ex_to_setcc_mod1_map[] = {
    0,
    SFPSETCC_MOD1_LREG_LT0,
    SFPSETCC_MOD1_LREG_EQ0,
    SFPSETCC_MOD1_LREG_GTE0,
    SFPSETCC_MOD1_LREG_NE0,
}; 

} // namespace

//
//    Code
//

void Code::expand_sfpassignlreg(uint32_t ret, uint32_t lr) {
    // TODO
    // SET_REGNO(ret, SFPU_REG_FIRST + lr);
    m_lreg_map.bind(ret, lr);
    push_node(Op::SFPASSIGNLREG, ret, lr);
}

void Code::expand_sfpload(uint32_t ret, uint32_t mod, uint32_t mode, uint32_t addr) {
    push_node(Op::SFPLOAD, ret, mod, mode, clamp_unsigned(addr, 0x3FFF));
}

void Code::expand_sfpxloadi(uint32_t ret, uint32_t mod, uint32_t imm) {
    if ((mod & SFPXLOADI_MOD0_32BIT_MASK) != 0) {
        // Early nonimm pass assures this
        bool load_32bit = true;
        uint32_t new_mod;

        switch (mod) {
        case SFPXLOADI_MOD0_INT32:
            // This gets interesting since we can do a signed load of a 16 bit
            // positive integer by using an unsigned load to fill the upper bits with 0s
            if (!(int(imm) > 32767 || int(imm) < -32768)) {
                new_mod = SFPLOADI_MOD0_SHORT;
                load_32bit = false;
            } else if (int(imm) >= 0 && int(imm) < 65536) {
                new_mod = SFPLOADI_MOD0_USHORT;
                load_32bit = false;
            }
            break;

        case SFPXLOADI_MOD0_UINT32:
            if (imm <= 0xFFFF) {
                new_mod = SFPLOADI_MOD0_USHORT;
                load_32bit = false;
            }
            break;

        case SFPXLOADI_MOD0_FLOAT:
            {
                uint32_t man = imm & 0x007FFFFF;
                int exp = ((imm >> 23) & 0xFF) - 127;

                if ((man & 0xFFFF) == 0) {
                    // Fits in fp16b
                    load_32bit = false;
                    new_mod = SFPLOADI_MOD0_FLOATB;
                    imm = fp32_to_fp16b(imm);
                } else if ((man & 0x1FFF) == 0 && exp < 16 && exp >= -14) {
                    // Fits in fp16a
                    load_32bit = false;
                    new_mod = SFPLOADI_MOD0_FLOATA;
                    imm = fp32_to_fp16a(imm);
                }
            }
            break;

        default:
            assert(false);
            break;
        }

        if (load_32bit) {
            push_node(Op::SFPLOADI, ret, SFPLOADI_MOD0_UPPER, imm >> 16);
            push_node(Op::SFPLOADI, ret, SFPLOADI_MOD0_LOWER, imm & 0xFFFF);
        } else {
            push_node(Op::SFPLOADI, ret, new_mod, imm);
        }

    } else {
        push_node(Op::SFPLOADI, ret, mod, clamp_signed(imm, 0x7FFF));
    }
}

void Code::expand_sfpstore(uint32_t src, uint32_t mod, uint32_t mode, uint32_t addr) {
    push_node(Op::SFPSTORE, RET_NONE, src, mod, mode, clamp_unsigned(addr, 0x3FFF));
}

void Code::expand_sfpshft_i(uint32_t ret, uint32_t dst, uint32_t imm) {
    push_node(Op::SFPSHFT_I, ret, dst, clamp_signed(imm, 0x7FF));
}

//
//    SKIPPED: expand_sfpiadd_i (unused?)
//

//
//    Extended (or external?) iadd_i
//    Handles:
//      - signed/unsigned immediate value
//      - >12 bits (>11 bits for unsigned)
//      - comparators: <, ==, !=, >=, <=, >
//      - use of SETCC vs IADD for perf
//
//    For comparisons:
//      compare  < 0 or >= 0  use setcc
//      compare == 0 or != 0  use setcc
//
//      <=, > use multiple instructions, <= uses a COMPC which relies on the
//      wrapper emitting a PUSHC as a "fence" for the COMPC when needed
//
//    Below, n is either not 0 or unknown
//      compare  < n or >= n  use iadd_i (subtract and compare)
//      compare == n or != n  use iadd_i and setcc (subtract then compare)
//
//    Note: wrapper/instruction combining cannot create the case where the op
//    is either <= n or > n and we care about the result.  The code below doesn't
//    handle it and if it did, the result would be inefficient.
//

void Code::expand_sfpxiadd_i(uint32_t ret, uint32_t src, uint32_t imm, uint32_t mod) {
    uint32_t cmp = mod & SFPXCMP_MOD1_CC_MASK;
    uint32_t base_mod = mod & ~SFPXCMP_MOD1_CC_MASK;

    // Decompose aggregate comparisons, recurse
    if (cmp == SFPXCMP_MOD1_CC_LTE || cmp == SFPXCMP_MOD1_CC_GT) {
        expand_sfpxiadd_i(ret, src, imm, base_mod | SFPXCMP_MOD1_CC_GTE);
        expand_sfpxiadd_i(ret, ret, 0, base_mod | SFPXCMP_MOD1_CC_NE);
        if (cmp == SFPXCMP_MOD1_CC_LTE) {
            push_node(Op::SFPCOMPC, RET_NONE);
        }
        return;
    }

    bool need_loadi = true;
    bool is_signed = ((mod & SFPXIADD_MOD1_SIGNED) == SFPXIADD_MOD1_SIGNED);
    bool is_12bits = ((mod & SFPXIADD_MOD1_12BIT) != 0);
    bool is_sub = ((mod & SFPXIADD_MOD1_IS_SUB) != 0);
    bool dst_unused = ((mod & SFPXIADD_MOD1_DST_UNUSED) != 0);
    int iv = int(imm);

    // Figure out if we need to do a loadi (>12 bits signed)
    iv = is_sub ? -iv : iv;
    if (!(iv >= 2048 || iv < -2048)) {
        need_loadi = false;
        imm = uint32_t(iv);
    }

    uint32_t set_cc_arg = src;

    bool need_setcc = ((cmp & SFPXCMP_MOD1_CC_MASK) != 0);
    if (need_loadi) {
        // Load imm into dst
        uint32_t loadi_mod = is_signed ? SFPXLOADI_MOD0_INT32 : SFPXLOADI_MOD0_UINT32;
        expand_sfpxloadi(ret, loadi_mod, imm);

        uint32_t mod1 = is_sub ? SFPIADD_MOD1_ARG_2SCOMP_LREG_DST : SFPIADD_MOD1_ARG_LREG_DST;
        if (cmp == SFPXCMP_MOD1_CC_LT || cmp == SFPXCMP_MOD1_CC_GTE) {
            // Perform op w/ compare
            mod1 |= (cmp == SFPXCMP_MOD1_CC_LT) ? SFPIADD_MOD1_CC_LT0 : SFPIADD_MOD1_CC_GTE0;
            push_node(Op::SFPIADD_V, ret, ret, src, mod1);
            need_setcc = false;
        } else {
            // Perform op w/o compare, compare with SETCC
            mod1 |= SFPIADD_MOD1_CC_NONE;
            push_node(Op::SFPIADD_V, ret, ret, src, mod1);
            set_cc_arg = ret;
        }
    } else {
        if (iv != 0) {
            if (cmp == SFPXCMP_MOD1_CC_LT || cmp == SFPXCMP_MOD1_CC_GTE) {
                // Perform op w/ compare
                uint32_t mod1 = 
                    (cmp == SFPXCMP_MOD1_CC_LT) ? 
                        SFPIADD_MOD1_CC_LT0 : SFPIADD_MOD1_CC_GTE0;
                push_node(Op::SFPIADD_I, ret, src, imm, mod1 | SFPIADD_MOD1_ARG_IMM);
                need_setcc = false;
            } else {
                // Perform op w/o compare
                uint32_t mod1 = SFPIADD_MOD1_ARG_IMM | SFPIADD_MOD1_CC_NONE;
                push_node(Op::SFPIADD_I, ret, src, imm, mod1);
                set_cc_arg = ret;
            }
        } else if (!dst_unused) {
            // An add or subtract against 0 isn't particularly interesting, but
            // we need to keep the register usage correct since dst is now src
            uint32_t mod1 = SFPIADD_MOD1_ARG_IMM | SFPIADD_MOD1_CC_NONE;
            push_node(Op::SFPIADD_I, ret, src, imm, mod1);
        }

        if (need_setcc) {
            push_node(Op::SFPSETCC_V, RET_NONE, set_cc_arg, cmp_ex_to_setcc_mod1_map[cmp]); 
        }
    }
}

//
//    See comment block above at expand_sfpxiadd_i
//

void Code::expand_sfpxiadd_v(uint32_t ret, uint32_t dst, uint32_t src, uint32_t mod) {
    uint32_t cmp = mod & SFPXCMP_MOD1_CC_MASK;
    uint32_t base_mod = mod & ~SFPXCMP_MOD1_CC_MASK;

    // Decompose aggregate comparisons, recurse
    if (cmp == SFPXCMP_MOD1_CC_LTE || cmp == SFPXCMP_MOD1_CC_GT) {
        expand_sfpxiadd_v(ret, dst, src, base_mod | SFPXCMP_MOD1_CC_GTE);
        push_node(Op::SFPSETCC_V, RET_NONE, ret, SFPSETCC_MOD1_LREG_NE0);
        if (cmp == SFPXCMP_MOD1_CC_LTE) {
            push_node(Op::SFPCOMPC, RET_NONE);
        }
        return;
    }

    bool is_sub = ((mod & SFPXIADD_MOD1_IS_SUB) != 0);
    uint32_t mod1 = is_sub ? SFPIADD_MOD1_ARG_2SCOMP_LREG_DST : SFPIADD_MOD1_ARG_LREG_DST;

    if (cmp == SFPXCMP_MOD1_CC_LT || cmp == SFPXCMP_MOD1_CC_GTE) {
        // Perform op w/ compare
        mod1 |= (cmp == SFPXCMP_MOD1_CC_LT) ? SFPIADD_MOD1_CC_LT0 : SFPIADD_MOD1_CC_GTE0;
        push_node(Op::SFPIADD_V, ret, dst, src, mod1);
    } else {
        // Perform op w/o compare
        mod1 |= SFPIADD_MOD1_CC_NONE;
        push_node(Op::SFPIADD_V, ret, dst, src, mod1);
        if (cmp != 0) {
            // Must be EQ0 or NE0, compare with SETCC
            push_node(Op::SFPSETCC_V, RET_NONE, ret, cmp_ex_to_setcc_mod1_map[cmp]);
        }
    }
}

void Code::expand_sfpxfcmps(uint32_t v, uint32_t f, uint32_t mod) {
    bool need_sub = false;
    uint32_t ref_val = vreg_alloc();

    // Wrapper will convert 0 to -0
    uint32_t fmt = mod & SFPXSCMP_MOD1_FMT_MASK;
    if (f != 0 &&
            ((fmt != SFPXSCMP_MOD1_FMT_FLOAT && f != 0x80000000) ||
                (fmt == SFPXSCMP_MOD1_FMT_FLOAT && f != 0x8000))) {
        need_sub = true;
        if ((fmt == SFPXSCMP_MOD1_FMT_FLOAT && f == 0x3f800000) ||
                (fmt != SFPXSCMP_MOD1_FMT_FLOAT && f == 0x3f80)) {
            expand_sfpassignlreg(ref_val, CREG_IDX_1);
        } else if ((fmt == SFPXSCMP_MOD1_FMT_FLOAT && f == 0xbf800000) ||
                (fmt != SFPXSCMP_MOD1_FMT_FLOAT && f == 0xbf80)) {
            expand_sfpassignlreg(ref_val, CREG_IDX_NEG_1);
        } else {
            expand_sfpxloadi(ref_val, scmp2loadi_mod(fmt), f);
        }
    }

    uint32_t cmp = mod & SFPXCMP_MOD1_CC_MASK;
    uint32_t setcc_mod = cmp_ex_to_setcc_mod1_map[cmp];
    if (need_sub) {
        uint32_t neg_one = vreg_alloc();
        uint32_t tmp = vreg_alloc();
        expand_sfpassignlreg(neg_one, CREG_IDX_NEG_1);
        push_node(Op::SFPMAD, tmp, ref_val, neg_one, v, 0);
        v = tmp;
    }

    if (cmp == SFPXCMP_MOD1_CC_LTE || cmp == SFPXCMP_MOD1_CC_GT) {
        push_node(Op::SFPSETCC_V, RET_NONE, v, SFPSETCC_MOD1_LREG_GTE0);
        push_node(Op::SFPSETCC_V, RET_NONE, v, SFPSETCC_MOD1_LREG_NE0);
        if (cmp == SFPXCMP_MOD1_CC_LTE) {
            push_node(Op::SFPCOMPC, RET_NONE);
        }
    } else {
        push_node(Op::SFPSETCC_V, RET_NONE, v, setcc_mod);
    }
}

//
//    Compare two vectors by subtracting v2 from v1 and doing a setcc
//

void Code::expand_sfpxfcmpv(uint32_t v1, uint32_t v2, uint32_t mod) {
    uint32_t tmp = vreg_alloc();
    uint32_t neg1 = vreg_alloc();

    expand_sfpassignlreg(neg1, CREG_IDX_NEG_1);
    push_node(Op::SFPMAD, tmp, v2, neg1, v1, 0);

    uint32_t cmp = mod & SFPXCMP_MOD1_CC_MASK;
    if (cmp == SFPXCMP_MOD1_CC_LTE || cmp == SFPXCMP_MOD1_CC_GT) {
        push_node(Op::SFPSETCC_V, RET_NONE, tmp, SFPSETCC_MOD1_LREG_GTE0);
        push_node(Op::SFPSETCC_V, RET_NONE, tmp, SFPSETCC_MOD1_LREG_NE0);
        if (cmp == SFPXCMP_MOD1_CC_LTE) {
            push_node(Op::SFPCOMPC, RET_NONE);
        }
    } else {
        push_node(Op::SFPSETCC_V, RET_NONE, tmp, cmp_ex_to_setcc_mod1_map[mod]);
    }
}

void Code::expand_sfpdivp2(uint32_t ret, uint32_t imm, uint32_t src, uint32_t mod) {
    push_node(Op::SFPDIVP2, ret, clamp_signed(imm, 0x7FF), src, mod);
} 
 
void Code::expand_sfpstochrnd_i(
        uint32_t ret, 
        uint32_t mode, 
        uint32_t imm, 
        uint32_t src, 
        uint32_t mod) {
    push_node(Op::SFPSTOCHRND_I, ret, mode, clamp_unsigned(imm, 0x1F), src, mod);
}

void Code::expand_sfpsetexp_i(uint32_t ret, uint32_t imm, uint32_t src) {
    push_node(Op::SFPSETEXP_I, ret, clamp_unsigned(imm, 0xFFF), src);
}

void Code::expand_sfpsetsgn_i(uint32_t ret, uint32_t imm, uint32_t src) {
    push_node(Op::SFPSETSGN_I, ret, clamp_unsigned(imm, 0xFFF), src);
}

void Code::expand_sfpsetman(uint32_t ret, uint32_t imm, uint32_t src) {
    if (imm > 4095) {
        expand_sfpxloadi(ret, SFPXLOADI_MOD0_UINT32, imm);
        push_node(Op::SFPSETMAN_V, ret, ret, src);
    } else {
        push_node(Op::SFPSETMAN_I, ret, imm, src);
    }
} 

void Code::expand_sfpshft2_e(uint32_t ret, uint32_t src, uint32_t mod) {
    // This routine handles a subset of mod values that all require a NOP
    assert(mod == 3 || mod == 4);

    if (mod == 4) {
        // WH_B0 HW bug (issue #3240): the shftr version of the insn doesn't set the
        // value shifted into place to 0 but instead uses the previous value (eg,
        // from a ror) Here we clear that value to 0 by rotating in the 0 register
        // Optimization potential to not do this if the previous insn was a shftr
        uint32_t lreg9 = vreg_alloc();
        // TODO (lreg[9] = 0)
        // SET_REGNO(lreg9, SFPU_REG_FIRST + 9);
        m_lreg_map.bind(lreg9, 9);
        push_node(Op::SFPSHFT2_E, lreg9, lreg9, 3); 
    }

    push_node(Op::SFPSHFT2_E, ret, src, mod);
} 

//
//    Helper methods
//

uint32_t Code::clamp_signed(uint32_t v, uint32_t mask) {
    uint32_t out = v & mask;
    if (v & (mask + 1)) {
        out |= ~mask;
    }
    return out;
}

uint32_t Code::clamp_unsigned(uint32_t v, uint32_t mask) {
    return v & mask;
}

uint32_t Code::fp32_to_fp16a(uint32_t val) {
    // https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
    // Handles denorms.  May be costly w/ non-immediate values
    uint32_t b = val + 0x00001000;
    uint32_t e = (b & 0x7F800000) >> 23;
    uint32_t m = b & 0x007FFFFF;
    uint32_t result =
       (b & 0x80000000) >> 16 |
       (e > 112) * ((((e - 112) << 10) &0x7C00) | m >> 13) |
       ((e < 113) & (e > 101)) * ((((0x007FF000 + m) >> (125 - e)) + 1) >> 1) |
       (e > 143) * 0x7FFF;
#if 0
    // Simple/faster but less complete
    const unsigned int result =
       ((val >> 16) & 0x8000) |
       ((((val & 0x7F800000) - 0x38000000) >> 13) & 0x7c00) |
       ((val >> 13) & 0x03FF);
#endif
    return result;
}

uint32_t Code::fp32_to_fp16b(uint32_t val) {
    return val >> 16;
} 

uint32_t Code::scmp2loadi_mod(uint32_t mod) {
    uint32_t fmt = mod & SFPXSCMP_MOD1_FMT_MASK;
    if (fmt == SFPXSCMP_MOD1_FMT_A) {
        return SFPLOADI_MOD0_FLOATA;
    }
    if (fmt == SFPXSCMP_MOD1_FMT_B) {
        return SFPLOADI_MOD0_FLOATB;
    }
    return SFPXLOADI_MOD0_FLOAT;
} 

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

