// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/math.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_sfpu;
using core::p_exp;

constexpr static uint32_t
    LREG0 = p_sfpu::LREG0,
    LREG1 = p_sfpu::LREG1,
    LREG2 = p_sfpu::LREG2,
    LREG3 = p_sfpu::LREG3,
    LREG4 = p_sfpu::LREG4,
    LREG5 = p_sfpu::LREG5,
    LREG6 = p_sfpu::LREG6,
    LREG7 = p_sfpu::LREG7;

constexpr static uint32_t
    LCONST_0_8373 = p_sfpu::LCONST_0_8373,
    LCONST_0 = p_sfpu::LCONST_0,
    LCONST_1 = p_sfpu::LCONST_1;

constexpr static uint32_t
    LREG11 = p_sfpu::LREG11,
    LREG12 = p_sfpu::LREG12,
    LREG13 = p_sfpu::LREG13,
    LREG14 = p_sfpu::LREG14;

// configured during init
constexpr static uint32_t 
    LCONST_neg1 = LREG11,
    LCONST_ln2 = LREG12,
    LCONST_ln2_recip = LREG13,
    LCONST_neg_point_5 = LREG14; 

//
//    Math
//

// initialization

void Math::sfpu_init(bool APPROXIMATION_MODE, SfpuType operation, uint32_t param0) {
    configure_programmable_constants();
    uint32_t imm0;
    uint32_t imm1;
    uint32_t imm2;
    switch (operation) {
    case SfpuType::tanh:
    case SfpuType::tanh_derivative:
        imm0 = 0x1DFF; // 0.90625 * x
        imm1 = 0x481A; // 0.09375 * x + 0.8125
        imm2 = 0xFF00; // 1
        SFPLOADI(0, 2, imm0);
        SFPLOADI(1, 2, imm1);
        SFPLOADI(2, 2, imm2);
        break;
    case SfpuType::sigmoid:
        imm0 = 0x3DFF;
        imm1 = 0x21D8;
        imm2 = 0xFF10;
        SFPLOADI(0, 2, imm0);
        SFPLOADI(1, 2, imm1);
        SFPLOADI(2, 2, imm2);
        break;
    case SfpuType::gelu:
        imm0 = 0x18FF;
        imm1 = APPROXIMATION_MODE ? 0x212C : 0x2010;
        imm2 = 0xFF00;
        SFPLOADI(0, 2, imm0);
        SFPLOADI(1, 2, imm1);
        SFPLOADI(2, 2, imm2);
        break;
    case SfpuType::sqrt:
        imm2 = APPROXIMATION_MODE ? (127 << 7) : 0x5f37;
        SFPLOADI(2, 0, imm2);
        break;
    case SfpuType::exponential:
        if (APPROXIMATION_MODE) {
            SFPLOADI(LREG0, 0, p_exp::C23_73);
            SFPLOADI(LREG2, 0, p_exp::ADJ_EXP);
        }
        break;
    case SfpuType::dropout:
        init_dropout_seed(param0);
        // store binary value of 0b1 - used to extract LSB
        sfpu_load_imm32(LREG7, 0x0, 0x1);
        break;
    default:
        // Should result in compile time error??
        break;
    }
}

void Math::configure_programmable_constants() {
    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0xBF80);
    SFPCONFIG(0, LCONST_neg1, 0);

    SFPLOADI(0, 0xA, 0x6000);
    SFPLOADI(0, 0x8, 0x3f31);
    SFPCONFIG(0, LCONST_ln2, 0);

    SFPLOADI(0, 0xA, 0xAA3B);
    SFPLOADI(0, 0x8, 0x3FB8);
    SFPCONFIG(0, LCONST_ln2_recip, 0);

    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0xBF00);
    SFPCONFIG(0, LCONST_neg_point_5, 0);

    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0x0000);
}

void Math::init_dropout_seed(uint16_t p2) {
    // TODO: Calcuate random input seed
    uint16_t per_tensix_input_seed = 0xa94b;

    SFPMOV(0x0, 15, LREG2, 0);                      // lreg2 <= lreg15 (tile-id)
    SFPSHFT(10, 0, LREG2, 1);                       // lreg2 <<= 10
    SFPMOV(0x0, LREG2, LREG3, 0);                   // lreg3 <= lreg2 (tile-id)
    SFPLOADI(LREG0, 0xA, per_tensix_input_seed);    // insmod == A will write the lower bits
                                                    //     and not affect the upper bits;
    SFPLOADI(LREG0, 0x8, 0);                        // zero out upper 16-bits

    // XOR per-tensix random seed with tile id
    // XOR breakdown lreg3 = lreg2 ^ lreg0
    SFPAND(0, LREG0, LREG2, 0);    // lreg2 = lreg2 AND lreg0  = tile-id AND p2
    SFPNOT(0, LREG2, LREG2, 0);    // lreg2 = ~lreg2
    SFPOR(0, LREG0, LREG3, 0);     // lreg3 = lreg3 OR lreg0  = tile-id OR p2
    SFPAND(0, LREG2, LREG3, 0);    // lreg3 = lreg2 AND lreg3
                                   //     = ~(tile-id AND p2) AND (tile-id OR p2)
}

// calculation

