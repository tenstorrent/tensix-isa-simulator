// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <type_traits>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/sfpi_fp16.hpp"

#define OP_TEMPLATE_DECL(TYPE, BASE) \
    template<typename TYPE, \
        typename std::enable_if_t<std::is_base_of<BASE, TYPE>::value> * = nullptr>

#define OP_TEMPLATE_IMPL(TYPE, BASE) \
    template<typename TYPE, \
        typename std::enable_if_t<std::is_base_of<BASE, TYPE>::value> *>

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Forward
//

// interface

class vFloat;
class vInt;
class vUInt;
enum class LRegs;

// internal

class __vBase;
class __vIntBase;
class __vConstFloat;
class __vConstIntBase;
class __vCond;
class __vCCCtrl;
class __vCCCtrlBase;

//
//    Helpers
//

inline uint32_t __f32asui(float val) {
    union {
        float f;
        uint32_t i;
    } tmp;
    tmp.f = val;
    return tmp.i;
}

//
//    __RegFile
//

template<typename Type, int N>
class __RegFile {
public:
    Type operator[](int x) const;
};

//
//    __vRegBase
//

class __vRegBase {
public:
    constexpr explicit __vRegBase(int r): 
        reg(r) { }
public:
    int get() const { 
        return reg; 
    }
protected:
    int reg;
};

//
//    __vRegBaseInitializer
//

class __vRegBaseInitializer {
public:
    constexpr __vRegBaseInitializer(int in): 
        n(in) { }
public:
    constexpr int get() const { 
        return n; 
    }
private:
    int n;
};

//
//    __vDReg
//

class __vDReg: public __vRegBase {
private:
    template<class Type, int N> friend class __RegFile;
private:
    constexpr explicit __vDReg(__vRegBaseInitializer i): 
        __vRegBase(i.get() * SFP_DESTREG_STRIDE) { }
public:
    // Assign register to register
    OP_TEMPLATE_DECL(vecType, __vBase)
        vecType operator=(vecType vec) const;
    void operator=(__vDReg dreg) const;
    vFloat operator=(__vConstFloat creg) const;
    vFloat operator=(s2vFloat16 f) const;
    vInt operator=(int i) const;
    vUInt operator=(unsigned int i) const;
    vFloat operator=(float f) const;
    vFloat operator=(double d) const;
public:
    // Construct operator classes from operations
    vFloat operator+(vFloat b) const;
    vFloat operator-(vFloat b) const;
    vFloat operator-() const;
    vFloat operator*(vFloat b) const;
public:
    // Conditionals
    __vCond operator==(float x) const;
    __vCond operator!=(float x) const;
    __vCond operator<(float x) const;
    __vCond operator<=(float x) const;
    __vCond operator>(float x) const;
    __vCond operator>=(float x) const;
public:
    __vCond operator==(s2vFloat16 x) const;
    __vCond operator!=(s2vFloat16 x) const;
    __vCond operator<(s2vFloat16 x) const;
    __vCond operator<=(s2vFloat16 x) const;
    __vCond operator>(s2vFloat16 x) const;
    __vCond operator>=(s2vFloat16 x) const;
public:
    __vCond operator==(vFloat x) const;
    __vCond operator!=(vFloat x) const;
    __vCond operator<(vFloat x) const;
    __vCond operator<=(vFloat x) const;
    __vCond operator>(vFloat x) const;
    __vCond operator>=(vFloat x) const;
};

//
//    __DestReg
//

class __DestReg {
public:
    __vDReg operator[](int i) const { 
        return dreg[i]; 
    }
    // Make these void - ugly as these aren't really inc/dec
    void operator++() const { 
        rvtt->sfpincrwc(0, SFP_DESTREG_STRIDE, 0, 0); 
    }
    void operator++(int) const { 
        rvtt->sfpincrwc(0, SFP_DESTREG_STRIDE, 0, 0); 
    }
    void operator+=(int i) const { 
        rvtt->sfpincrwc(0, SFP_DESTREG_STRIDE * i, 0, 0); 
    }
private:
    __RegFile<__vDReg, SFP_DESTREG_MAX_ADDR> dreg;
};

//
//    __vLReg
//

class __vLReg: public __vRegBase {
private:
    template<class Type, int N> friend class __RegFile;
private:
    constexpr explicit __vLReg(__vRegBaseInitializer i): 
        __vRegBase(i.get()) { }
public:
    Vec operator=(__vBase &v) const;
};

//
//    __LReg
//

class __LReg {
public:
    __vLReg operator[](enum LRegs lr) const { 
        return lreg[static_cast<std::underlying_type<LRegs>::type>(lr)]; 
    }
private:
    __RegFile<__vLReg, SFP_LREG_COUNT> lreg;
};

//
//    __vConstFloat
//

class __vConstFloat: public __vRegBase {
public:
    constexpr explicit __vConstFloat(int r) : __vRegBase(r) {}
public:
    void operator=(float in) const;
    void operator=(s2vFloat16 in) const;
public:
    // Construct operator classes from operations
    vFloat operator+(vFloat b) const;
    vFloat operator-(vFloat b) const;
    vFloat operator*(vFloat b) const;
public:
    __vCond operator==(vFloat x) const;
    __vCond operator!=(vFloat x) const;
    __vCond operator<(vFloat x) const;
    __vCond operator<=(vFloat x) const;
    __vCond operator>(vFloat x) const;
    __vCond operator>=(vFloat x) const;
};

//
//    __vConstIntBase
//

