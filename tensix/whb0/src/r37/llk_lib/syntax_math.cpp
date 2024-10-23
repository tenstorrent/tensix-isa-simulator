
#include "ckernel_core.h"
#include "ckernel_defs.h"

#include "llk_defs.h"
#include "llk_math_common.h"
#include "llk_math_eltwise_binary.h"
#include "llk_math_eltwise_unary_datacopy.h"
#include "llk_math_eltwise_unary_sfpu.h"
#include "llk_math_matmul.h"
#include "llk_math_reduce.h"

namespace syntax {

using namespace ckernel;

//
//    Dummy code just to test compilation of templates
//

void test_llk_math_common() {
    llk_math_wait_for_dest_available<DstSync::SyncFull>();
    llk_math_dest_section_done();
    llk_math_pack_sync_init<DstSync::SyncFull>();
}

void test_llk_math_eltwise_binary() {
    eltwise_binary_reuse_dest_as_src();
    llk_math_eltwise_binary_impl<EltwiseBinaryType::ELWADD, BroadcastType::NONE>(
        4,      // num_faces_a
        4,      // num_faces_b
        0,      // dst_index
        false); // clear_fp32_dst_acc
    llk_math_eltwise_binary<EltwiseBinaryType::ELWADD, BroadcastType::NONE>(0);
    llk_math_eltwise_binary<EltwiseBinaryType::ELWADD, BroadcastType::NONE>(0, 1, 0);
    eltwise_binary_configure_addrmod<EltwiseBinaryType::ELWADD, BroadcastType::NONE>();
    eltwise_binary_configure_mop<EltwiseBinaryType::ELWADD, BroadcastType::NONE>();
    llk_math_eltwise_binary_init_impl<EltwiseBinaryType::ELWADD, BroadcastType::NONE>(0, 0, 0);
    llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::NONE>();
    llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::NONE>(0, 1);
}

void test_llk_math_eltwise_unary_datacopy() {
    llk_math_eltwise_unary_datacopy<DataCopyType::A2D>(0);
    eltwise_unary_configure_addrmod<DataCopyType::A2D>();
    eltwise_unary_configure_mop<DataCopyType::A2D>(4, 16, 0);
    llk_math_eltwise_unary_datacopy_init<DataCopyType::A2D>();
}

void test_llk_math_eltwise_unary_sfpu() {
    eltwise_unary_sfpu_configure_addrmod<SfpuType::tanh>();
    llk_math_eltwise_unary_sfpu<SfpuType::tanh, false>(0);
    llk_math_eltwise_unary_sfpu_init<SfpuType::tanh, false>();
    llk_math_eltwise_unary_sfpu_rsqrt<false>(0);
    llk_math_eltwise_unary_sfpu_rsqrt_init<false>();
    llk_math_eltwise_unary_sfpu_log<false>(0);
    llk_math_eltwise_unary_sfpu_log_init<false>();
    llk_math_eltwise_unary_sfpu_log_with_base<false>(0, 1);
    llk_math_eltwise_unary_sfpu_log_with_base_init<false>();
    llk_math_eltwise_unary_sfpu_tanh<false>(0);
    llk_math_eltwise_unary_sfpu_signbit<false>(0);
    llk_math_eltwise_unary_sfpu_signbit_init<false>();
    llk_math_eltwise_unary_sfpu_tanh_init<false>();
    llk_math_eltwise_unary_sfpu_sign<false>(0);
    llk_math_eltwise_unary_sfpu_sign_init<false>();
    llk_math_eltwise_unary_sfpu_dropout(0, 0, 1, 1);
    llk_math_eltwise_unary_sfpu_dropout_init();
    llk_math_eltwise_unary_sfpu_sigmoid<false>(0);
    llk_math_eltwise_unary_sfpu_sigmoid_init<false>();
    llk_math_eltwise_unary_sfpu_eqz<false>(0);
    llk_math_eltwise_unary_sfpu_eqz_init<false>();
    llk_math_eltwise_unary_sfpu_nez<false>(0);
    llk_math_eltwise_unary_sfpu_nez_init<false>();
    llk_math_eltwise_unary_sfpu_ltz<false>(0);
    llk_math_eltwise_unary_sfpu_ltz_init<false>();
    llk_math_eltwise_unary_sfpu_gtz<false>(0);
    llk_math_eltwise_unary_sfpu_gtz_init<false>();
    llk_math_eltwise_unary_sfpu_lez<false>(0);
    llk_math_eltwise_unary_sfpu_lez_init<false>();
    llk_math_eltwise_unary_sfpu_gez<false>(0);
    llk_math_eltwise_unary_sfpu_gez_init<false>();
    llk_math_eltwise_unary_sfpu_max<false>(0);
    llk_math_eltwise_unary_sfpu_max_init<false>();
    llk_math_eltwise_unary_sfpu_square<false>(0);
    llk_math_eltwise_unary_sfpu_square_init<false>();
    llk_math_eltwise_unary_sfpu_power<false>(0);
    llk_math_eltwise_unary_sfpu_power_init<false>();
    llk_math_eltwise_unary_sfpu_abs<false>(0);
    llk_math_eltwise_unary_sfpu_abs_init<false>();
    llk_math_eltwise_unary_sfpu_cast_fp32_to_fp16a<false>(0);
    llk_math_eltwise_unary_sfpu_cast_fp32_to_fp16a_init<false>();
    llk_math_eltwise_unary_sfpu_exp2<false>(0);
    llk_math_eltwise_unary_sfpu_exp2_init<false>();
    llk_math_eltwise_unary_sfpu_heaviside<false>(0, 1);
    llk_math_eltwise_unary_sfpu_heaviside_init<false>();
    llk_math_eltwise_unary_sfpu_expm1<false>(0);
    llk_math_eltwise_unary_sfpu_expm1_init<false>();
    llk_math_eltwise_unary_sfpu_asin<false>(0);
    llk_math_eltwise_unary_sfpu_asin_init<false>();
    llk_math_eltwise_unary_sfpu_atan<false>(0);
    llk_math_eltwise_unary_sfpu_atan_init<false>();
    llk_math_eltwise_unary_sfpu_acos<false>(0);
    llk_math_eltwise_unary_sfpu_acos_init<false>();
}

void test_llk_math_matmul() {
    matmul_configure_addrmod<0>(
        false, // transpose 
        8,     // ct_dim 
        8,     // rt_dim 
        16,    // kt_dim 
        0,     // in0_id 
        1);    // in1_id
    matmul_configure_mop<0>(
        false, // transpose 
        8,     // ct_dim 
        8,     // rt_dim 
        16,    // kt_dim 
        0,     // in0_id 
        1);    // in1_id
    llk_math_matmul_init<0>(0, 1);
    llk_math_matmul<0>(0);
}

void test_llk_math_reduce() {
    llk_math_reduce<PoolType::SUM, ReduceDim::REDUCE_ROW>(0);
    reduce_configure_addrmod<PoolType::SUM, false>();
    reduce_configure_mop<ReduceDim::REDUCE_ROW, 0>();
    llk_math_reduce_init<PoolType::SUM, ReduceDim::REDUCE_ROW>();
}

} // namespace syntax

