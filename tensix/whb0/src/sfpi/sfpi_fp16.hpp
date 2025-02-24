// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "sfpi/sfpi_hw.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    s2vFloat16
//

class s2vFloat16 {
public:
    enum Format { 
        fp16a = SFPLOADI_MOD0_FLOATA, 
        fp16b = SFPLOADI_MOD0_FLOATB 
    };
public:
    s2vFloat16(float in, Format f = fp16b);
    s2vFloat16(int32_t in, Format f = fp16b);
    s2vFloat16(uint32_t in, Format f = fp16b);
public:
    s2vFloat16 negate() const { 
        return s2vFloat16(value ^ 0x8000, format); 
    }
    uint32_t get() const { 
        return value; 
    }
    uint32_t get_format() const { 
        return format; 
    }
private:
    static uint32_t fp32_to_fp16a(float val);
    static uint32_t fp32_to_fp16b(float val);
private:
    uint32_t value;
    Format format;
};

// implementation

inline s2vFloat16::s2vFloat16(float in, Format f) {
    value = (f == fp16a) ? fp32_to_fp16a(in) : fp32_to_fp16b(in);
    format = f;
}

inline s2vFloat16::s2vFloat16(int32_t in, Format f) {
    value = in;
    format = f;
}

inline s2vFloat16::s2vFloat16(uint32_t in, Format f) {
    value = in;
    format = f;
}

inline uint32_t s2vFloat16::fp32_to_fp16a(float val) {
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vfloat = val;

    // https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
    // Handles denorms.  May be costly w/ non-immediate values
    uint32_t b = tmp.vui + 0x00001000;
    uint32_t e = (b & 0x7F800000) >> 23;
    uint32_t m = b & 0x007FFFFF;
    uint32_t result =
        (b & 0x80000000) >> 16 |
        (e > 112u) * ((((e - 112u) << 10) & 0x7C00) | m >> 13) |
        ((e < 113u) & (e > 101u)) * ((((0x007FF000 + m) >> (125u - e)) + 1) >> 1) |
        (e > 143u) * 0x7FFF;
#if 0
    // Simple/faster but less complete
    uint32_t result =
        ((tmp.vui >> 16) & 0x8000) |
        ((((tmp.vui & 0x7F800000) - 0x38000000) >> 13) & 0x7c00) |
        ((tmp.vui >> 13) & 0x03FF);
#endif

    return result;
}

inline uint32_t s2vFloat16::fp32_to_fp16b(float val) {
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vfloat = val;

    return tmp.vui >> 16;
}

//
//    s2vFloat16a
//

class s2vFloat16a: public s2vFloat16 {
public:
    s2vFloat16a(float in): 
        s2vFloat16(in, fp16a) { }
    s2vFloat16a(double in): 
        s2vFloat16(float(in), fp16a) { }
    s2vFloat16a(int32_t in): 
        s2vFloat16(in, fp16a) { }
    s2vFloat16a(uint32_t in): 
        s2vFloat16(in, fp16a) { }
};

//
//    s2vFloat16b
//

class s2vFloat16b: public s2vFloat16 {
public:
    s2vFloat16b(float in): 
        s2vFloat16(in, fp16b) { }
    s2vFloat16b(double in): 
        s2vFloat16(float(in), fp16b) { }
    s2vFloat16b(int32_t in): 
        s2vFloat16(in, fp16b) { }
    s2vFloat16b(uint32_t in): 
        s2vFloat16(in, fp16b) { }
};

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

