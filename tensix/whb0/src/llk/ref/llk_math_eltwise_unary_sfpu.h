// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_addrmod.h"
#include "ckernel_defs.h"
#include "ckernel_sfpu.h"
#include "cmath_common.h"

#include "llk_defs.h"

BEGIN_NS_LLK

using namespace ckernel;

template <SfpuType sfpu_type>
void static_assert_sfpu_type_dependent() {
    static_assert(sfpu_type == SfpuType::unused, "sfpu_type exception");
}

// local function declarations
template <SfpuType sfpu_op>
inline void eltwise_unary_sfpu_configure_addrmod() {
    // NOTE: This kernel is typically used in conjunction with A2D,
    //    which is using ADDR_MOD_0 and ADDR_MOD_2, so use one that doesn't conflict
    addr_mod_t {
        .srca = {.incr = 0},
        .srcb = {.incr = 0},
        .dest = {.incr = 0},
    }
    .set(ADDR_MOD_7);
}

inline void eltwise_unary_sfpu_configure_mop();

template <SfpuType sfpu_op, bool APPROXIMATE, DstSync Dst = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu(
        uint32_t dst_index,
        int vector_mode = Dim::RC,
        uint32_t param0 = 0,
        uint32_t param1 = 0,
        uint32_t param2 = 0,
        uint32_t param3 = 0,
        uint32_t param4 = 0,
        uint32_t param5 = 0) {
    if constexpr (Dst == DstSync::SyncTile16 || Dst == DstSync::SyncTile2) {
        math::set_dst_write_addr<
            DstTileLayout::Default, DstTileShape::Tile32x32>(
                math_sync_tile_dst_index);
    } else {
        math::set_dst_write_addr<DstTileLayout::Default, DstTileShape::Tile32x32>(dst_index);
    }

    math::set_addr_mod_base();
    TTI_STALLWAIT(p_stall::STALL_SFPU, p_stall::MATH);

    if (vector_mode == Dim::R) {
        // Do a row vector, Face0 + Face1 -- first iteration (first row)
        const int ITERATIONS = 1;
#pragma GCC unroll 0
        for (int face = 0; face < 2; face++) {
            sfpu::calculate_sfpu<
                sfpu_op, APPROXIMATE, 0, ITERATIONS>(
                    param0, param1, param2, param3, param4, param5);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        }
        // Skip the next 2 faces
        TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
    } else if (vector_mode == Dim::C) {
        // Do a column vector, Face0 + Face2 -- All iterations for full face
#pragma GCC unroll 0
        for (int face = 0; face < 2; face++) {
            sfpu::calculate_sfpu<
                sfpu_op, APPROXIMATE, 0>(
                    param0, param1, param2, param3, param4, param5);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        }
    } else {
        // Do all four faces, and iterate through all 4 blocks of 4 rows each
#pragma GCC unroll 0
        for (int face = 0; face < 4; face++) {
            sfpu::calculate_sfpu<
                sfpu_op, APPROXIMATE, 0>(
                    param0, param1, param2, param3, param4, param5);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
            TTI_SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        }
    }
    math::clear_dst_reg_addr();

    TTI_STALLWAIT(p_stall::STALL_CFG, p_stall::WAIT_SFPU);
    math::clear_addr_mod_base();
}

template <SfpuType sfpu_op, bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_init(
        uint32_t param0 = 0, 
        uint32_t param1 = 0, 
        uint32_t param2 = 0, 
        uint32_t param3 = 0, 
        uint32_t param4 = 0, 
        uint32_t param5 = 0) {
    eltwise_unary_sfpu_configure_addrmod< sfpu_op >();
    if constexpr (sfpu_op == SfpuType::dropout) {
        sfpu::sfpu_init<APPROXIMATE>(sfpu_op, param2);
    } else {
        sfpu::sfpu_init<APPROXIMATE>(sfpu_op);
    }
    math::reset_counters(p_setrwc::SET_ABD_F);
}

// New LLK SFPU APIs

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_rsqrt(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::rsqrt, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_rsqrt_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::rsqrt, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_log(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::log, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_log_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::log, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_log_with_base(uint32_t dst_index,uint32_t base_scale) {
    llk_math_eltwise_unary_sfpu<
        SfpuType::log_with_base, APPROXIMATE, dst_sync>(
            dst_index, base_scale);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_log_with_base_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::log_with_base, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_tanh(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::tanh, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_signbit(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::signbit, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_signbit_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::signbit, APPROXIMATE>();
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_tanh_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::tanh, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_sign(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::sign, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_sign_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::sign, APPROXIMATE>();
}

template <DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_dropout(
        uint32_t dst_index, 
        int vector_mode, 
        int integer_dropout, 
        int scale_factor) {
    constexpr bool dont_care = false;
    llk_math_eltwise_unary_sfpu<
        SfpuType::dropout, dont_care, dst_sync>(
            dst_index, vector_mode, integer_dropout, scale_factor);
}

inline void llk_math_eltwise_unary_sfpu_dropout_init(uint32_t seed = 0) {
    constexpr bool dont_care = false;
    constexpr uint32_t dont_care_param = 0;
    llk_math_eltwise_unary_sfpu_init<
        SfpuType::dropout, dont_care>(
            dont_care_param, dont_care_param, seed);
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_sigmoid(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::sigmoid, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_sigmoid_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::sigmoid, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_eqz(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::equal_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_eqz_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::equal_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_nez(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::not_equal_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_nez_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::not_equal_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_ltz(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::less_than_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_ltz_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::less_than_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_gtz(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::greater_than_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_gtz_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::greater_than_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_lez(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::less_than_equal_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_lez_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::less_than_equal_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_gez(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::greater_than_equal_zero, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_gez_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::greater_than_equal_zero, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_max(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::max, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_max_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::max, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_square(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::square, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_square_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::square, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_power(
        uint32_t dst_index, 
        int pow = 0, 
        int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<
        SfpuType::power, APPROXIMATE, dst_sync>(
            dst_index, vector_mode, pow);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_power_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::power, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_abs(uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<SfpuType::abs, APPROXIMATE, dst_sync>(dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_abs_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::abs, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_cast_fp32_to_fp16a(
        uint32_t dst_index, int vector_mode = Dim::RC) {
    llk_math_eltwise_unary_sfpu<
        SfpuType::cast_fp32_to_fp16a, APPROXIMATE, dst_sync>(
            dst_index, vector_mode);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_cast_fp32_to_fp16a_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::cast_fp32_to_fp16a, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_exp2(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::exp2, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_exp2_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::exp2, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_heaviside(uint32_t dst_index, uint32_t param0) {
    llk_math_eltwise_unary_sfpu<SfpuType::heaviside, APPROXIMATE, dst_sync>(dst_index, param0);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_heaviside_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::heaviside, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_expm1(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::expm1, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_expm1_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::expm1, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_asin(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::asin, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_asin_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::asin, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_atan(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::atan, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_atan_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::atan, APPROXIMATE>();
}

template <bool APPROXIMATE, DstSync dst_sync = DstSync::SyncFull>
inline void llk_math_eltwise_unary_sfpu_acos(uint32_t dst_index) {
    llk_math_eltwise_unary_sfpu<SfpuType::acos, APPROXIMATE, dst_sync>(dst_index);
}

template <bool APPROXIMATE>
inline void llk_math_eltwise_unary_sfpu_acos_init() {
    llk_math_eltwise_unary_sfpu_init<SfpuType::acos, APPROXIMATE>();
}

END_NS_LLK

