// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
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
//    Global constants
//

constexpr __vConstFloat vConst0p8373(CREG_IDX_0P837300003);
constexpr __vConstFloat vConstFloatPrgm0(CREG_IDX_PRGM1);
constexpr __vConstFloat vConstFloatPrgm1(CREG_IDX_PRGM2);
constexpr __vConstFloat vConstFloatPrgm2(CREG_IDX_PRGM3);

constexpr __vConstIntBase vConstTileId(CREG_IDX_TILEID);
constexpr __vConstIntBase vConstIntPrgm0(CREG_IDX_PRGM1);
constexpr __vConstIntBase vConstIntPrgm1(CREG_IDX_PRGM2);
constexpr __vConstIntBase vConstIntPrgm2(CREG_IDX_PRGM3);

//
//    __vDReg
//

inline __vCond __vDReg::operator==(float x) const {
    return __vCond(__vCond::__vCondEQ, vFloat(*this), x); 
}

inline __vCond __vDReg::operator!=(float x) const { 
    return __vCond(__vCond::__vCondNE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<(float x) const { 
    return __vCond(__vCond::__vCondLT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<=(float x) const { 
    return __vCond(__vCond::__vCondLTE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>(float x) const { 
    return __vCond(__vCond::__vCondGT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>=(float x) const { 
    return __vCond(__vCond::__vCondGTE, vFloat(*this), x); 
}

template <>
inline vFloat __vDReg::operator=(vFloat vec) const {
    rvtt->sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

inline vFloat __vDReg::operator=(double d) const {
    vFloat v(static_cast<float>(d));
    *this = v;
    return v;
}

inline vFloat __vDReg::operator=(s2vFloat16 f) const {
    vFloat v(f);
    *this = v;
    return v;
}

inline vFloat __vDReg::operator=(const float f) const {
    vFloat v(f);
    *this = v;
    return v;
}

OP_TEMPLATE_IMPL(vecType, __vBase)
inline vecType __vDReg::operator=(vecType vec) const {
    rvtt->sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

inline void __vDReg::operator=(__vDReg dreg) const {
    vFloat tmp = dreg;
    rvtt->sfpstore(tmp.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

inline vFloat __vDReg::operator=(const __vConstFloat creg) const {
    Vec lr = rvtt->sfpassignlreg(creg.get());
    rvtt->sfpstore(lr, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vFloat(lr);
}

//
//    vFloat
//

inline vFloat::vFloat(__vDReg dreg) {
    v = rvtt->sfpload(SFPLOAD_MOD0_FMT_SRCB, SFPLOAD_ADDR_MODE_NOINC, dreg.get());
    initialized = true;
}

inline __vCond vFloat::operator==(float x) const { 
    return __vCond(__vCond::__vCondEQ, *this, x); 
}

inline __vCond vFloat::operator!=(float x) const { 
    return __vCond(__vCond::__vCondNE, *this, x); 
}

inline __vCond vFloat::operator<(float x) const { 
    return __vCond(__vCond::__vCondLT, *this, x); 
}

inline __vCond vFloat::operator<=(float x) const { 
    return __vCond(__vCond::__vCondLTE, *this, x); 
}

inline __vCond vFloat::operator>(float x) const { 
    return __vCond(__vCond::__vCondGT, *this, x); 
}

inline __vCond vFloat::operator>=(float x) const { 
    return __vCond(__vCond::__vCondGTE, *this, x); 
}

inline vFloat vFloat::operator-=(vFloat a) {
    Vec neg1 = rvtt->sfpassignlreg(vConstNeg1.get());
    assign(rvtt->sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

inline void vFloat::loadf(float val) {
    assign(rvtt->sfpxloadi(SFPXLOADI_MOD0_FLOAT, __f32asui(val)));
}

//
//    __vIntBase
//

OP_TEMPLATE_IMPL(vType, __vIntBase)
inline vType __vIntBase::operator^(vType b) const {
    return rvtt->sfpxor(v, b.get());
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
inline vType __vIntBase::operator^=(vType b) {
    v = rvtt->sfpxor(v, b.get());
    return v;
}

inline void __vConstFloat::operator=(float in) const {
    vFloat tmp = in;
    rvtt->sfpconfig_v(tmp.get(), get());
}

inline void __vConstFloat::operator=(s2vFloat16 in) const {
    vFloat tmp = in;
    rvtt->sfpconfig_v(tmp.get(), get());
}

inline void __vConstIntBase::operator=(int in) const {
    vInt tmp = in;
    rvtt->sfpconfig_v(tmp.get(), get());
}

inline void __vIntBase::loadsi(int32_t val) {
    assign(rvtt->sfpxloadi(SFPXLOADI_MOD0_INT32, val));
}

inline void __vIntBase::loadui(uint32_t val) {
    assign(rvtt->sfpxloadi(SFPXLOADI_MOD0_UINT32, val));
}

//
//    Lreg indices
//

enum class LRegs {
    LReg0 = 0,
    LReg1 = 1,
    LReg2 = 2,
    LReg3 = 3,
    LReg4 = 4,
    LReg5 = 5,
    LReg6 = 6,
    LReg7 = 7,
    LRegCount = SFP_LREG_COUNT,
};

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

