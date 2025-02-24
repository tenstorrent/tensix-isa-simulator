// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
//    Implementation elements specific for Wormhole B0
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Functional math library
//

inline vFloat lut(vFloat v, vUInt l0, vUInt l1, vUInt l2) {
    return rvtt->sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_RETAIN);
}

inline vFloat lut_sign(vFloat v, vUInt l0, vUInt l1, vUInt l2) {
    return rvtt->sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_UPDATE);
}

inline vFloat lut2(vFloat v, vUInt l0, vUInt l1, vUInt l2) {
    uint32_t mod0 = SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN;
    return rvtt->sfplutfp32_3r(l0.get(), l1.get(), l2.get(), v.get(), mod0);
}

inline vFloat lut2_sign(vFloat v, vUInt l0, vUInt l1, vUInt l2) {
    uint32_t mod0 = SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE;
    return rvtt->sfplutfp32_3r(l0.get(), l1.get(), l2.get(), v.get(), mod0);
}

inline vFloat lut2(
        vFloat v, 
        vFloat a0, 
        vFloat a1, 
        vFloat a2, 
        vFloat b0, 
        vFloat b1, 
        vFloat b2) {
    uint32_t mod0 = SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN;
    return rvtt->sfplutfp32_6r(
        a0.get(), 
        a1.get(), 
        a2.get(), 
        b0.get(), 
        b1.get(), 
        b2.get(), 
        v.get(), 
        mod0);
}

inline vFloat lut2_sign(
        vFloat v, 
        vFloat a0, 
        vFloat a1, 
        vFloat a2, 
        vFloat b0, 
        vFloat b1, 
        vFloat b2) {
    uint32_t mod0 = SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE;
    return rvtt->sfplutfp32_6r(
        a0.get(), 
        a1.get(), 
        a2.get(),
        b0.get(), 
        b1.get(), 
        b2.get(),
        v.get(), 
        mod0);
}

inline vFloat lut2(
        vFloat v,
        vUInt a01, 
        vUInt a23, 
        vUInt a45,
        vUInt b01, 
        vUInt b23, 
        vUInt b45, 
        int mode = 1) {
    uint32_t mod0 = 
        (mode == 1) ? 
            SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : 
            SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    mod0 |= SFPLUTFP32_MOD0_SGN_RETAIN;
    return rvtt->sfplutfp32_6r(
        a01.get(), 
        a23.get(), 
        a45.get(),
        b01.get(), 
        b23.get(), 
        b45.get(),
        v.get(), 
        mod0);
}

inline vFloat lut2_sign(
        vFloat v,
        vUInt a01, 
        vUInt a23, 
        vUInt a45,
        vUInt b01, 
        vUInt b23, 
        vUInt b45, 
        int mode = 1) {
    uint32_t mod0 = 
        (mode == 1) ? 
            SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : 
            SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    mod0 |= SFPLUTFP32_MOD0_SGN_UPDATE;
    return rvtt->sfplutfp32_6r(
        a01.get(), 
        a23.get(), 
        a45.get(),
        b01.get(), 
        b23.get(), 
        b45.get(),
        v.get(), 
        mod0);
}

inline vInt exexp(vFloat v) {
    return rvtt->sfpexexp(v.get(), SFPEXEXP_MOD1_DEBIAS);
}

