
#include <cstdint>
#include <cassert>

#include "core/addr_mod.hpp"
#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/math.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::AddrMod;

using core::p_setrwc;
using core::p_stall;

//
//    Math
//

void Math::math_eltwise_unary_sfpu_exponential_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::exponential, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_sqrt_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::sqrt, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_gelu_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::gelu, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_gelu_derivative_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::gelu_derivative, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_log_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::log, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_reciprocal_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::reciprocal, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_tanh_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::tanh, APPROXIMATE);
}

void Math::math_eltwise_unary_sfpu_dropout_init(uint32_t seed) {
    constexpr bool dont_care = false;
    constexpr uint32_t dont_care_param = 0;

    math_eltwise_unary_sfpu_init(
        SfpuType::dropout, 
        dont_care, 
        dont_care_param, 
        dont_care_param, 
        seed);
}

void Math::math_eltwise_unary_sfpu_sigmoid_init(bool APPROXIMATE) {
    math_eltwise_unary_sfpu_init(SfpuType::sigmoid, APPROXIMATE);
} 

void Math::math_eltwise_unary_sfpu_exponential(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::exponential, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_sqrt(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::sqrt, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_gelu(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::gelu, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_gelu_derivative(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::gelu_derivative, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_reciprocal(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::reciprocal, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_log(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::log, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_tanh(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::tanh, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_dropout(
        DstSync dst_sync, 
        uint32_t dst_index, 
        int integer_dropout, 
        int scale_factor) {
    constexpr bool dont_care = false;
    math_eltwise_unary_sfpu(
        SfpuType::dropout, 
        dont_care, 
        dst_sync, 
        dst_index, 
        integer_dropout, 
        scale_factor);
}

void Math::math_eltwise_unary_sfpu_sigmoid(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index) {
    math_eltwise_unary_sfpu(SfpuType::sigmoid, APPROXIMATE, dst_sync, dst_index);
}

void Math::math_eltwise_unary_sfpu_init(
        SfpuType sfpu_op, 
        bool APPROXIMATE,
        uint32_t param0/* = 0*/, 
        uint32_t param1/* = 0*/, 
        uint32_t param2/* = 0*/, 
        uint32_t param3/* = 0*/, 
        uint32_t param4/* = 0*/, 
        uint32_t param5/* = 0*/) {
    eltwise_unary_sfpu_configure_addrmod();
    if (sfpu_op == SfpuType::dropout) {
        sfpu_init(APPROXIMATE, sfpu_op, param2);
    } else {
        sfpu_init(APPROXIMATE, sfpu_op, 0);
    }
    reset_counters(p_setrwc::SET_ABD_F);
} 

void Math::math_eltwise_unary_sfpu(
        SfpuType sfpu_op, 
        bool APPROXIMATE, 
        DstSync Dst,
        uint32_t dst_index,
        uint32_t param0/* = 0*/,
        uint32_t param1/* = 0*/,
        uint32_t param2/* = 0*/,
        uint32_t param3/* = 0*/,
        uint32_t param4/* = 0*/,
        uint32_t param5/* = 0*/) {
    STALLWAIT(p_stall::STALL_SFPU, p_stall::MATH);
    if (Dst == DstSync::SyncTile16 || Dst == DstSync::SyncTile2) {
        set_dst_write_addr(
            DstTileLayout::Default, 
            DstTileShape::Tile32x32, 
            get_math_sync_tile_dst_index());
    } else {
        set_dst_write_addr(DstTileLayout::Default, DstTileShape::Tile32x32, dst_index);
    }
    for (int face = 0; face < 4; face++) {
        calculate_sfpu(
            sfpu_op, 
            APPROXIMATE, 
            0, // SfpuType_PARAM
            8, // ITERATIONS
            3, // ADDR_MOD
            param0, 
            param1, 
            param2, 
            param3, 
            param4, 
            param5);
        SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
        SETRWC(p_setrwc::CLR_NONE, p_setrwc::CR_D, 8, 0, 0, p_setrwc::SET_D);
    }
    clear_dst_reg_addr();
}

void Math::eltwise_unary_sfpu_configure_addrmod() {
    // NOTE: this kernel is typically used in conjunction with
    //       A2D, which is using ADDR_MOD_0 and ADDR_MOD_2, so use one
    //       that doesn't conflict!
    AddrMod a3;
    a3.srca.incr = 0;
    a3.srcb.incr = 0;
    a3.dest.incr = 0;
    set_addr_mod(3, a3);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

