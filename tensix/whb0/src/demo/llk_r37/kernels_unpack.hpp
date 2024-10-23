#pragma once

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {
namespace unpack {

void binary_add_main();
void bcast_h_add_main();
void bcast_w_add_main();
void bcast_hw_add_main();
void matmul_main();
void reduce_h_sum_main();
void reduce_w_sum_main();
void reduce_hw_sum_main();
void tilize_main();
void untilize_main();

void sfpu_tanh_main();
void sfpu_hardtanh_main();
void sfpu_gelu_main();
void sfpu_exponential_main();
void sfpu_exp_with_base_main();
void sfpu_sigmoid_main();
void sfpu_reciprocal_main();
void sfpu_sqrt_main();
void sfpu_lrelu_main();
void sfpu_power_main();
void sfpu_square_main();
void sfpu_tanh_derivative_main();
void sfpu_log_main();
void sfpu_log_with_base_main();
void sfpu_equal_zero_main();
void sfpu_not_equal_zero_main();
void sfpu_less_than_zero_main();
void sfpu_greater_than_equal_zero_main();
void sfpu_less_than_equal_zero_main();
void sfpu_greater_than_zero_main();
void sfpu_clamp_main();
void sfpu_gelu_derivative_main();
void sfpu_dropout_main();
void sfpu_abs_main();
void sfpu_sign_main();
void sfpu_max_main();
void sfpu_sine_main();
void sfpu_cosine_main();
void sfpu_tan_main();
void sfpu_relu_max_main();
void sfpu_relu_min_main();
void sfpu_cast_fp32_to_fp16a_main();
void sfpu_sigmoid_appx_main();
void sfpu_gelu_appx_main();
void sfpu_elu_main();
void sfpu_min_main();
void sfpu_exp2_main();
void sfpu_heaviside_main();
void sfpu_expm1_main();
void sfpu_signbit_main();
void sfpu_asin_main();
void sfpu_acos_main();
void sfpu_atan_main();
void sfpu_erf_main();
void sfpu_erfc_main();
void sfpu_rsqrt_main();
void sfpu_isfinite_main();
void sfpu_isinf_main();
void sfpu_isposinf_main();
void sfpu_isneginf_main();
void sfpu_isnan_main();
void sfpu_logical_not_unary_main();
void sfpu_erfinv_main();
void sfpu_i0_main();

} // namespace unpack
} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