inline vInt exexp_nodebias(vFloat v) {
    return rvtt->sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

inline vInt exman8(vFloat v) {
    return rvtt->sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

inline vInt exman9(vFloat v) {
    return rvtt->sfpexman(v.get(), SFPEXMAN_MOD1_PAD9);
}

inline vFloat setexp(vFloat v, uint32_t exp) {
    return rvtt->sfpsetexp_i(exp, v.get());
}

inline vFloat setexp(vFloat v, __vIntBase exp) {
    // Odd: dst is both exponent and result so undergoes a type change
    // If exp is not used later, compiler renames tmp and doesn't issue a mov
    return rvtt->sfpsetexp_v(exp.get(), v.get());
}

inline vFloat setman(vFloat v, uint32_t man) {
    return rvtt->sfpsetman_i(man, v.get(), 0);
}

inline vFloat setman(vFloat v, __vIntBase man) {
    // Grayskull HW bug, is this useful?  Should there be a "Half" form?
    // Odd: dst is both man and result so undergoes a type change
    // If man is not used later, compiler renames tmp and doesn't issue a mov
    return rvtt->sfpsetman_v(man.get(), v.get());
}

inline vFloat addexp(vFloat in, int32_t exp) {
    return rvtt->sfpdivp2(exp, in.get(), SFPSDIVP2_MOD1_ADD);
}

OP_TEMPLATE_DECL(vType, __vBase)
inline vType setsgn(vType v, int32_t sgn) {
    return rvtt->sfpsetsgn_i(sgn, v.get());
}

template <typename vTypeA, typename vTypeB,
    typename std::enable_if_t<std::is_base_of<__vBase, vTypeA>::value> * = nullptr,
    typename std::enable_if_t<std::is_base_of<__vBase, vTypeB>::value> * = nullptr>
inline vTypeA setsgn(vTypeA v, vTypeB sgn) {
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return rvtt->sfpsetsgn_v(sgn.get(), v.get());
}

OP_TEMPLATE_DECL(vType, __vBase)
inline vType setsgn(vType v, vInt sgn) {
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return rvtt->sfpsetsgn_v(sgn.get(), v.get());
}

OP_TEMPLATE_DECL(vType, __vBase)
inline vInt lz(vType v) {
    return vInt(rvtt->sfplz(v.get(), SFPLZ_MOD1_CC_NONE));
}

OP_TEMPLATE_DECL(vType, __vBase)
inline vInt lz_nosgn(vType v) {
    return vInt(rvtt->sfplz(v.get(), SFPLZ_MOD1_NOSGN_CC_NONE));
}

inline vFloat abs(vFloat v) {
    return rvtt->sfpabs(v.get(), SFPABS_MOD1_FLOAT);
}

inline vInt abs(vInt v) {
    return rvtt->sfpabs(v.get(), SFPABS_MOD1_INT);
}

inline vUInt shft(vUInt v, vInt amt) {
    return rvtt->sfpshft_v(v.get(), amt.get());
}

OP_TEMPLATE_DECL(vType, __vBase)
inline vType reinterpret(__vBase v) {
    return vType(v.get());
}

inline vFloat int32_to_float(vInt in, int round_mode = 1) {
    uint32_t mod1 = round_mode ? SFPCAST_MOD1_RND_STOCH : SFPCAST_MOD1_RND_EVEN;
    return rvtt->sfpcast(in.get(), mod1);
}

inline vUInt float_to_fp16a(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_FP16A;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline vUInt float_to_fp16b(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_FP16B;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline vUInt float_to_uint8(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_UINT8;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline vUInt float_to_int8(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_INT8;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline vUInt int32_to_uint8(vInt in, vUInt descale, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_INT32_TO_UINT8;
    return rvtt->sfpstochrnd_v(rnd, descale.get(), in.get(), mod1);
}

inline vUInt int32_to_uint8(vInt in, unsigned int descale, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_INT32_TO_UINT8 | SFPSTOCHRND_MOD1_IMM8;
    return rvtt->sfpstochrnd_i(rnd, descale, in.get(), mod1);
}

inline vUInt int32_to_int8(vInt in, vUInt descale, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_INT32_TO_INT8;
    return rvtt->sfpstochrnd_v(rnd, descale.get(), in.get(), mod1);
}

inline vUInt int32_to_int8(vInt in, unsigned int descale, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_INT32_TO_INT8 | SFPSTOCHRND_MOD1_IMM8;
    return rvtt->sfpstochrnd_i(rnd, descale, in.get(), mod1);
}

inline vUInt float_to_uint16(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_UINT16;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline vUInt float_to_int16(vFloat in, int round_mode = 1) {
    uint32_t rnd = round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN;
    uint32_t mod1 = SFPSTOCHRND_MOD1_FP32_TO_INT16;
    return rvtt->sfpstochrnd_i(rnd, 0, in.get(), mod1);
}

inline void subvec_transp(__vBase &l0, __vBase &l1, __vBase &l2, __vBase &l3) {
    rvtt->sfptransp(l0.get(), l1.get(), l2.get(), l3.get());
}

inline Vec subvec_shflror1(const __vBase &src) {
    return rvtt->sfpshft2_e(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLROR1);
}

inline Vec subvec_shflshr1(const __vBase &src) {
    return rvtt->sfpshft2_e(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLSHR1);
}

inline void vec_swap(__vBase &dst, __vBase &src) {
    rvtt->sfpswap(dst.get(), src.get(), SFPSWAP_MOD1_SWAP);
}

inline void vec_min_max(__vBase &dst, __vBase &src) {
    rvtt->sfpswap(dst.get(), src.get(), SFPSWAP_MOD1_VEC_MIN_MAX);
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