class __vConstIntBase: public __vRegBase {
public:
    constexpr explicit __vConstIntBase(int r): 
        __vRegBase(r) { }
public:
    void operator=(int in) const;
    // Construct operator classes from operations
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator+(vType b) const;
    vInt operator+(int32_t b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
       vType operator-(vType b) const;
    vInt operator-(int32_t b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator&(vType b) const;
    vInt operator&(int32_t b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator|(vType b) const;
    vInt operator|(int32_t b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator^(vType b) const;
    vInt operator^(int32_t b) const;
    __vCond operator==(vInt x) const;
    __vCond operator!=(vInt x) const;
    __vCond operator<(vInt x) const;
    __vCond operator<=(vInt x) const;
    __vCond operator>(vInt x) const;
    __vCond operator>=(vInt x) const;
    // Shifts
    __vIntBase operator<<(uint32_t amt) const;
    vUInt operator>>(uint32_t amt) const;
};

//
//    __vBase
//

class __vBase {
public:
    __vBase(): 
        initialized(false) { }
public:
    Vec get() const { 
        return v; 
    }
    Vec &get() { 
        return v; 
    }
    // Associate variable w/ a value pre-loaded into a particular lreg
    void operator=(__vLReg lr);
protected:
    void assign(Vec t);
protected:
    bool initialized;
    Vec v;
};

//
//    vFloat
//

class vFloat: public __vBase {
public:
    vFloat() = default;
public:
    vFloat(__vDReg dreg);
    vFloat(__vConstFloat creg);
    vFloat(s2vFloat16 f) { 
        loadf16(f); 
    }
    vFloat(float f) { 
        loadf(f); 
    }
    vFloat(Vec t) { 
        assign(t); 
    }
    vFloat(__vLReg lr) { 
        __vBase::operator=(lr); 
    }
public:
    // Assignment
    vFloat operator=(vFloat in) { 
        assign(in.v); 
        return v; 
    }
    vFloat operator=(__vLReg lr) { 
        __vBase::operator=(lr); 
        return v; 
    }
public:
    // Construct operator from operations
    vFloat operator+(vFloat b) const;
    vFloat operator+=(vFloat);
    vFloat operator-(vFloat b) const;
    vFloat operator-(float b) const;
    vFloat operator-(s2vFloat16 b) const;
    vFloat operator-=(vFloat);
    vFloat operator-() const;
    vFloat operator*(vFloat b) const;
    vFloat operator*=(vFloat);
    vFloat operator++(int) { 
        *this += 1; 
        return *this; 
    }
    vFloat operator++() { 
        vFloat tmp = *this; 
        *this += 1; 
        return tmp; 
    }
    vFloat operator--(int) { 
        *this -= 1; 
        return *this; 
    }
    vFloat operator--() { 
        vFloat tmp = *this; 
        *this -= 1; 
        return tmp; 
    }
public:
    // Conditionals
    __vCond operator==(float x) const;
    __vCond operator==(s2vFloat16 x) const;
    __vCond operator==(vFloat x) const;
    __vCond operator!=(float x) const;
    __vCond operator!=(s2vFloat16 x) const;
    __vCond operator!=(vFloat x) const;
    __vCond operator<(float x) const;
    __vCond operator<(s2vFloat16 x) const;
    __vCond operator<(vFloat x) const;
    __vCond operator<=(float x) const;
    __vCond operator<=(s2vFloat16 x) const;
    __vCond operator<=(vFloat x) const;
    __vCond operator>(float x) const;
    __vCond operator>(s2vFloat16 x) const;
    __vCond operator>(vFloat x) const;
    __vCond operator>=(float x) const;
    __vCond operator>=(s2vFloat16 x) const;
    __vCond operator>=(vFloat x) const;
private:
    void loadf(float val);
    void loadf16(s2vFloat16 val);
};

//
//    __vIntBase
//

class __vIntBase: public __vBase {
public:
    __vIntBase() = default;
    __vIntBase(Vec in) { 
        assign(in); 
    }
    __vIntBase(__vConstIntBase creg);
public:
    OP_TEMPLATE_DECL(vType, __vBase)
        explicit operator vType() const { 
            return vType(v); 
        }
public:
    // Bit Operations
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator&(vType b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        void operator&=(vType b);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator|(vType b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator|=(vType b);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator^(vType b) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator^=(vType b);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator~() const;
public:
    // Arith
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType add(int32_t val, unsigned int mod) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator+(__vIntBase val) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator+(__vConstIntBase val) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType sub(int32_t val, unsigned int mod) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator-(__vIntBase val) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator-(__vConstIntBase val) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        void add_eq(int32_t val, unsigned int mod);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator+=(__vIntBase val);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator+=(__vConstIntBase val);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        void sub_eq(int32_t val, unsigned int mod);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator-=(__vIntBase val);
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator-=(__vConstIntBase val);
public:
    // Shifts
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator<<(uint32_t amt) const;
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vType operator<<=(uint32_t amt);
protected:
    void loadss(int16_t val);
    void loadus(uint16_t val);
    void loadsi(int32_t val);
    void loadui(uint32_t val);
};

//
//    vInt
//

class vInt: public __vIntBase {
    friend class vUInt;
public:
    vInt() = default;
    vInt(Vec in) { 
        assign(in); 
    }
    vInt(__vConstIntBase creg) { 
        v = rvtt->sfpassignlreg(creg.get()); 
        initialized = true; 
    }
    vInt(__vIntBase in) { 
        assign(in.get()); 
    };
    vInt(short val) { 
        loadss(val); 
    }
    vInt(int val) { 
        loadsi(val); 
    }
    vInt(unsigned short val) { 
        loadus(val); 
    }
    vInt(unsigned int val) { 
        loadui(val); 
    }
    vInt(__vLReg lr) { 
        __vBase::operator=(lr); 
    }
    vInt(__vCond vc);
public:
    // Assignment
    vInt operator=(vInt in) { 
        assign(in.v); 
        return v; 
    }
    vInt operator=(__vLReg lr) { 
        __vBase::operator=(lr); 
        return v; 
    }
public:
    // Operations
    vInt operator&(int32_t b) const { 
        return this->__vIntBase::operator&(vInt(b)); 
    }
    vInt operator&(vInt b) const { 
        return this->__vIntBase::operator&(b); 
    }
    vInt operator&=(vInt b) { 
        this->__vIntBase::operator&=(b); 
        return v; 
    }
    vInt operator|(int32_t b) const { 
        return this->__vIntBase::operator|(vInt(b)); 
    }
    vInt operator|(vInt b) const { 
        return this->__vIntBase::operator|(b); 
    }
    vInt operator|=(vInt b) { 
        this->__vIntBase::operator|=(b); 
        return v; 
    }
    vInt operator^(int32_t b) const { 
        return this->__vIntBase::operator^(vInt(b)); 
    }
    vInt operator^(vInt b) const { 
        return this->__vIntBase::operator^(b); 
    }
    vInt operator^=(vInt b) { 
        this->__vIntBase::operator^=(b); 
        return v; 
    }
    vInt operator~() const { 
        return this->__vIntBase::operator~<vInt>(); 
    }
    vInt operator+(int32_t val) const { 
        return this->__vIntBase::add<vInt>(val, SFPXIADD_MOD1_SIGNED); 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vInt operator+(vType val) const { 
            return this->__vIntBase::operator+<vInt>(val); 
        }
    vInt operator+(__vConstIntBase val) const { 
        return this->__vIntBase::operator+<vInt>(val); 
    }
    vInt operator-(int32_t val) const { 
        return this->__vIntBase::sub<vInt>(val, SFPXIADD_MOD1_SIGNED); 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vInt operator-(vType val) const { 
            return this->__vIntBase::operator-<vInt>(val); 
        }
    vInt operator-(__vConstIntBase val) const { 
        return this->__vIntBase::operator-<vInt>(val); 
    }
    vInt operator+=(int32_t val) { 
        this->__vIntBase::add_eq<vInt>(val, SFPXIADD_MOD1_SIGNED); 
        return v; 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vInt operator+=(vType val) { 
            this->__vIntBase::operator+=<vInt>(val); 
            return v; 
        }
    vInt operator+=(__vConstIntBase val) { 
        this->__vIntBase::operator-=<vInt>(val); 
        return v; 
    }
    vInt operator-=(int32_t val) { 
        this->__vIntBase::sub_eq<vInt>(val, SFPXIADD_MOD1_SIGNED); 
        return v; 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vInt operator-=(vType val) { 
            this->__vIntBase::operator-=<vInt>(val); 
            return v; 
        }
    vInt operator-=(__vConstIntBase val) { 
        this->__vIntBase::operator-=<vInt>(val); 
        return v; 
    }
    vInt operator++(int) { 
        *this += 1; 
        return *this; 
    }
    vInt operator++() { 
        vInt tmp = *this; 
        *this += 1; 
        return tmp; 
    }
    vInt operator--(int) { 
        *this -= 1; 
        return *this; 
    }
    vInt operator--() { 
        vInt tmp = *this; 
        *this -= 1; 
        return tmp; 
    }
    vInt operator<<(uint32_t amt) const { 
        return this->__vIntBase::operator<<<vInt>(amt); 
    }
    vInt operator<<=(uint32_t amt) { 
        this->__vIntBase::operator<<=<vInt>(amt); 
        return v; 
    }
public:
    // Conditionals
    __vCond operator==(int32_t val) const;
    __vCond operator!=(int32_t val) const;
    __vCond operator<(int32_t val) const;
    __vCond operator<=(int32_t val) const;
    __vCond operator>(int32_t val) const;
    __vCond operator>=(int32_t val) const;
    __vCond operator==(__vIntBase src) const;
    __vCond operator!=(__vIntBase src) const;
    __vCond operator<(__vIntBase src) const;
    __vCond operator<=(__vIntBase src) const;
    __vCond operator>(__vIntBase src) const;
    __vCond operator>=(__vIntBase src) const;
};

//
//    vUInt
//

class vUInt: public __vIntBase {
    friend class vInt;
public:
    vUInt() = default;
    vUInt(Vec in) { 
        assign(in); 
    }
    vUInt(__vConstIntBase creg) { 
        v = rvtt->sfpassignlreg(creg.get()); 
        initialized = true; 
    }
    vUInt(__vIntBase in) { 
        assign(in.get()); 
    }
    vUInt(short val) { 
        loadss(val); 
    }
    vUInt(int val) { 
        loadsi(val); 
    }
    vUInt(unsigned short val) { 
        loadus(val); 
    }
    vUInt(unsigned int val) { 
        loadui(val); 
    }
    vUInt(__vLReg lr) { 
        __vBase::operator=(lr); 
    }
    vUInt(__vCond vc);
public:
    // Assignment
    vUInt operator=(vUInt in ) { 
        assign(in.v); 
        return v; 
    }
    vUInt operator=(__vLReg lr) { 
        __vBase::operator=(lr); 
        return v; 
    }
public:
    // Operations
    vUInt operator&(uint32_t b) const { 
        return this->__vIntBase::operator&(vUInt(b)); 
    }
    vUInt operator&(vUInt b) const { 
        return this->__vIntBase::operator&(b); 
    }
    vUInt operator&=(vUInt b) { 
        this->__vIntBase::operator&=(b); 
        return v; 
    }
    vUInt operator|(vUInt b) const { 
        return this->__vIntBase::operator|(b); 
    }
    vUInt operator|(vUInt b) { 
        return this->__vIntBase::operator|(b); 
    }
    vUInt operator|=(vUInt b) { 
        this->__vIntBase::operator|=(b); return v; 
    }
    vUInt operator^(vUInt b) const { 
        return this->__vIntBase::operator^(b); 
    }
    vUInt operator^(vUInt b) { 
        return this->__vIntBase::operator^(b); 
    }
    vUInt operator^=(vUInt b) { 
        this->__vIntBase::operator^=(b); return v; 
    }
    vUInt operator~() const {
        return this->__vIntBase::operator~<vUInt>(); 
    }
    vUInt operator+(int32_t val) const { 
        return this->__vIntBase::add<vUInt>(val, 0); 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vUInt operator+(vType val) const { 
            return this->__vIntBase::operator+<vUInt>(val); 
        }
    vUInt operator+(__vConstIntBase val) const { 
        return this->__vIntBase::operator+<vUInt>(val); 
    }
    vUInt operator-(int32_t val) const { 
        return this->__vIntBase::sub<vUInt>(val, 0); 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vUInt operator-(vType val) const { 
            return this->__vIntBase::operator-<vUInt>(val); 
        }
    vUInt operator-(__vConstIntBase val) const { 
        return this->__vIntBase::operator+<vUInt>(val); 
    }
    vUInt operator+=(int32_t val) { 
        this->__vIntBase::add_eq<vUInt>(val, 0); 
        return v; 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vUInt operator+=(vType val) { 
            this->__vIntBase::operator+=<vUInt>(val); 
            return v; 
        }
    vUInt operator+=(__vConstIntBase val) { 
        this->__vIntBase::operator+=<vUInt>(val); 
        return v; 
    }
    vUInt operator-=(int32_t val) { 
        this->__vIntBase::sub_eq<vUInt>(val, 0); 
        return v; 
    }
    OP_TEMPLATE_DECL(vType, __vIntBase)
        vUInt operator-=(vType val) { 
            this->__vIntBase::operator-=<vUInt>(val); 
            return v; 
        }
    vUInt operator-=(__vConstIntBase val) { 
        this->__vIntBase::operator-=<vUInt>(val); 
        return v; 
    }
    vUInt operator++(int) { 
        *this += 1; 
        return *this; 
    }
    vUInt operator++() { 
        vUInt tmp = *this; 
        *this += 1; 
        return tmp; 
    }
    vUInt operator--(int) { 
        *this -= 1; 
        return *this; 
    }
    vUInt operator--() { 
        vUInt tmp = *this; 
        *this -= 1; 
        return tmp; 
    }
    vUInt operator<<(uint32_t amt) const { 
        return this->__vIntBase::operator<<<vUInt>(amt); 
    }
    vUInt operator>>(uint32_t amt) const;
    vUInt operator<<=(uint32_t amt) { 
        this->__vIntBase::operator<<=<vUInt>(amt); return v; 
    }
    vUInt operator>>=(uint32_t amt);
public:
    // Conditionals
    __vCond operator==(int32_t val) const;
    __vCond operator!=(int32_t val) const;
    __vCond operator<(int32_t val) const;
    __vCond operator<=(int32_t val) const;
    __vCond operator>(int32_t val) const;
    __vCond operator>=(int32_t val) const;
    __vCond operator==(__vIntBase src) const;
    __vCond operator!=(__vIntBase src) const;
    __vCond operator<(__vIntBase src) const;
    __vCond operator<=(__vIntBase src) const;
    __vCond operator>(__vIntBase src) const;
    __vCond operator>=(__vIntBase src) const;
};

//
//    __vCond
//

class __vCond {
    friend class __vCCCtrl;
    friend class vInt;
    friend class vUInt;
 public:
    enum __vCondOpType {
        __vCondLT = SFPXCMP_MOD1_CC_LT,
        __vCondNE = SFPXCMP_MOD1_CC_NE,
        __vCondGTE = SFPXCMP_MOD1_CC_GTE,
        __vCondEQ = SFPXCMP_MOD1_CC_EQ,
        __vCondLTE = SFPXCMP_MOD1_CC_LTE,
        __vCondGT = SFPXCMP_MOD1_CC_GT,
    };
private:
    enum class vBoolOpType {
        vBoolOr = SFPXBOOL_MOD1_OR,
        vBoolAnd = SFPXBOOL_MOD1_AND,
        vBoolNot = SFPXBOOL_MOD1_NOT,
    };
public:
    // Bool
    __vCond(vBoolOpType t, const __vCond &a, const __vCond &b) { 
        result = rvtt->sfpxbool((int)t, a.get(), b.get()); 
    }
public:
    // Float
    __vCond(__vCondOpType t, vFloat a, float b) { 
        result = rvtt->sfpxfcmps(a.get(), __f32asui(b), t | SFPXSCMP_MOD1_FMT_FLOAT); 
    }
    __vCond(__vCondOpType t, vFloat a, s2vFloat16 b) { 
        uint32_t mod1_fmt = 
            (b.get_format() == SFPLOADI_MOD0_FLOATA) ? 
                SFPXSCMP_MOD1_FMT_A : SFPXSCMP_MOD1_FMT_B;
        result = rvtt->sfpxfcmps(a.get(), b.get(), t | mod1_fmt);
    }
    __vCond(__vCondOpType t, vFloat a, vFloat b) { 
        result = rvtt->sfpxfcmpv(a.get(), b.get(), t); 
    }
public:
    // Int
    __vCond(__vCondOpType t, __vIntBase a, int32_t b, uint32_t mod) { 
        result = rvtt->sfpxicmps(a.get(), b, mod | t); 
    }
    __vCond(__vCondOpType t, __vIntBase a, __vIntBase b, uint32_t mod) { 
        result = rvtt->sfpxicmpv(a.get(), b.get(), mod | t); 
    }
public:
    // Create from an integer context
    __vCond(vInt a) { 
        result = rvtt->sfpxicmps(a.get(), 0, __vCondNE); 
    }
public:
    // Create boolean operations from conditional operations
    __vCond operator&&(const __vCond &b) const { 
        return __vCond(vBoolOpType::vBoolAnd, *this, b); 
    }
    __vCond operator||(const __vCond &b) const { 
        return __vCond(vBoolOpType::vBoolOr, *this, b); 
    }
    __vCond operator!() const { 
        return __vCond(vBoolOpType::vBoolNot, *this, *this); 
    }
private:
    int get() const { return result; }
private:
    int result;
};

//
//    __vCCCtrl
//

class __vCCCtrl {
public:
    __vCCCtrl();
    ~__vCCCtrl();
public:
    void cc_if(const __vCond &op) const;
    void cc_if(const __vIntBase &b) const;
    void cc_else() const;
    void cc_elseif(const __vCond &cond) const;
    void cc_elseif(const __vIntBase &b) const;
    void mark_top();
    void push();
    void pop();
    static void enable_cc();
protected:
    int top;
    int push_count;
};

//
//    Global constants
//

constexpr __vConstFloat vConst0(CREG_IDX_0);
constexpr __vConstFloat vConst1(CREG_IDX_1);
constexpr __vConstFloat vConstNeg1(CREG_IDX_NEG_1);

//
//    Internal functions
//

namespace sfpi_int {

inline vFloat fp_add(vFloat a, vFloat b) {
    return rvtt->sfpadd(a.get(), b.get(), 0);
}

inline vFloat fp_mul(vFloat a, vFloat b) {
    return rvtt->sfpmul(a.get(), b.get(), 0);
}

inline vFloat fp_sub(vFloat a, vFloat b) {
    Vec neg1 = rvtt->sfpassignlreg(vConstNeg1.get());
    return rvtt->sfpmad(neg1, b.get(), a.get(), 0);
}

} // namespace sfpi_int

//
//    __RegFile
//

template<class TYPE, int N>
TYPE __RegFile<TYPE, N>::operator[](int x) const {
    return TYPE(__vRegBaseInitializer(x));
}

//
//    __vDReg
//

inline vInt __vDReg::operator=(int i) const {
    vInt v(i);
    *this = v;
    return v;
}

inline vUInt __vDReg::operator=(unsigned int i) const {
    vUInt v(i);
    *this = v;
    return v;
}

inline vFloat __vDReg::operator+(vFloat b) const {
    return sfpi_int::fp_add(vFloat(*this), b); 
}

inline vFloat __vDReg::operator-(vFloat b) const { 
    return sfpi_int::fp_sub(vFloat(*this), b); 
}

inline vFloat __vDReg::operator*(vFloat b) const { 
    return sfpi_int::fp_mul(vFloat(*this), b); 
}

inline __vCond __vDReg::operator==(s2vFloat16 x) const {
    return __vCond(__vCond::__vCondEQ, vFloat(*this), x); 
}

inline __vCond __vDReg::operator!=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondNE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondLT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondLTE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondGT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondGTE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator==(vFloat x) const {
    return __vCond(__vCond::__vCondEQ, vFloat(*this), x); 
}

inline __vCond __vDReg::operator!=(vFloat x) const { 
    return __vCond(__vCond::__vCondNE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<(vFloat x) const { 
    return __vCond(__vCond::__vCondLT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator<=(vFloat x) const { 
    return __vCond(__vCond::__vCondLTE, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>(vFloat x) const { 
    return __vCond(__vCond::__vCondGT, vFloat(*this), x); 
}

inline __vCond __vDReg::operator>=(vFloat x) const { 
    return __vCond(__vCond::__vCondGTE, vFloat(*this), x); 
}

inline vFloat __vDReg::operator-() const {
    vFloat tmp = *this;
    return rvtt->sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//
//    __vLReg
//

inline Vec __vLReg::operator=(__vBase &v) const {
    rvtt->sfppreservelreg(v.get(), reg);
    return v.get();
}

//
//    __vConstFloat
//

inline vFloat __vConstFloat::operator+(vFloat b) const { 
    return vFloat(*this) + b; 
}

inline vFloat __vConstFloat::operator-(vFloat b) const { 
    return vFloat(*this) - b; 
}

inline vFloat __vConstFloat::operator*(vFloat b) const { 
    return vFloat(*this) * b; 
}

inline __vCond __vConstFloat::operator==(vFloat x) const { 
    return __vCond(__vCond::__vCondEQ, *this, x); 
}

inline __vCond __vConstFloat::operator!=(vFloat x) const { 
    return __vCond(__vCond::__vCondNE, *this, x); 
}

inline __vCond __vConstFloat::operator<(vFloat x) const { 
    return __vCond(__vCond::__vCondLT, *this, x); 
}

inline __vCond __vConstFloat::operator<=(vFloat x) const { 
    return __vCond(__vCond::__vCondLTE, vFloat(*this), x); 
}

inline __vCond __vConstFloat::operator>(vFloat x) const { 
    return __vCond(__vCond::__vCondGT, vFloat(*this), x); 
}

inline __vCond __vConstFloat::operator>=(vFloat x) const { 
    return __vCond(__vCond::__vCondGTE, *this, x); 
}

//
//    __vConstIntBase
//

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vConstIntBase::operator+(vType b) const { 
    return vType(*this) + b; 
}

inline vInt __vConstIntBase::operator+(int32_t b) const { 
    return vInt(*this) + b; 
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vConstIntBase::operator-(vType b) const { 
    return vType(*this) - b; 
}

inline vInt __vConstIntBase::operator-(int32_t b) const { 
    return vInt(*this) - b; 
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vConstIntBase::operator&(vType b) const { 
    return vType(*this) & b; 
}

inline vInt __vConstIntBase::operator&(int32_t b) const { 
    return vInt(*this) & vInt(b); 
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vConstIntBase::operator|(vType b) const { 
    return vType(*this) | b; 
}

inline vInt __vConstIntBase::operator|(int32_t b) const { 
    return vInt(*this) | vInt(b); 
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vConstIntBase::operator^(vType b) const { 
    return vType(*this) ^ b; 
}

inline vInt __vConstIntBase::operator^(int32_t b) const { 
    return vInt(*this) ^ vInt(b); 
}

inline __vCond __vConstIntBase::operator==(vInt x) const { 
    return __vCond(__vCond::__vCondEQ, __vIntBase(*this), x, 0); 
}

inline __vCond __vConstIntBase::operator!=(vInt x) const { 
    return __vCond(__vCond::__vCondNE, __vIntBase(*this), x, 0); 
}

inline __vCond __vConstIntBase::operator<(vInt x) const { 
    return __vCond(__vCond::__vCondLT, __vIntBase(*this), x, 0); 
}

inline __vCond __vConstIntBase::operator<=(vInt x) const { 
    return __vCond(__vCond::__vCondLTE, __vIntBase(*this), x, 0); 
}

inline __vCond __vConstIntBase::operator>(vInt x) const { 
    return __vCond(__vCond::__vCondGT, __vIntBase(*this), x, 0); 
}

inline __vCond __vConstIntBase::operator>=(vInt x) const { 
    return __vCond(__vCond::__vCondGTE, __vIntBase(*this), x, 0); 
}

inline __vIntBase __vConstIntBase::operator<<(uint32_t amt) const {
    Vec v = rvtt->sfpassignlreg(reg);
    return rvtt->sfpshft_i(v, amt);
}

inline vUInt __vConstIntBase::operator>>(uint32_t amt) const {
    Vec v = rvtt->sfpassignlreg(reg);
    return rvtt->sfpshft_i(v, uint32_t(-int32_t(amt)));
}

//
//   __vBase 
//

inline void __vBase::assign(Vec in) {
    v = initialized ? rvtt->sfpassign_lv(v, in) : in;
    initialized = true;
}

inline void __vBase::operator=(__vLReg lr) {
    v = rvtt->sfpassignlreg(lr.get());
    initialized = true;
}

//
//    vFloat
//

inline vFloat::vFloat(__vConstFloat creg) {
    v = rvtt->sfpassignlreg(creg.get());
    initialized = true;
}

inline vFloat vFloat::operator+(vFloat b) const { 
    return sfpi_int::fp_add(*this, b); 
}

inline vFloat vFloat::operator-(vFloat b) const { 
    return sfpi_int::fp_sub(*this, b); 
}

inline vFloat vFloat::operator-(float b) const { 
    return sfpi_int::fp_add(*this, vFloat(-b)); 
}

inline vFloat vFloat::operator-(s2vFloat16 b) const { 
    return sfpi_int::fp_add(*this, b.negate()); 
}

inline vFloat vFloat::operator*(vFloat b) const { 
    return sfpi_int::fp_mul(*this, b); 
}

inline __vCond vFloat::operator==(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondEQ, *this, x); 
}

inline __vCond vFloat::operator!=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondNE, *this, x); 
}

inline __vCond vFloat::operator<(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondLT, *this, x); 
}

inline __vCond vFloat::operator<=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondLTE, *this, x); 
}

inline __vCond vFloat::operator>(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondGT, *this, x); 
}

inline __vCond vFloat::operator>=(s2vFloat16 x) const { 
    return __vCond(__vCond::__vCondGTE, *this, x);
}

inline __vCond vFloat::operator==(vFloat x) const { 
    return __vCond(__vCond::__vCondEQ, *this, x); 
}

inline __vCond vFloat::operator!=(vFloat x) const { 
    return __vCond(__vCond::__vCondNE, *this, x); 
}

inline __vCond vFloat::operator<(vFloat x) const { 
    return __vCond(__vCond::__vCondLT, *this, x); 
}

inline __vCond vFloat::operator<=(vFloat x) const { 
    return __vCond(__vCond::__vCondLTE, *this, x); 
}

inline __vCond vFloat::operator>(vFloat x) const { 
    return __vCond(__vCond::__vCondGT, *this, x); 
}

inline __vCond vFloat::operator>=(vFloat x) const { 
    return __vCond(__vCond::__vCondGTE, *this, x); 
}

inline vFloat vFloat::operator*=(vFloat m) {
    assign(rvtt->sfpmul(v, m.get(), SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

inline vFloat vFloat::operator+=(vFloat a) {
    assign(rvtt->sfpadd(v, a.get(), SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

inline vFloat vFloat::operator-() const {
    return rvtt->sfpmov(v, SFPMOV_MOD1_COMPSIGN);
    return v;
}

inline void vFloat::loadf16(s2vFloat16 val) {
    assign(rvtt->sfpxloadi(val.get_format(), val.get()));
}

//
//   __vIntBase 
//

inline __vIntBase::__vIntBase(__vConstIntBase creg) {
    v = rvtt->sfpassignlreg(creg.get());
    initialized = true;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator&(vType b) const {
    return rvtt->sfpand(v, b.get());
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
void __vIntBase::operator&=(vType b) {
    v = rvtt->sfpand(v, b.get());
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator|(vType b) const {
    return rvtt->sfpor(v, b.get());
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator|=(vType b) {
    v = rvtt->sfpor(v, b.get());
    return v;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator~() const {
    return rvtt->sfpnot(v);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator<<(uint32_t amt) const {
    return rvtt->sfpshft_i(v, amt);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator<<=(uint32_t amt) {
    assign((static_cast<vType>(*this) << amt).get());
    return v;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::add(int32_t val, unsigned int mod_base) const {
    return rvtt->sfpxiadd_i(v, val, mod_base);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator+(__vIntBase val) const {
    return rvtt->sfpxiadd_v(val.get(), v, 0);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator+(__vConstIntBase val) const {
    Vec c = rvtt->sfpassignlreg(val.get());
    return rvtt->sfpxiadd_v(c, v, 0);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::sub(int32_t val, unsigned int mod_base) const {
    return rvtt->sfpxiadd_i(v, val, mod_base | SFPXIADD_MOD1_IS_SUB);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator-(__vIntBase val) const {
    return rvtt->sfpxiadd_v(val.get(), v, SFPXIADD_MOD1_IS_SUB);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator-(__vConstIntBase val) const {
    Vec c = rvtt->sfpassignlreg(val.get());
    return rvtt->sfpxiadd_v(c, v, SFPXIADD_MOD1_IS_SUB);
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
void __vIntBase::add_eq(int32_t val, unsigned int mod_base) {
    assign(rvtt->sfpxiadd_i(v, val, mod_base));
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator+=(__vIntBase val) {
    assign(rvtt->sfpxiadd_v(v, val.get(), 0));
    return v;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator+=(__vConstIntBase val) {
    Vec c = rvtt->sfpassignlreg(val.get());
    assign(rvtt->sfpxiadd_v(c, v, 0));
    return v;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
void __vIntBase::sub_eq(int32_t val, unsigned int mod_base) {
    assign(rvtt->sfpxiadd_i(v, val, mod_base | SFPXIADD_MOD1_IS_SUB));
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator-=(__vIntBase val) {
    assign(rvtt->sfpxiadd_v(val.get(), v, SFPXIADD_MOD1_IS_SUB));
    return v;
}

OP_TEMPLATE_IMPL(vType, __vIntBase)
vType __vIntBase::operator-=(__vConstIntBase val) {
    Vec c = rvtt->sfpassignlreg(val.get());
    assign(rvtt->sfpxiadd_v(c, v, SFPXIADD_MOD1_IS_SUB));
    return v;
}

inline void __vIntBase::loadss(int16_t val) {
    assign(rvtt->sfpxloadi(SFPLOADI_MOD0_SHORT, val));
}

inline void __vIntBase::loadus(uint16_t val) {
    assign(rvtt->sfpxloadi(SFPLOADI_MOD0_USHORT, val));
}

//
//    vUInt
//

inline vInt::vInt(__vCond vc) {
    v = rvtt->sfpxcondi(vc.get());
    initialized = true;
}

inline __vCond vInt::operator==(int32_t val) const { 
    return __vCond(__vCond::__vCondEQ, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator!=(int32_t val) const { 
    return __vCond(__vCond::__vCondNE, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator<(int32_t val) const { 
    return __vCond(__vCond::__vCondLT, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator<=(int32_t val) const { 
    return  __vCond(__vCond::__vCondLTE, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator>(int32_t val) const { 
    return  __vCond(__vCond::__vCondGT, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator>=(int32_t val) const { 
    return __vCond(__vCond::__vCondGTE, *this, val, SFPXIADD_MOD1_SIGNED); 
}

inline __vCond vInt::operator==(__vIntBase src) const { 
    return __vCond(__vCond::__vCondEQ, src, *this, 0); 
}

inline __vCond vInt::operator!=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondNE, src, *this, 0); 
}

inline __vCond vInt::operator<(__vIntBase src) const { 
    return __vCond(__vCond::__vCondLT, src, *this, 0); 
}

inline __vCond vInt::operator<=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondLTE, src, *this, 0); 
}

inline __vCond vInt::operator>(__vIntBase src) const { 
    return __vCond(__vCond::__vCondGT, src, *this, 0); 
}

inline __vCond vInt::operator>=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondGTE, src, *this, 0); 
}

inline vUInt vUInt::operator>>(uint32_t amt) const {
    return rvtt->sfpshft_i(v, uint32_t(-int32_t(amt)));
}

inline vUInt vUInt::operator>>=(uint32_t amt) {
    assign((*this >> amt).get());
    return v;
}

//
//    vUInt
//

inline vUInt::vUInt(__vCond vc) {
    v = rvtt->sfpxcondi(vc.get());
    initialized = true;
}

inline __vCond vUInt::operator==(int32_t val) const { 
    return __vCond(__vCond::__vCondEQ, *this, val, 0); 
}

inline __vCond vUInt::operator!=(int32_t val) const { 
    return __vCond(__vCond::__vCondNE, *this, val, 0); 
}

inline __vCond vUInt::operator<(int32_t val) const { 
    return __vCond(__vCond::__vCondLT, *this, val, 0); 
}

inline __vCond vUInt::operator<=(int32_t val) const { 
    return  __vCond(__vCond::__vCondLTE, *this, val, 0); 
}

inline __vCond vUInt::operator>(int32_t val) const { 
    return  __vCond(__vCond::__vCondGT, *this, val, 0); 
}

inline __vCond vUInt::operator>=(int32_t val) const { 
    return __vCond(__vCond::__vCondGTE, *this, val, 0); 
}

inline __vCond vUInt::operator==(__vIntBase src) const { 
    return __vCond(__vCond::__vCondEQ, src, *this, 0); 
}

inline __vCond vUInt::operator!=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondNE, src, *this, 0); 
}

inline __vCond vUInt::operator<(__vIntBase src) const { 
    return __vCond(__vCond::__vCondLT, src, *this, 0); 
}

inline __vCond vUInt::operator<=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondLTE, src, *this, 0); 
}

inline __vCond vUInt::operator>(__vIntBase src) const { 
    return __vCond(__vCond::__vCondGT, src, *this, 0); 
}

inline __vCond vUInt::operator>=(__vIntBase src) const { 
    return __vCond(__vCond::__vCondGTE, src, *this, 0); 
}

//
//    Global operators for vFloat, vInt, and vUint
//

inline vFloat operator+(float a, vFloat b) { 
    return b + a; 
}

inline vFloat operator*(float a, vFloat b) { 
    return b * a; 
}

inline vFloat operator-(float a, vFloat b) { 
    return vFloat(a) - b; 
}

inline __vCond operator==(float a, vFloat b) { 
    return (b == a); 
}

inline __vCond operator!=(float a, vFloat b) { 
    return (b != a); 
}

inline __vCond operator<(float a, vFloat b) { 
    return (b > a); 
}

inline __vCond operator<=(float a, vFloat b) { 
    return (b >= a); 
}

inline __vCond operator>(float a, vFloat b) { 
    return (b < a); 
}

inline __vCond operator>=(float a, vFloat b) { 
    return (b <= a); 
}

inline __vCond operator==(s2vFloat16 a, vFloat b) { 
    return (b == a); 
}

inline __vCond operator!=(s2vFloat16 a, vFloat b) { 
    return (b != a); 
}

inline __vCond operator<(s2vFloat16 a, vFloat b) { 
    return (b > a); 
}

inline __vCond operator<=(s2vFloat16 a, vFloat b) { 
    return (b >= a); 
}

inline __vCond operator>(s2vFloat16 a, vFloat b) { 
    return (b < a); 
}

inline __vCond operator>=(s2vFloat16 a, vFloat b) { 
    return (b <= a); 
}

inline vInt operator+(int32_t a, vInt b) { 
    return b + a; 
}

inline vInt operator-(int32_t a, vInt b) { 
    return vInt(a) - b; 
}

inline vInt operator&(int32_t a, vInt b) { 
    return b & a; 
}

inline vInt operator|(int32_t a, vInt b) { 
    return b | a; 
}

inline vInt operator^(int32_t a, vInt b) { 
    return b ^ a; 
}

inline __vCond operator==(int32_t a, vInt b) { 
    return (b == a); 
}

inline __vCond operator!=(int32_t a, vInt b) { 
    return (b != a); 
}

inline __vCond operator<(int32_t a, vInt b) { 
    return (b > a); 
}

inline __vCond operator<=(int32_t a, vInt b) { 
    return (b >= a); 
}

inline __vCond operator>(int32_t a, vInt b) { 
    return (b < a); 
}

inline __vCond operator>=(int32_t a, vInt b) { 
    return (b <= a); 
}

inline vUInt operator+(int32_t a, vUInt b) { 
    return b + a; 
}

inline vUInt operator-(int32_t a, vUInt b) { 
    return vUInt(a) - b; 
}

inline vUInt operator&(int32_t a, vUInt b) { 
    return b & a; 
}

inline vUInt operator|(int32_t a, vUInt b) { 
    return b | a; 
}

inline vUInt operator^(int32_t a, vUInt b) { 
    return b ^ a; 
}

inline __vCond operator==(int32_t a, vUInt b) { 
    return (b == a); 
}

inline __vCond operator!=(int32_t a, vUInt b) { 
    return (b != a); 
}

inline __vCond operator<(int32_t a, vUInt b) { 
    return (b > a); 
}

inline __vCond operator<=(int32_t a, vUInt b) { 
    return (b >= a); 
}

inline __vCond operator>(int32_t a, vUInt b) { 
    return (b < a); 
}

inline __vCond operator>=(int32_t a, vUInt b) { 
    return (b <= a); 
}

//
//    __vCCCtrl
//

inline __vCCCtrl::__vCCCtrl(): 
        push_count(0) {
    push();
}

inline __vCCCtrl::~__vCCCtrl() {
    while (push_count != 0) {
        pop();
    }
}

inline void __vCCCtrl::cc_if(const __vCond &op) const {
    rvtt->sfpxcondb(op.get(), top);
}

inline void __vCCCtrl::cc_if(const __vIntBase &v) const {
    rvtt->sfpxcondb(__vCond(__vCond::__vCondNE, v, 0, 0).get(), top);
}

inline void __vCCCtrl::cc_elseif(const __vCond &op) const {
    cc_if(op);
}

inline void __vCCCtrl::cc_elseif(const __vIntBase &v) const {
    cc_if(v);
}

inline void __vCCCtrl::cc_else() const {
    rvtt->sfpcompc();
}

inline void __vCCCtrl::mark_top() {
    top = rvtt->sfpxvif();
}

inline void __vCCCtrl::push() {
    push_count++;
    rvtt->sfppushc();
}

inline void __vCCCtrl::pop() {
    push_count--;
    rvtt->sfppopc();
}

inline void __vCCCtrl::enable_cc() {
    rvtt->sfpencc(SFPENCC_IMM12_BOTH, SFPENCC_MOD1_EI_RI);
}

//
//    Global register files
//

constexpr __DestReg dst_reg;
constexpr __LReg l_reg;

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

//
//    Vector conditional statements
//

#define v_if(x)             \
{                           \
   __vCCCtrl __cc;          \
   __cc.mark_top();         \
   __cc.cc_if(x);

#define v_elseif(x)         \
    __cc.cc_else();         \
    __cc.push();            \
    __cc.mark_top();        \
    __cc.cc_elseif(x);

#define v_else              \
    __cc.cc_else();

#define v_endif             \
}

#define v_block             \
{                           \
    __vCCCtrl __cc;

#define v_and(x)            \
    __cc.mark_top();        \
    __cc.cc_if(x)

#define v_endblock          \
}

#include "sfpi/sfpi_impl.hpp"
#include "sfpi/sfpi_lib.hpp"

#define sfpi_inline inline

#undef OP_TEMPLATE_DECL
#undef OP_TEMPLATE_IMPL

