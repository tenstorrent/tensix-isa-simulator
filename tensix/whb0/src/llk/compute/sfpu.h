#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_eltwise_unary_sfpu.h"
#include "llk_math_eltwise_unary_sfpu_elu.h"
#include "llk_math_eltwise_unary_sfpu_erf_erfc.h"
#include "llk_math_eltwise_unary_sfpu_erfinv.h"
#include "llk_math_eltwise_unary_sfpu_exp.h"
#include "llk_math_eltwise_unary_sfpu_gelu.h"
#include "llk_math_eltwise_unary_sfpu_i0.h"
#include "llk_math_eltwise_unary_sfpu_isinf_isnan.h"
#include "llk_math_eltwise_unary_sfpu_logical_not_noti.h"
#include "llk_math_eltwise_unary_sfpu_recip.h"
#include "llk_math_eltwise_unary_sfpu_relu.h"
#include "llk_math_eltwise_unary_sfpu_sqrt.h"
#include "llk_math_eltwise_unary_sfpu_trigonometry.h"
#endif

BEGIN_NS_COMPUTE

inline void rsqrt_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_rsqrt_init<APPROX>()));
}

inline void rsqrt_tile(uint32_t idst, bool fast_and_approx = true) {
    if (fast_and_approx) {
        MATH((llk_math_eltwise_unary_sfpu_rsqrt<true, SyncHalf>(idst)));
    } else {
        MATH((llk_math_eltwise_unary_sfpu_rsqrt<false, SyncHalf>(idst)));
    }
}

inline void sigmoid_tile_init() {
    // TODO(AP): move out init
    MATH((llk_math_eltwise_unary_sfpu_sigmoid_init<APPROX>()));
}

inline void sigmoid_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_sigmoid<APPROX, SyncHalf>(idst)));
}

inline void log_tile_init() {
    // TODO(AP): move out init
    MATH((llk_math_eltwise_unary_sfpu_log_init<APPROX>()));
}

inline void log_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_log<APPROX, SyncHalf>(idst)));
}

inline void log_with_base_tile_init() {
    // TODO(AP): move out init
    MATH((llk_math_eltwise_unary_sfpu_log_with_base_init<APPROX>()));
}

inline void log_with_base_tile(uint32_t idst, uint32_t base_scale) {
    MATH((llk_math_eltwise_unary_sfpu_log_with_base<APPROX, SyncHalf>(idst, base_scale)));
}

inline void tanh_tile_init() {
    // TODO(AP): move out init
    MATH((llk_math_eltwise_unary_sfpu_tanh_init<APPROX>()));
}

inline void tanh_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_tanh<APPROX, SyncHalf>(idst)));
}

inline void signbit_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_signbit_init<APPROX>()));
}

inline void signbit_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_signbit<APPROX, SyncHalf>(idst)));
}

inline void abs_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_abs_init<APPROX>()));
}

inline void abs_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_abs<APPROX, SyncHalf>(idst)));
}

inline void sign_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_sign_init<APPROX>()));
}

inline void sign_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_sign<APPROX, SyncHalf>(idst)));
}

inline void square_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_square_init<APPROX>()));
}

inline void square_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_square<APPROX, SyncHalf>(idst)));
}

inline void ltz_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_ltz_init<APPROX>()));
}

inline void ltz_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_ltz<APPROX, SyncHalf>(idst)));
}

inline void eqz_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_eqz_init<APPROX>()));
}

inline void eqz_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_eqz<APPROX, SyncHalf>(idst)));
}

inline void lez_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_lez_init<APPROX>()));
}

inline void lez_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_lez<APPROX, SyncHalf>(idst)));
}

inline void gtz_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_gtz_init<APPROX>()));
}

inline void gtz_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_gtz<APPROX, SyncHalf>(idst)));
}

inline void nez_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_nez_init<APPROX>()));
}

inline void nez_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_nez<APPROX, SyncHalf>(idst)));
}

inline void gez_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_gez_init<APPROX>()));
}

inline void gez_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_gez<APPROX, SyncHalf>(idst)));
}

inline void power_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_power_init<APPROX>()));
}

// POWER : y = x^(const param0)
inline void power_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_power<APPROX, SyncHalf>(idst, param0)));
}

#if 0 // SKIPPED
inline void graph_interpreter_init() { 
    // TODO(AP): probably duplicated, remove
    MATH((llk_math_pack_sync_init<SyncHalf>()));
    PACK((llk_pack_init()));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SyncHalf, DstTileFaceLayout::RowMajor, false>()));
    PACK((llk_pack_hw_configure_disaggregated<false>(16)));
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_hw_configure_disaggregated(0, 0)));
}
inline void get_next_op_info(tt::op_info_t &op_info) {
    MATH((llk_get_next_op_info(op_info)));
    PACK((llk_get_next_op_info(op_info)));
    UNPACK((llk_get_next_op_info(op_info)));
}

#endif

inline void exp2_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_exp2_init<true>()));
}

// exp2 : y = 2 ^ x  ==> [y = exp(x * log(2))]
inline void exp2_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_exp2<true, SyncHalf>(idst)));
}

inline void heaviside_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_heaviside_init<APPROX>()));
}

// heaviside : y = 0 if x < 0 , 1 if x > 0 , else value
inline void heaviside_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_heaviside<APPROX, SyncHalf>(idst, param0)));
}

