#pragma once

#include <cstdint>
#include <limits>

#include "sfpi/sfpi.hpp"

#include "ckernel_defs.h"
#include "ckernel_sfpi.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace sfpu {

using namespace sfpi;

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat calculate_sqrt_custom(vFloat in) {
    vFloat val = in;
    vFloat out;
    vConstFloatPrgm0 = s2vFloat16b(0x5f37);
    v_if (val != 0.0f){
        vUInt magic = vConstIntPrgm0;
        vFloat approx = reinterpret<vFloat>(magic - (reinterpret<vUInt>(val) >> 1));
        for (int r = 0; r < 2; r++) {
            approx = ((approx * approx) * (val * -0.5f) + 1.5f) * approx;
        }
        out = approx * val;
    } v_else{
        out = val;
    }
    v_endif;
    return out;
}

template <bool APPROXIMATION_MODE>
sfpi_inline vFloat calculate_erfinv_body(vFloat in) {
    vFloat log_value = in * in;
    log_value = 1.0f - log_value;
    dst_reg[0] = log_value;
    calculate_log_body<false>(0);
    log_value = dst_reg[0];
    vFloat temp = dst_reg[0] * 0.5f;
    temp = 4.5469f + temp;
    temp = -temp;
    vFloat calculated_value = (temp * temp) - (log_value * 7.1427f);
    vFloat intermediate_result = calculate_sqrt_custom<false>(calculated_value);
    calculated_value = temp + intermediate_result;
    log_value = calculate_sqrt_custom<false>(calculated_value);
    dst_reg[0] = log_value;
    return log_value;
}

template <bool APPROXIMATION_MODE>
inline void calculate_erfinv() {
    begin_sfpi();

    // SFPU microcode
    for (int d = 0; d < WHB0_ITERATIONS; d++) {
        vFloat v = dst_reg[0];
        v_if (v == 1.0f) {
            dst_reg[0] = std::numeric_limits<float>::infinity();
        } v_elseif (v == -1.0f) {
            dst_reg[0] = -std::numeric_limits<float>::infinity();
        } v_elseif (v < -1.0f || v > 1.0f) {
            dst_reg[0] = std::numeric_limits<float>::quiet_NaN();
        } v_elseif (v < 0.0f) {
            calculate_erfinv_body<true>(v);
            dst_reg[0] = -dst_reg[0];
        } v_else {
            calculate_erfinv_body<true>(v);
        }
        v_endif;
        dst_reg++;
    }

    end_sfpi();
}

template <bool APPROXIMATION_MODE>
void erfinv_init() {
    // nothing to do
}

} // namespace sfpu
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