void Math::calculate_sfpu(
        SfpuType operation, 
        bool APPROXIMATION_MODE, 
        int SfpuType_PARAM, 
        int ITERATIONS, 
        uint32_t ADDR_MOD,
        uint32_t param0, 
        uint32_t param1, 
        uint32_t param2, 
        uint32_t param3, 
        uint32_t param4, 
        uint32_t param5) {
    if (operation == SfpuType::exponential) {
        calculate_exponential(APPROXIMATION_MODE, APPROXIMATION_MODE, false, ADDR_MOD, param0);
    } else if (operation == SfpuType::exp_with_base) {
        calculate_exponential(APPROXIMATION_MODE, false, true, ADDR_MOD, param0);
    } else if (operation == SfpuType::tanh) {
        calculate_tanh(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::hardtanh) {
        calculate_hardtanh(APPROXIMATION_MODE, ADDR_MOD, param0, param1, param2);
    } else if (operation == SfpuType::gelu) {
        calculate_gelu(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::reciprocal) {
        calculate_reciprocal(APPROXIMATION_MODE, ITERATIONS, ADDR_MOD);
    } else if (operation == SfpuType::sigmoid) {
        calculate_sigmoid(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::sqrt) {
        calculate_sqrt(APPROXIMATION_MODE, ITERATIONS, 2, ADDR_MOD);
    } else if (operation == SfpuType::tanh_derivative) {
        calculate_tanh_derivative(APPROXIMATION_MODE, SfpuType_PARAM, ADDR_MOD);
    } else if (operation == SfpuType::lrelu) {
        calculate_lrelu(APPROXIMATION_MODE, ADDR_MOD, param0);
    } else if (operation == SfpuType::dropout) {
        calculate_dropout(APPROXIMATION_MODE, ADDR_MOD, param0, param1);
    } else if (operation == SfpuType::power) {
        calculate_power(APPROXIMATION_MODE, ADDR_MOD, param0);
    } else if (operation == SfpuType::multiply) {
        calculate_multiply(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::log) {
        calculate_log(APPROXIMATION_MODE, false, ADDR_MOD, param0);
    } else if (operation == SfpuType::log_with_base) {
        calculate_log(APPROXIMATION_MODE, true, ADDR_MOD, param0);
    } else if (operation == SfpuType::gelu_derivative) {
        calculate_gelu_derivative(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::equal_zero ||
            operation == SfpuType::not_equal_zero ||
            operation == SfpuType::less_than_zero ||
            operation == SfpuType::greater_than_equal_zero ||
            operation == SfpuType::less_than_equal_zero ||
            operation == SfpuType::greater_than_zero) {
        calculate_comp(APPROXIMATION_MODE, operation, ADDR_MOD, param5);
    } else if (operation == SfpuType::clamp) {
        calculate_clamp(APPROXIMATION_MODE, ADDR_MOD, param0, param1, param2);
    } else if (operation == SfpuType::abs) {
        calculate_abs(APPROXIMATION_MODE, ADDR_MOD);
    } else if (operation == SfpuType::sign) {
        calculate_sign(APPROXIMATION_MODE, ADDR_MOD, param5);
    }
}

void Math::calculate_exponential(
        bool APPROXIMATION_MODE, 
        bool ZERO_NEGATIVE, 
        bool SCALE_EN, 
        uint32_t ADDR_MOD,
        uint16_t exp_base_scale_factor) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(LREG3, 0, ADDR_MOD, 0); // load from dest
        if (SCALE_EN) {
            SFPLOADI(LREG0, 1, exp_base_scale_factor);
            // input scaling: x * ln(a) = x * exp_base_scale_factor
            SFPMAD(LREG3, LREG0, LCONST_0, LREG3, 0);
            SFPNOP();
        }
        if (APPROXIMATION_MODE) {
            // * by 1/ln2 and add convert to 7.3 FxP format
            // lreg3 = lreg3 * 1.442 + lreg0 (c23_73)
            SFPMAD(LREG3, LCONST_ln2_recip, LREG0, LREG3, 0);
            SFPNOP();

            // Remove Exponent of 7 and bias the Mantissa to 127.
            // LREG2 already holds 2's complement value so we simply do REG2 + REG3
            SFPIADD(0, LREG2, LREG3, 0); // lreg3 = lreg2 + lreg3.

            // SHL to move integer bits to exponent
            SFPSHFT(10 - p_exp::FRAC_BITS, LREG3, LREG3, 1); // lreg3 = lreg3 << 7

            SFPSTORE(LREG3, 0, ADDR_MOD, 0); // Store from lreg3 into dest registers

            // Needed for fused kernels such as math_row_softmax_tables
            // which call calculate_exponential() without using Relu in Packer
            // to clamp -ve Infinity to 0.
            if (ZERO_NEGATIVE) {
                sfpu_enable_cc();
                sfpu_set_cc_from_reg3_sign();
                SFPSTORE(LCONST_0, 0, ADDR_MOD, 0); // Store 0 into dest registers
                sfpu_disable_cc();
            }
        } else {
            // Force sign to 0 (make number positive)
            SFPSETSGN(0, 3, 3, 1);

            sfpu_exp();

            SFPLOAD(1, 0, ADDR_MOD, 0); // load from dest

            if (SCALE_EN) {
                SFPLOADI(LREG0, 1, exp_base_scale_factor);
                // input scaling: x * ln(a) = x * exp_base_scale_factor
                SFPMAD(LREG1, LREG0, LCONST_0, LREG1, 0);
                SFPNOP();
            }

            SFPSTORE(3, 0, ADDR_MOD, 0); // Store from lreg3 into dest registers

            // Enable predication
            sfpu_toggle_enable_cc();
            sfpu_set_cc_from_reg1_sign();

            // CC = sign(X) => reciprocal is conditional
            sfpu_reciprocal(3, ADDR_MOD);
            SFPSTORE(2, 0, ADDR_MOD, 0); // Store from lreg2 into dest registers
            sfpu_toggle_enable_cc();
        }

        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_exponential_body(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    if (APPROXIMATION_MODE) {
        int FRAC_BITS = 3;
        int C23_73 = 0x4340; // Based on FRAC_BITS and INT_BITS (INT_BITS = 6)
        uint32_t SP_BIAS = 127 << FRAC_BITS;

        SFPLOADI(0, 0, C23_73);
        // * by 1 / ln2 and add convert to 7.3 FxP format
        SFPMAD(3, LCONST_ln2_recip, 0, 3, 0); // lreg3 = lreg3 * 1.442 + lreg0 (c23_73)
        SFPNOP();

        // Clear exp bits
        SFPIADD(0, 3, 0, 0x2); // lreg0 = lreg3 - lreg0

        // Add bias
        SFPIADD(SP_BIAS, 0, 0, 1); // lreg0 += SP_BIAS

        // SHL to move integer bits to exponent
        SFPSHFT(10 - FRAC_BITS, 0, 3, 1); // lreg3 = lreg0 << 7
    } else {
        // Force sign to 0 (make number positive)
        SFPSETSGN(0, 3, 3, 1);

        // lreg3 = exp ^ lreg3
        sfpu_exp();

        // Load input value, to determine whether reciprocal needs to be run
        // lreg0 = val
        SFPLOAD(0, 0, ADDR_MOD, 0);

        // store tentatively e ^ x
        // reciprocal function relies on reloading input
        SFPSTORE(3, 0, ADDR_MOD, 0);

        // Enable predication
        sfpu_toggle_enable_cc();
        sfpu_set_cc_from_reg0_sign();

        // CC = sign(X) => reciprocal is conditional
        // if (lreg0 < 0)
        //     lreg3 = 1 / lreg3
        sfpu_reciprocal_lreg_reduced(ADDR_MOD, 3);
        sfpu_toggle_enable_cc();
    }
}

void Math::calculate_gelu_core(bool APPROXIMATION_MODE) {
    uint32_t imm0 = 0x18FF;
    uint32_t imm1 = APPROXIMATION_MODE ? 0x212C : 0x2010;
    uint32_t imm2 = 0xFF00;

    SFPLOADI(0, 2, imm0); //reload lreg0

    // result = (APPROX_MODE == 1) ?
    //     (1 + erf(x / sqrt(2))) :
    //     (1 + tanh(sqrt(2 / pi) * (x + 0.44715 * x ^ 3) )
    if (APPROXIMATION_MODE) {
        SFPLOADI(1, 2, imm1); // reload lreg1
        SFPLOADI(2, 2, imm2); // reload lreg2
    } else {
        // copy to lreg2
        SFPMOV(0x0, 3, 2, 0); // lreg2 = lreg3

        // f = (0.044715 * x ^ 3 + x)
        SFPMUL(3, 3, LCONST_0, 1, 0); // lreg1 = x ^ 2
        SFPNOP();
        SFPMUL(1, 3, LCONST_0, 3, 0); // lreg3 = x * x ^ 2
        SFPNOP();

        SFPMULI(0x3d37, 3, 0); // lreg3 = .044715 * x3
        SFPNOP();

        SFPADD(2, LCONST_1, 3, 3, 0); // lreg3 = lreg3 + lreg2 (x + lreg1)
        SFPNOP();
        SFPLOADI(1, 2, imm1); // reload lreg1
        SFPLOADI(2, 2, imm2); // reload lreg2
        SFPMULI(0x3f4c, 3, 0); // lreg3 = lreg3 * sqrt(2/pi)
        SFPNOP();
    }

    SFPLUT(LREG3, 4, 0);
    SFPNOP();

    SFPADD(LCONST_1, LCONST_1, 3, 3, 0); // lreg3 = 1 + lreg3
    SFPNOP();
    SFPMULI(0x3f00, 3, 0); // lreg3 = lreg3 * 0.5
    SFPNOP();
}

void Math::calculate_gelu(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    uint32_t imm1 = APPROXIMATION_MODE ? 0x212C : 0x2010;
    uint32_t imm2 = 0xFF00;

    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        if (APPROXIMATION_MODE) {
            SFPLOADI(1, 2, imm1); // reload lreg1
            SFPLOADI(2, 2, imm2); // reload lreg2
        } else {
            // copy to lreg2
            SFPMOV(0x0, 3, 2, 0); // lreg2 = lreg3

            // f = 0.044715 * x ^ 3 + x
            SFPMUL(3, 3, LCONST_0, 1, 0); // lreg1 = x ^ 2
            SFPNOP();
            SFPMUL(1, 3, LCONST_0, 3, 0); // lreg3 = x * x ^ 2
            SFPNOP();

            SFPMULI(0x3d37, 3, 0); // lreg3 = .044715 * x3
            SFPNOP();

            SFPADD(2, LCONST_1, 3, 3, 0); // lreg3 = lreg3 + lreg2 (x + lreg1)
            SFPNOP();
            SFPLOADI(1, 2, imm1); // reload lreg1

            SFPLOADI(2, 2, imm2);  // reload lreg2
            SFPMULI(0x3f4c, 3, 0); // lreg3 = lreg3 * sqrt(2 / pi)
            SFPNOP();
        }

        SFPLUT(LREG3, 4, 0);
        SFPNOP();

        SFPLOAD(1, 0, ADDR_MOD, 0); // re-load from dest -> lreg1
        SFPADD(LCONST_1, LCONST_1, 3, 2, 0); // lreg2 = 1 + tanh()
        SFPNOP();
        SFPMULI(0x3f00, 1, 0); // lreg1 = x * 0.5
        SFPNOP();
        SFPMUL(1, 2, LCONST_0, 3, 0); // lreg3 = lreg2 * lreg1
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_sigmoid(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    uint32_t add_imm0 = (0 << 15) | (126 << 7) | (0 << 0);
    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        SFPLUT(LREG3, 4, 0);
        SFPNOP();

        SFPADDI(add_imm0, 3, 0);
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_tanh(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        SFPLUT(LREG3, 4, 0);
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_hardtanh(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD,
        uint32_t param0, 
        uint32_t param1, 
        uint32_t param2) {
    // All params are in FP16_B format
    // param0 = -(neg_threshold)
    // param1 = -(pos_threshold - neg_threshold)
    // param2 = -(pos_threshold)

    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        SFPADDI(param0, 3, 0); // lreg3 = param0 + lreg3
        SFPNOP();

        // Enable predication
        sfpu_toggle_enable_cc();

        // if lreg3 < 0
        SFPSETCC(0, LREG3, 0, 0);
        SFPNOP();
        SFPLOADI(LREG3, 1, 0x0000); // lreg3 = 0

        sfpu_toggle_enable_cc();

        SFPADDI(param1, 3, 0); // lreg3 = param1 + lreg3
        SFPNOP();

        sfpu_toggle_enable_cc();

        // if lreg3 >= 0
        SFPSETCC(0, LREG3, 0, 4);
        SFPNOP();
        SFPLOADI(LREG3, 1, 0x0000); // lreg3 = 0

        // Disable predication
        sfpu_toggle_enable_cc();

        SFPADDI(param2, 3, 0); // lreg3 = param2 + lreg3
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_tanh_derivative(
        bool APPROXIMATION_MODE, 
        int WITH_PRECOMPUTED_TANH, 
        uint32_t ADDR_MOD) {
    // tanh'(x) = 1 - tanh(x) ^ 2
    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        if (!WITH_PRECOMPUTED_TANH) {
            SFPLUT(LREG3, 4, 0); // lreg3 = tanh(x)
            SFPNOP();
        }
        SFPMUL(3, 3, LCONST_0, 3, 0); // lreg3 = lreg3 ^ 2
        SFPNOP();
        SFPADD(3, LCONST_neg1, LCONST_1, 3, 0); // lreg3 = 1 - lreg3
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_gelu_derivative(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    for (int d = 0; d < 8; d++) {
        // lreg3 = x;
        SFPLOAD(3, 0, ADDR_MOD, 0);

        // lreg3 = lreg3 ^ 2
        SFPMUL(LREG3, LREG3, LCONST_0, LREG3, 0);

        // Reload x to lreg1 and keep it saved
        // lreg1 = x;
        SFPLOAD(1, 0, ADDR_MOD, 0);

        // lreg3 = x * (-0.5)
        SFPMULI(0xbf00, 3, 0);
        SFPNOP();

        // Dest = lreg3
        // Store intermediate result since exp(x) reloads value from dest
        SFPSTORE(3, 0, ADDR_MOD, 0);

        // lreg3 = e ^ lreg3
        calculate_exponential_body(false, ADDR_MOD); // Gelu_derivative needs accurate exponent

        // lreg3 = lreg3 * 1 / sqrt(2 * pi)
        SFPMULI(0x3ecc, 3, 0);
        SFPNOP();

        // lreg0 = lreg3 * lreg1
        SFPMUL(LREG3, LREG1, LCONST_0, LREG0, 0); 
        SFPNOP();

        // Store intermediate result
        SFPSTORE(0, 0, ADDR_MOD, 0);

        // lreg3 = lreg1
        SFPMOV(0x0, 1, 3, 0);

        // lreg3 = (APPROX_MODE == 1) ?
        //     0.5 * (1 + erf(x / sqrt(2))) :
        //     0.5 * (1 + tanh(sqrt(2 / pi) * (x + 0.044715 * x ^ 3))
        calculate_gelu_core(true);

        // Load previous result
        SFPLOAD(0, 0, ADDR_MOD, 0);

        // lreg3 = lreg3 + lreg0
        SFPADD(LREG0, LCONST_1, LREG3, LREG3, 0); 
        SFPNOP();

        // dest = lreg3
        SFPSTORE(3, 0, ADDR_MOD, 0);

        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_reciprocal(bool APPROXIMATION_MODE, int ITERATIONS, uint32_t ADDR_MOD) {
    for (int d = 0; d < ITERATIONS; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest

        // Force sign to 0 (make number positive)
        SFPSETSGN(0, 3, 3, 1);

        // Enable predication
        sfpu_toggle_enable_cc();

        if (APPROXIMATION_MODE) {
            sfpu_reciprocal(2, ADDR_MOD);
        } else {
            sfpu_reciprocal(3, ADDR_MOD);
        }

        SFPLOAD(1, 0, ADDR_MOD, 0); // load from dest

        sfpu_set_cc_from_reg1_sign();

        // Invert sign on calculated value if CC=1 (number is negative)
        SFPMOV(0, 2, 2, 1);

        // Disable code conditional
        sfpu_toggle_enable_cc();

        SFPSTORE(2, 0, ADDR_MOD, 0); // Store from lreg2 into dest registers
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_sqrt(
        bool APPROXIMATION_MODE, 
        int ITERATIONS, 
        int RECIPROCAL_ITERATIONS, 
        uint32_t ADDR_MOD) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest
        if (APPROXIMATION_MODE) {
            // sqrt initial approximation
            // adjust bias
            SFPIADD(0, 2, 3, 0); // lreg3 += lreg2
            // approximation of square root
            SFPSHFT(0xfff, 0, 3, 1); // lreg3 >>= 1
            SFPSTORE(3, 0, ADDR_MOD, 0); // Store from lreg3 into dest registers
        } else {
            // check if zero to avoid NAN output
            sfpu_enable_cc();

            // if cond
            sfpu_set_cc_if_not_zero(LREG3);

            // Recip root method
            // calculate y_0 = 1/sqrt(x) using fast inverse square root method,
            // then use iteration to come up with better approximation y_n and
            // finally get sqrt(x) by doing x * y_n
            // Init approx
            // u.i = SQRT_MAGIC_F - (u.i >> 1);
            SFPMOV(0x0, 3, 0, 0); // lreg0 = lreg3

            SFPSHFT(0xfff, 0, 0, 1); // lreg0 >>= 1

            SFPIADD(0, 2, 0, 0x6); // lreg0 = lreg2 - lreg0 (make exponent negative)

            // Reciproot iterations
            for (int r = 0; r < RECIPROCAL_ITERATIONS; r++) {
                // x * r * (1.5f - xhalf * r * r)
                SFPMUL(0, 0, LCONST_0, 1, 0); // lreg1 = r ^ 2 (lreg0)
                SFPNOP();
                SFPMUL(1, 3, LCONST_0, 1, 0); // lreg1 *= x (lreg3)
                SFPNOP();
                SFPMUL(1, LCONST_neg_point_5, LCONST_0, 1, 0); // lreg1 *= -0.5
                SFPNOP();
                SFPADDI(0x3fc0, 1, 0); // lreg1 = 1.5f + lreg1
                SFPNOP();
                SFPMUL(1, 0, LCONST_0, 0, 0); // lreg0 = lreg1 * r (lreg0)
                SFPNOP();
            }

            SFPMUL(0, 3, LCONST_0, 3, 0); // lreg3 = lreg0 * lreg3
            SFPNOP();

            SFPSTORE(3, 0, ADDR_MOD, 0); // Store from lreg3 into dest registers

            sfpu_disable_cc();
        }

        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_dropout(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t prob, 
        uint32_t scale) {
    sfpu_load_imm32(LREG0, 0x0, 0xB400);             // lreg0 = mask
    for (int d = 0; d < 8; d++) {
        // [Scale samples]

        SFPLOADI(LREG1, 0 , scale);                  // lreg1 = scale
        SFPLOAD(LREG2, 0, ADDR_MOD, 0);              // lreg2 = dest
        SFPMUL(LREG2, LREG1, LCONST_0, LREG1, 0);    // lreg1 = lreg2 * lreg1
        SFPNOP();
        SFPSTORE(LREG1, 0, ADDR_MOD, 0);             // Store lreg1 to dest

        // [Drop samples]

        sfpu_load_imm32(LREG2, prob);                // lreg2 <= prob
        SFPMOV(0x0, LREG3, LREG1, 0);                // lreg1 <= current lfsr (lreg3)

        // Compare random num in lreg3 and set cc
        SFPIADD(0, LREG1, LREG2, 0x2);               // lreg2 <= lreg1 - lreg2

        sfpu_toggle_enable_cc();
        sfpu_set_cc_from_reg2_sign();
        // Store (zero) to dest if random number (LFSR state) is less than prob (an integer)
        SFPSTORE(LCONST_0, 0, ADDR_MOD, 0);
        sfpu_toggle_enable_cc();

        // [16-bit PRNG update]

        SFPMOV(0x0, LREG3, LREG2, 0);                // lreg2 <= current lfsr (lreg3)
        SFPAND(0x0, LREG7, LREG2, 0x0);              // lreg2 <= LSB of current lfsr (lreg3)

        // PRNG SHR by one
        SFPSHFT(0xFFF, 0, LREG3, 0x1);               // lreg3 = lreg3 >> 1

        // if sign is negative
        sfpu_toggle_enable_cc();
        sfpu_set_cc_if_not_zero(LREG2);              // Check LSB of LFSR before right shift
        SFPMOV(0x0, LREG3, LREG2, 0);                // lreg2 <= current lfsr (lreg3), shifted

        // XOR breakdown
        SFPAND(0, LREG0, LREG2, 0);                  // lreg2 = lreg2 AND lreg0
        SFPNOT(0, LREG2, LREG2, 0);                  // lreg2 = ~lreg2

        SFPOR(0, LREG0, LREG3, 0);                   // lreg3 = lreg3 OR lreg0

        SFPAND(0, LREG2, LREG3, 0);                  // lreg3 = lreg2 AND lreg3

        sfpu_toggle_enable_cc();

        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_lrelu(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t slope) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(2, 0, ADDR_MOD, 0);	// load from dest

        SFPLOADI(1, 0, slope);

        sfpu_toggle_enable_cc();
        sfpu_set_cc_from_reg2_sign();

        SFPMAD(2, 1, LCONST_0, 2, 0);
        SFPNOP();

        sfpu_toggle_enable_cc();

        SFPSTORE(2, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_power(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t exponent) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(3, 0, ADDR_MOD, 0); // load from dest
        SFPMUL(3, 3, LCONST_0, 1, 0); // lreg1 = x ^ 2
        SFPNOP();

        for (uint32_t i = 2; i < exponent; i++) {
            SFPMUL(1, 3, LCONST_0, 1, 0); // lreg1 = lreg1 * x
            SFPNOP();
        }

        SFPSTORE(1, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_multiply(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    for (int d = 0; d < 8; d++) {
// TODO!
//        WRCFG(p_gpr_math::DEST_OP0_BASE, 0, DEST_REGW_BASE_Base_ADDR32);
        SFPNOP();
        SFPLOAD(2, 0, ADDR_MOD, 0);	// load from dest

// TODO!
//        WRCFG(p_gpr_math::DEST_OP1_BASE, 0, DEST_REGW_BASE_Base_ADDR32);
        SFPNOP();
        SFPLOAD(3, 0, ADDR_MOD, 0);	// load from dest

        SFPMUL(2, 3, LCONST_0, 1, 0); // lreg1 = x ^ 2
        SFPNOP();

        SFPSTORE(1, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_log_body(
        bool HAS_BASE_SCALING, 
        uint32_t ADDR_MOD, 
        uint32_t log_base_scale_factor) {
    // [Load From dest + "normalize to calculation range"]

    // lreg3 = dest
    SFPLOAD(LREG3, 0, ADDR_MOD, 0);
    // set exponent to 127 (force the value to range 1-2)
    SFPSETEXP(127, LREG3, LREG3, 1);
    // Subtract a 1 to account for ln(x + 1) action
    SFPMAD(LREG3, LCONST_1, LCONST_neg1, LREG3, 0);

    // Load coefficients for Horner form multiplication:
    // x * (x * (A * x + B) + C) + D
    sfpu_load_imm32(LREG1, 0x3dd8, 0xadac); // A :0.1058 * x ^ 3
    sfpu_load_imm32(LREG2, 0xbec9, 0xd495); // B: -0.3942 * x ^ 2
    SFPMAD(LREG3, LREG1, LREG2, LREG2, 0);

    sfpu_load_imm32(LREG1, 0x3f7b, 0x367a); // C: 0.9813 * x
    SFPMAD(LREG3, LREG2, LREG1, LREG2, 0);

    sfpu_load_imm32(LREG1, 0x3a1d, 0x4952); // D: 0.006
    SFPMAD(LREG3, LREG2, LREG1, LREG2, 0);

    // lreg2 holds the series result

    // Convert exponent to float
    // Reload argument into lreg3, set lreg1 to 0
    SFPLOAD(LREG3, 0, ADDR_MOD, 0);

    // Extract exponent, debiased, 2's complement integer and convert to sign + magnitude
    sfpu_toggle_enable_cc();
    // set cc flag based on the sign of the extracted exp
    SFPEXEXP(0, LREG3, LREG1, 2);

    // if (lreg1 < 0 )
    //     ~exp + 1
    SFPNOT(0, LREG1, LREG1, 0);
    SFPIADD(1, LREG1, LREG1, 5); // IADD with immediate op = +1
    // set negative sign
    // ACHTUNG: Apparent bug in original code here
    SFPSETSGN(1, LREG1, LREG1, 1);

    sfpu_toggle_enable_cc();

    // Cast to fp32
    SFPCAST(LREG1, LREG1, 0);

    // [Exp Correction + Base Correction]

    // lreg2 contains series result
    // lreg1 contains exponent converted to float
    // exp correction: ln(1 + x) + exp * ln(2)
    SFPMAD(LREG1, LCONST_ln2, LREG2, LREG2, 0); 

    if (HAS_BASE_SCALING) {
        // load constant for non-natural base
        SFPLOADI(LREG0, 1, log_base_scale_factor);  
        // base correction: ln(x) * log_mult_const, log_mult_const is 1 / ln(base)
        SFPMAD(LREG2, LREG0, LCONST_0, LREG2, 0);
        SFPNOP();
    }

    // [Base case when input is 0. ln(0) = -inf]

    sfpu_toggle_enable_cc();
    sfpu_set_cc_if_zero(LREG3);

    // if (lreg3 == 0)
    //     Load -inf
    sfpu_load_imm32(LREG2, 0xff80, 0x0000);

    sfpu_toggle_enable_cc();

    SFPSTORE(LREG2, 0, ADDR_MOD, 0);
}

void Math::calculate_log(
        bool APPROXIMATION_MODE, 
        bool HAS_BASE_SCALING, 
        uint32_t ADDR_MOD, 
        uint32_t log_base_scale_factor) {
    for (int d = 0; d < 8; d++) {
        calculate_log_body(HAS_BASE_SCALING, ADDR_MOD, log_base_scale_factor);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_comp(
        bool APPROXIMATION_MODE, 
        SfpuType COMP_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t exponent_size_8) {
    // invert output and use same comparison check
    bool invert_output = 
        (COMP_MODE == SfpuType::greater_than_equal_zero ||
            COMP_MODE == SfpuType::not_equal_zero ||
            COMP_MODE == SfpuType::greater_than_zero);

    // output_0 and output_1 hold the oututs use use when a zero or negative check is true/false.
    // False = 0.0 = kCONST_0 (5/8-bit exponent format)
    // True  = 1.0 = kCONST_1_FP16B (8-bit exponent format)
    // SFPU uses 8-bit exponent in operations so loading these constants in 8-bit exponent format.
    // Although a command flag can tell SFPU to re-bias a 5-bit exponent to 8-bit, we are loading
    // 8-bit exponent and telling SFPU to not add any bias to these constants.
    auto output_0 = invert_output ? p_sfpu::kCONST_0 : p_sfpu::kCONST_1_FP16B;
    auto output_1 = invert_output ? p_sfpu::kCONST_1_FP16B : p_sfpu::kCONST_0;
    auto instr_mode = p_sfpu::kCONST_Exp_8Bit;

    bool check_zero = 
        (COMP_MODE == SfpuType::equal_zero || 
            COMP_MODE == SfpuType::not_equal_zero);
    bool second_check = 
        (COMP_MODE == SfpuType::less_than_equal_zero || 
            COMP_MODE == SfpuType::greater_than_zero);

    for (int d = 0; d < 8; d++) {
        SFPLOAD(LREG3, 0, ADDR_MOD, 0); // load from dest

        sfpu_enable_cc();
        // if cond
        if (check_zero) {
            if (!exponent_size_8) {
                // this loads bias const into lreg0
                // and modifies lreg3
                sfpu_set_cc_from_reg3_if_zero();
            } else {
                sfpu_set_cc_if_zero (LREG3); // if lreg3 == 0
            }
        }
        else{
            sfpu_set_cc_from_reg3_sign();
        }
        SFPLOADI(LREG3, instr_mode, output_0);

        // else
        sfpu_comp_cc();
        SFPLOADI(LREG3, instr_mode, output_1);

        // Disable predication
        sfpu_disable_cc();

        if (second_check) {
            SFPLOAD(LREG2, 0, ADDR_MOD, 0); // load from dest
            // Enable predication
            sfpu_enable_cc();

            // if cond
            if (!exponent_size_8) {
                sfpu_set_cc_from_reg2_if_zero();
            } else {
                sfpu_set_cc_if_zero (LREG2); // if lreg2 == 0
            }

            SFPLOADI(LREG2, instr_mode, output_0);

            // else
            sfpu_comp_cc();

            SFPLOADI(LREG2, instr_mode, output_1);

            // Disable Predication
            sfpu_disable_cc();

            // sfpu_operation::less_than_equal_zero
            // LREG3 = 0x3F80(1.0) if DST < 0 else 0
            // LREG2 = 0x3F80(1.0) if DST == 0 else 0
            // Do a bitwise Or (LREG2 | LREG3) to get <= condition.
            // LREG3 < 0 OR LREG2 == 0 => DST is Less than or Equal to zero.
            // Result will be either 0x0000(0.0) or 0x3F80(1.0) in LREG3
            if (COMP_MODE == SfpuType::less_than_equal_zero) {
                SFPOR(0, LREG2, LREG3, 0);
            } else {
                // sfpu_operation::greater_than_zero
                // LREG3 = 0x3F80(1.0) if DST >= 0 else 0
                // LREG2 = 0x3F80(1.0) if DST != 0 else 0
                // Do a bitwise And (LREG2 & LREG3) to get > condition.
                // LREG3 >= 0 AND LREG2 != 0 => DST is Greater than zero
                // Result will be either 0x0000(0.0) or 0x3F80(1.0) in LREG3
                SFPAND(0, LREG2, LREG3, 0);
            }

        }
        SFPSTORE(LREG3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_clamp(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t param0, 
        uint32_t param1, 
        uint32_t param2) {
    // All params are in FP16 format
    // param0 = min
    // param1 = max

    uint32_t format = 1;

    for (int d = 0; d < 8; d++) {
        SFPLOAD(LREG3, 0, ADDR_MOD, 0); // load from dest into lreg3
        SFPLOADI(LREG2, format,param0); // Load param0 into lreg2 (format fp16_a or fp16_b)

        SFPMAD(LREG2, LCONST_neg1, LREG3, LREG2, 0); // lreg2 = lreg2 * (-1) + lreg3
        SFPNOP();

        // Enable predication
        sfpu_toggle_enable_cc();

        // if lreg2 < 0
        sfpu_set_cc_if_sign_neg(LREG2);
        SFPLOADI(LREG3, format, param0); // Load param0 into lreg3 if (lreg2 < 0)

        sfpu_toggle_enable_cc();

        SFPLOADI(LREG2, format, param1); // Load param1 into lreg2 (format fp16_a or fp16_b)

        SFPMAD(LREG2, LCONST_neg1, LREG3, LREG2, 0); // lreg2 = lreg2 * (-1) + lreg3
        SFPNOP();

        sfpu_toggle_enable_cc();

        // if lreg4 > 0
        sfpu_set_cc_if_sign_pos(LREG2);
        SFPLOADI(LREG3,format,param1); // Load param1 into lreg3 if (lreg2 > 0)

        // Disable predication
        sfpu_toggle_enable_cc();

        SFPADDI(param2, 3, 0); // lreg3 = param2 + lreg3 (lreg3 = datum - max + max = datum)
        SFPNOP();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_abs(bool APPROXIMATION_MODE, uint32_t ADDR_MOD) {
    for (int d = 0; d < 8; d++) {
        SFPLOAD(LREG3, 0, ADDR_MOD, 0); // load from dest into lreg3
        SFPABS(0, LREG3, LREG2, 1);
        SFPSTORE(2, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::calculate_sign(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t exponent_size_8) {
    // All params are in FP16 format
    for (int d = 0; d < 8; d++) {
        SFPLOAD(LREG2, 0, ADDR_MOD, 0); // load from dest into lreg2
        // Enable predication
        sfpu_enable_cc();
        // if sign is negative
        sfpu_set_cc_if_sign_neg(LREG2);
        SFPMOV(0, LCONST_neg1, LREG3, 0); // Load -1 into lreg3 if (lreg2 < 0)
        // else
        sfpu_comp_cc();
        SFPMOV(0, LCONST_1, LREG3, 0);  // Load +1 into lreg3 if (lreg2 < 0)
        // endif
        sfpu_enable_cc();
        // param0 == 0 is Bfp8 format. It does not require bias removal.
        // param0 != 0 is Float16 format and exp bias needs to be removed for zero check.
        // if cond
        if (!exponent_size_8) {
            // if zero
            sfpu_set_cc_from_reg2_if_zero();
        } else {
            // if zero
            sfpu_set_cc_if_zero (LREG2); // if lreg2 == 0
        }
        SFPMOV(0, LCONST_0, LREG3, 0);
        // endif
        sfpu_disable_cc();

        SFPSTORE(3, 0, ADDR_MOD, 0);
        INCRWC(0, 2, 0, 0);
    }
}

void Math::sfpu_exp() {
    // If exponent is > -1 extract it and replace with -1
    // Extract exponent to lreg0 (debiased)
    sfpu_toggle_enable_cc();

    SFPEXEXP(0, 3, 0, 0xA);

    // If exponent is greater than -1 - set it to -1
    // Set exponent to 126 to make the number in 0-1 range in lreg3
    SFPSETEXP(126, 3, 3, 1);

    sfpu_toggle_enable_cc();

    // Run series in Horner form
    // Load coefficients needed for first mad into lreg1 and lreg2
    SFPLOADI(2, 0, 0x3F5D);  // 0.8634
    SFPMAD(3, LCONST_0_8373, 2, 2, 0); // lreg8 has hard-coded value of 0.8373
    SFPNOP();

    SFPMAD(3, 2, LCONST_1, 3, 0); // lreg10 has hard-coded value of 1.0 (0x3F80)
    SFPNOP();

    // Run predicated loop of squarings
    // Enable predication
    sfpu_toggle_enable_cc();
    SFPSETCC(0, 0, 0, 4);

    for (uint32_t s_iter = 0; s_iter < 8; s_iter++) {
        SFPMAD(3, 3, LCONST_0, 3, 0);
        SFPIADD(0xFFF, 0, 0, 9);
    }

    // Disable predication
    sfpu_toggle_enable_cc();
}

void Math::sfpu_reciprocal(int max_iter, uint32_t ADDR_MOD) {
    if (max_iter == 1) {
        // If we are only doing one iteration of the MAD loop, then we only need to use one LREG
        // for the MAD instructions because we have our "first guess" in a hard-coded register
        // This allows us to avoid having to load back in the original value later on
        SFPEXEXP(0, 3, 2, 0); // Extract exponent from original number to lreg2
    }
    SFPMOV(0, 3, 3, 1); // invert sign on loaded value

    SFPSETEXP(126, 3, 3, 1); // Set exponent to 126 to make the number in 0.5-1 range in lreg3

    SFPLOADI(1, 1, 0x4000); // Load 2.0 into lreg1

    if (max_iter == 1) {
        // If we are only doing one iteration of the MAD loop, then we only need to use one LREG
        // for the MAD instructions because we have our "first guess" in a hard-coded register
        // This allows us to avoid having to load back in the original value later on
        // Use 1.44 as first guess at x (hard-coded in lreg7) -- 
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(3, LCONST_ln2_recip, 1, 0, 0);
        SFPNOP();
        // Use 1.44 as first guess at x (hard-coded in lreg7) -- 
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LCONST_ln2_recip, 0, LCONST_0, 0, 0);
        SFPNOP();
    } else {
        // Use 1.44 as first guess at x (hard-coded in lreg7) --
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(3, LCONST_ln2_recip, 1, 2, 0);  
        SFPNOP();
        // Use 1.44 as first guess at x (hard-coded in lreg7) -- 
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LCONST_ln2_recip, 2, LCONST_0, 0, 0);  
        SFPNOP();
    }
    for (int s_iter = 0; s_iter < max_iter - 1; s_iter++) {
        SFPMAD(3, 0, 1, 2, 0);
        SFPNOP();
        SFPMAD(0, 2, LCONST_0, 0, 0);
        SFPNOP();
    }

    if (max_iter == 1) {
        SFPEXEXP(0, 0, 1, 0); // Extract exponent from result to lreg1
    } else {
        SFPLOAD(3, 0, ADDR_MOD, 0); // reload original number (pre exponent wrangling) into lreg3

        SFPEXEXP(0, 0, 1, 0); // Extract exponent from result to lreg1

        SFPEXEXP(0, 3, 2, 0); // Extract exponent from original number to lreg2
    }

    // Execute: -1 - exp; put result back in lreg2
    // Invert EXP from original number and add 1 to it (2's complement)
    // removes the need to explicitly subtract 1
    SFPNOT(0xFFF, 2, 2, 0);

    // Subtract exponents
    SFPIADD(0, 1, 2, 4);

    // Re-bias exponent
    SFPIADD(127, 2, 2, 5);

    sfpu_push_cc();
    sfpu_set_cc_from_reg2_sign();

    // if rebiased exponent is negative, we need to saturate at 0.
    // This means the initial number was too big so reciprocal result should be 0
    SFPMOV(0, LCONST_0, LREG0, 0); // Move 0 to lreg0
    SFPMOV(0, LCONST_0, LREG2, 0); // Move 0 to lreg2

    sfpu_comp_cc();
    // endif

    sfpu_pop_cc();

    // Set newly denormalized exponent to result exponent field and put number in lreg0
    SFPSETEXP(0, 0, 2, 0);
}

void Math::sfpu_reciprocal_lreg_reduced(uint32_t ADDR_MOD, uint32_t max_iter) {
    // This function performs reciprocal function without using LREG1
    // Perf penalty is that constant 2.0 is being reloaded every time we need it
    // Use this function as part of bigger procedure when one of LREGs keeps some other result

    if (max_iter == 1) {
        // If we are only doing one iteration of the MAD loop,
        // then we only need to use one LREG for the MAD instructions
        // because we have our "first guess" in a hard-coded register
        // This allows us to avoid having to load back in the original value later on

        // Extract exponent from original number to lreg2
        SFPEXEXP(0, 3, 2, 0);
    }

    // invert sign on loaded value
    SFPMOV(0, 3, 3, 1);
    // Set exponent to 126 to make the number in 0.5-1 range in lreg3
    SFPSETEXP(126, 3, 3, 1);

    if (max_iter == 1) {
        // If we are only doing one iteration of the MAD loop,
        // then we only need to use one LREG for the MAD instructions
        // because we have our "first guess" in a hard-coded register
        // This allows us to avoid having to load back in the original value later on

        // lreg0 = 2.0
        SFPLOADI(LREG0, 1, 0x4000);
        // lreg0 = lreg3 * 1.44 + lreg0
        // Use 1.44 as first guess at x (loaded in lreg13) --
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LREG3, LCONST_ln2_recip, LREG0, LREG0, 0);
        SFPNOP();
        // lreg0 = lreg0 * 1.44 + 0
        // Use 1.44 as first guess at x (hard-coded in lreg13) --
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LREG0, LCONST_ln2_recip, LCONST_0, LREG0, 0);
        SFPNOP();
        // Extract exponent from result to lreg3
        SFPEXEXP(0, 0, 3, 0);
    } else {
        // lreg7 = 2.0
        SFPLOADI(LREG7, 1, 0x4000);
        // lreg2 = lreg3 * 1.44 + lreg7
        // Use 1.44 as first guess at x (hard-coded in lreg13) --
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LREG3, LCONST_ln2_recip, LREG7, LREG2, 0); 
        SFPNOP();
        // lreg0 = 1.44 * lreg2 + 0
        // Use 1.44 as first guess at x (hard-coded in lreg13) --
        //     the ideal value would be 1.33, but we happen to have 1.44 available,
        //     so use that to avoid a load
        SFPMAD(LCONST_ln2_recip, LREG2, LCONST_0, LREG0, 0); 
        SFPNOP();

        for (uint32_t s_iter = 1; s_iter < max_iter; s_iter++) {
            // lreg7 = 2.0
            // lreg2 = lreg3 * lreg0 + lreg7
            SFPMAD(LREG3, LREG0, LREG7, LREG2, 0);
            SFPNOP();
            // lreg0 = lreg0 * lreg2 + 0
            SFPMAD(LREG0, LREG2, LCONST_0, LREG0, 0);
            SFPNOP();
        }

        // reload original number (pre exponent wrangling) into lreg3
        SFPLOAD(3, 0, ADDR_MOD, 0);

        // Extract exponent from original number to lreg2
        SFPEXEXP(0, 3, 2, 0);
        // Extract exponent from result to lreg3
        SFPEXEXP(0, 0, 3, 0);
    }

    // Execute: -1 - exp; put result back in lreg2
    // Invert EXP from original number and add 1 to it (2's complement)
    // removes the need to explicitly subtract 1
    SFPNOT(0xFFF, 2, 2, 0);

    // Subtract exponents
    SFPIADD(0, 3, 2, 4);

    // Re-bias exponent
    SFPIADD(127, 2, 2, 5);

    sfpu_push_cc();
    sfpu_set_cc_from_reg2_sign();

    // if rebiased exponent is negative, we need to saturate at 0.
    // This means the initial number was too big so reciprocal result should be 0

    // Move 0 to lreg0
    SFPMOV(0, LCONST_0, LREG0, 0);
    // Move 0 to lreg2
    SFPMOV(0, LCONST_0, LREG2, 0);

    sfpu_comp_cc();
    sfpu_pop_cc();

    // Set newly denormalized exponent to result exponent field and put number in lreg3
    SFPSETEXP(0, 0, 2, 0);
    // lreg3 = lreg2
    SFPMOV(0x0, 2, 3, 0);
}

// helper functions

void Math::sfpu_push_cc() {
    SFPPUSHC(0, 0, 0, 0);
}

void Math::sfpu_pop_cc() {
    SFPPOPC(0, 0, 0, 0);
}

void Math::sfpu_comp_cc() {
    SFPCOMPC(0, 0, 0, 0);
}

void Math::sfpu_toggle_enable_cc() {
    SFPENCC(0, 0, 0, 1);
}

void Math::sfpu_enable_cc() {
    SFPENCC(1, 0, 0, 2);
}

void Math::sfpu_disable_cc() {
    SFPENCC(0, 0, 0, 2);
}

void Math::sfpu_flip_cc_flag() {
    SFPSETCC(0, 0, 0, 8);
}

void Math::sfpu_set_cc_flag() {
    SFPSETCC(1, 0, 0, 1);
}

void Math::sfpu_set_cc_from_reg0_sign() {
    // Set predicate based on sign of lreg0
    SFPSETCC(0, LREG0, 0, 0);
}

void Math::sfpu_set_cc_from_reg1_sign() {
    // Set predicate based on sign of lreg1
    SFPSETCC(0, LREG1, 0, 0);
}

void Math::sfpu_set_cc_from_reg2_sign() {
    // Set predicate based on sign of lreg2
    SFPSETCC(0, LREG2, 0, 0);
}

void Math::sfpu_set_cc_from_reg3_sign() {
    // Set predicate based on sign of lreg3
    SFPSETCC(0, LREG3, 0, 0);
}

void Math::sfpu_set_cc_from_reg3_if_zero() {
    // if math data format is fp16, SFPU has to convert 5 bit exp to 8 bit exp
    // in grayskull, this unconditionally adds bias value to exp (even for zero)
    // in wormhole, this bug is fixed and zero will be 0b'0
    // Set predicate based on value of lreg3 being 0
    SFPSETCC(0, LREG3, 0, 6);
}

void Math::sfpu_set_cc_from_reg2_if_zero() {
    // Set predicate based on value of lreg2 being 0
    SFPSETCC(0, LREG2, 0, 6);
}

void Math::sfpu_set_cc_if_sign_neg(uint32_t reg) {
    SFPSETCC(0, reg, 0, 0);
}

void Math::sfpu_set_cc_if_sign_pos(uint32_t reg) {
    SFPSETCC(0, reg, 0, 4);
}

void Math::sfpu_set_cc_if_zero(uint32_t reg) {
    SFPSETCC(0, reg, 0, 6);
}

void Math::sfpu_set_cc_if_not_zero(uint32_t reg) {
    SFPSETCC(0, reg, 0, 2);
}

void Math::sfpu_load_imm32(
        const uint32_t dest, 
        const uint32_t upper16, 
        const uint32_t lower16) {
    // insmod == A will write the lower bits, and not affect the upper bits
    // insmod == 8 will write the upper bits, and not affect the lower bits
    SFPLOADI(dest, 0xA, lower16);
    SFPLOADI(dest, 0x8, upper16);
}

void Math::sfpu_load_imm32(const uint32_t dest, const uint32_t val) {
    // insmod == A will write the lower bits, and not affect the upper bits
    // insmod == 8 will write the upper bits, and not affect the lower bits
    SFPLOADI(dest, 0xA, (val & 0xFFFF));
    SFPLOADI(dest, 0x8, (val >> 16) & 0xFFFF);
}

#if 0 // TODO: Revise this
void Math::sfpu_access_even_cols() {
    // Using insmod == 5 means to use the immediate field as an AND mask
    // This restores the read/write columns to default value of 0 to point to even columns
    SFPCONFIG(0xFF3F, 15, 0x5);
    SFPNOP();
}

void Math::sfpu_access_odd_cols() {
    // Using insmod == 3 means to use the immediate field as an OR mask
    // This will flip the read/write columns (bits 6 and 7)
    SFPCONFIG(0x00C0, 15, 0x3);
    SFPNOP();
}
#endif

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