inline void expm1_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_expm1_init<true>()));
}

// expm1 : (exp(x) - 1)
inline void expm1_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_expm1<true, SyncHalf>(idst)));
}

inline void asin_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_asin_init<true>()));
}

inline void asin_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_asin<true, SyncHalf>(idst)));
}

inline void atan_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_atan_init<true>()));
}

inline void atan_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_atan<true, SyncHalf>(idst)));
}

inline void acos_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_acos_init<true>()));
}

inline void acos_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_acos<true, SyncHalf>(idst)));
}

// eltwise_binary/elu.h

inline void elu_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_elu_init<APPROX>()));
}

// elu : y = relu(x) + slope*(exp(x) - 1)*(x <= 0 );
inline void elu_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_elu<APPROX, SyncHalf>(idst, param0)));
}

// eltwise_binary/erf_erfc.h

inline void erf_tile_init() { 
    MATH((llk_math_eltwise_unary_sfpu_erf_init<APPROX>())); 
}

inline void erf_tile(uint32_t idst, bool fast_and_approx = true) {
    if (fast_and_approx) {
        MATH((llk_math_eltwise_unary_sfpu_erf<true, SyncHalf>(idst)));
    } else {
        MATH((llk_math_eltwise_unary_sfpu_erf<false, SyncHalf>(idst)));
    }
}

inline void erfc_tile_init() { 
    MATH((llk_math_eltwise_unary_sfpu_erfc_init<APPROX>())); 
}

inline void erfc_tile(uint32_t idst, bool fast_and_approx = true) {
    if (fast_and_approx) {
        MATH((llk_math_eltwise_unary_sfpu_erfc<true, SyncHalf>(idst)));
    } else {
        MATH((llk_math_eltwise_unary_sfpu_erfc<false, SyncHalf>(idst)));
    }
}

// eltwise_binary/erfinv.h

inline void erfinv_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_erfinv_init<APPROX>()));
}

inline void erfinv_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_erfinv_op<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/exp.h

inline void exp_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_exponential_init<APPROX>()));
}

inline void exp_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_exponential<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/gelu.h

inline void gelu_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_gelu_init<APPROX>()));
}

inline void gelu_tile(uint32_t idst, bool fast_and_approx=true) {
    if (fast_and_approx) {
        MATH((llk_math_eltwise_unary_sfpu_gelu<true, SyncHalf>(idst)));
    } else {
        MATH((llk_math_eltwise_unary_sfpu_gelu<false, SyncHalf>(idst)));
    }
}

// eltwise_binary/i0.h

inline void i0_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_i0_init<APPROX>()));
}

inline void i0_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_i0_op<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/isinf_isnan.h

inline void isinf_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_isinf_init<APPROX>()));
}

inline void isinf_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_isinf<APPROX, SyncHalf>(idst)));
}

inline void isposinf_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_isposinf_init<APPROX>()));
}

inline void isposinf_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_isposinf<APPROX, SyncHalf>(idst)));
}

inline void isneginf_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_isneginf_init<APPROX>()));
}

inline void isneginf_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_isneginf<APPROX, SyncHalf>(idst)));
}

inline void isnan_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_isnan_init<APPROX>()));
}

inline void isnan_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_isnan<APPROX, SyncHalf>(idst)));
}

inline void isfinite_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_isfinite_init<APPROX>()));
}

inline void isfinite_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_isfinite<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/logical_not_noti.h

inline void logical_not_unary_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_logical_not_unary_init<APPROX>()));
}

inline void logical_not_unary_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_logical_not_unary_op<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/recip.h

inline void recip_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_reciprocal_init<APPROX>()));
}

inline void recip_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_reciprocal<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/relu.h

inline void relu_max_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_relu_max_init<APPROX>()));
}

inline void relu_max_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_relu_max<APPROX, SyncHalf>(idst, param0)));
}

inline void relu_min_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_relu_min_init<APPROX>()));
}

inline void relu_min_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_relu_min<APPROX, SyncHalf>(idst, param0)));
}

inline void relu_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_relu_init<APPROX>()));
}

inline void relu_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_relu<APPROX, SyncHalf>(idst)));
}

inline void leaky_relu_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_lrelu_init<APPROX>()));
}

// Leaky Relu : y = relu(x) + slope*-relu(-x)
inline void leaky_relu_tile(uint32_t idst, uint32_t param0) {
    MATH((llk_math_eltwise_unary_sfpu_lrelu<APPROX, SyncHalf>(idst, param0)));
}

// eltwise_binary/sqrt.h

inline void sqrt_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_sqrt_init<APPROX>()));
}

inline void sqrt_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_sqrt<APPROX, SyncHalf>(idst)));
}

// eltwise_binary/trigonometry.h

inline void sin_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_sine_init<APPROX>()));
}

inline void sin_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_sine_op<APPROX, SyncHalf>(idst)));
}

inline void cos_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_cosine_init<APPROX>()));
}

inline void cos_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_cosine_op<APPROX, SyncHalf>(idst)));
}

inline void tan_tile_init() {
    MATH((llk_math_eltwise_unary_sfpu_tan_init<APPROX>()));
}

inline void tan_tile(uint32_t idst) {
    MATH((llk_math_eltwise_unary_sfpu_tan_op<APPROX, SyncHalf>(idst)));
}

END_NS_COMPUTE

