#pragma once

#include <cstdint>
#include <functional>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/common.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::DataFormat;
using core::Tensix;
using core::ThreadSync;
using core::Thread;

class Math: public Thread {
public:
    Math(
        Tensix *tensix, 
        ThreadSync *thread_sync, 
        const ThreadParams &thread_params,
        std::function<void (Math *)> main);
    ~Math();
public:
    void run() override;
public:
    void acquire_dst(DstMode mode);
    void release_dst(DstMode mode);
public:
    void binary_op_init_common(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void binary_op_specific_init(int op_code);
    void add_tiles_init();
    void add_tiles_init_nof();
    void add_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void sub_tiles_init();
    void sub_tiles_init_nof();
    void sub_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void mul_tiles_init();
    void mul_tiles_init_f();
    void mul_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
public:
    void copy_tile_init();
    void copy_tile_to_dst_init_short();
    void copy_tile(uint32_t in_addr, uint32_t idst);
public:
    void init_sfpu(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void unary_op_init_common(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void gelu_tile_init();
    void gelu_tile(uint32_t idst);
    void recip_tile_init();
    void recip_tile(uint32_t idst);
    void exp_tile_init();
    void exp_tile(uint32_t idst);
    void sqrt_tile_init();
    void sqrt_tile(uint32_t idst);
    void sigmoid_tile_init();
    void sigmoid_tile(uint32_t idst);
    void log_tile_init();
    void log_tile(uint32_t idst);
    void tanh_tile_init();
    void tanh_tile(uint32_t idst);
public:
    void pack_relu_tile_to_stream(uint32_t idst, uint32_t addr);
    void pack_relu_config(uint32_t enable);
public:
    void init_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void any_tiles_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst);
    void add_bcast_rows_init_short();
    void add_bcast_cols_init_short();
    void add_bcast_scalar_init_short();
    void add_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst);
    void add_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void add_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void add_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void sub_bcast_rows_init_short();
    void sub_bcast_cols_init_short();
    void sub_bcast_scalar_init_short();
    void sub_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst);
    void sub_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void sub_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void sub_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void mul_bcast_rows_init_short();
    void mul_bcast_cols_init_short();
    void mul_bcast_scalar_init_short();
    void mul_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst);
    void mul_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void mul_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
    void mul_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst);
public:
    void mm_init(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void mm_init_short();
    void mm_init_once();
    void matmul_tiles(
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst,
        bool transpose);
public:
    void reduce_init(
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        float scaler);
    void reduce_init_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void reduce_init_delta_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void reduce_revert_delta_v2(
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void reduce_tile(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t addr,
        uint32_t idst, 
        float scaler);
    void reduce_tile_v2(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst);
public:
    void transpose_wh_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void transpose_wh_tile(uint32_t addr, uint32_t idst);
public:
    void tilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block);
    void tilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block);
    void tilize_block(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr); 
    void tilize_uninit();
public:
    void untilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void untilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void untilize_block(
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr); 
    void untilize_uninit(DataFormat in_src_format, DataFormat in_dst_format);
public:
    void pack_tile(uint32_t ifrom_dst, uint32_t addr);
public:
    void math_wait_for_dest_available(DstSync Dst);
    void math_dest_section_done(DstSync Dst, bool is_fp32_dest_acc_en);
public:
    void math_eltwise_binary_init(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType src_b_bcast_type, 
        uint32_t NUM_FIDELITY_PHASES, 
        bool acc_to_dest);
    void math_eltwise_binary(
        EltwiseBinaryType eltwise_binary_type,
        BroadcastType src_b_bcast_type,
        DstSync Dst,
        int NUM_FIDELITY_PHASES,
        bool acc_to_dest,
        bool is_fp32_dest_acc_en,
        uint32_t dst_index, 
        bool clear_dest_acc);
private:
    void math_eltwise_binary_configure_addrmod(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType bcast_type);
    void math_eltwise_binary_configure_mop(
        EltwiseBinaryType eltwise_binary_type, 
        BroadcastType bcast_type, 
        uint32_t NUM_FIDELITY_PHASES, 
        bool acc_to_dest);
public:
    // within_face_16x16_transpose is used by unpacker, math does not transpose
    void math_eltwise_unary_datacopy_init(
        DataCopyType type, 
        BroadcastType src_b_bcast_type, 
        bool transpose_xy,
        uint32_t within_face_16x16_transpose); 
    void math_eltwise_unary_datacopy(
        DataCopyType type, 
        BroadcastType src_b_bcast_type, 
        DstSync Dst, 
        bool is_fp32_dest_acc_en,
        uint32_t dst_index);
private:
    void eltwise_unary_configure_addrmod(DataCopyType type, BroadcastType bcast_type);
    void eltwise_unary_configure_mop(
        DataCopyType type, 
        BroadcastType bcast_type, 
        bool transpose_xy,
        uint32_t rows_per_inst, 
        uint32_t total_rows);
public:
    void math_eltwise_unary_sfpu_exponential_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_sqrt_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_gelu_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_gelu_derivative_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_log_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_reciprocal_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_tanh_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_dropout_init(uint32_t seed);
    void math_eltwise_unary_sfpu_sigmoid_init(bool APPROXIMATE);
    void math_eltwise_unary_sfpu_exponential(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_sqrt(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_gelu(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_gelu_derivative(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_reciprocal(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_log(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_tanh(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
    void math_eltwise_unary_sfpu_dropout(
        DstSync dst_sync, 
        uint32_t dst_index, 
        int integer_dropout, 
        int scale_factor);
    void math_eltwise_unary_sfpu_sigmoid(
        bool APPROXIMATE, DstSync dst_sync, uint32_t dst_index);
private:
    void math_eltwise_unary_sfpu_init(
        SfpuType sfpu_op, 
        bool APPROXIMATE,
        uint32_t param0 = 0, 
        uint32_t param1 = 0, 
        uint32_t param2 = 0, 
        uint32_t param3 = 0, 
        uint32_t param4 = 0, 
        uint32_t param5 = 0);
    void math_eltwise_unary_sfpu(
        SfpuType sfpu_op, 
        bool APPROXIMATE, 
        DstSync Dst,
        uint32_t dst_index,
        uint32_t param0 = 0,
        uint32_t param1 = 0,
        uint32_t param2 = 0,
        uint32_t param3 = 0,
        uint32_t param4 = 0,
        uint32_t param5 = 0);
    void eltwise_unary_sfpu_configure_addrmod();
public:
    void math_matmul_init(uint32_t NUM_FIDELITY_PHASES, uint32_t transpose);
    void math_matmul(uint32_t NUM_FIDELITY_PHASES, uint32_t dst_index);
private:
    void math_matmul_configure_addrmod(uint32_t NUM_FIDELITY_PHASES);
    void math_matmul_configure_mop(uint32_t NUM_FIDELITY_PHASES, bool transpose);
public:
    void math_reduce_init(PoolType type, ReduceDim dim, int num_fidelity_phases);
    void math_reduce(
        PoolType type, 
        ReduceDim dim, 
        int num_fidelity_phases, 
        bool is_fp32_dest_acc_en,
        uint32_t dst_index);
private:
    void reduce_configure_addrmod(PoolType type, bool is_high_fidelity);
    void reduce_configure_mop(ReduceDim dim, int num_fidelity_phases);
private:
    // SFPU initialization
    void sfpu_init(bool APPROXIMATION_MODE, SfpuType operation, uint32_t param0);
    void configure_programmable_constants();
    void init_dropout_seed(uint16_t p2);
    // SFPU calculation
    void calculate_sfpu(
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
        uint32_t param5);
    void calculate_exponential(
        bool APPROXIMATION_MODE, 
        bool ZERO_NEGATIVE, 
        bool SCALE_EN, 
        uint32_t ADDR_MOD,
        uint16_t exp_base_scale_factor);
    void calculate_exponential_body(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_gelu_core(bool APPROXIMATION_MODE);
    void calculate_gelu(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_sigmoid(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_tanh(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_hardtanh(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD,
        uint32_t param0, 
        uint32_t param1, 
        uint32_t param2);
    void calculate_tanh_derivative(
        bool APPROXIMATION_MODE, 
        int WITH_PRECOMPUTED_TANH, 
        uint32_t ADDR_MOD);
    void calculate_gelu_derivative(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_reciprocal(bool APPROXIMATION_MODE, int ITERATIONS, uint32_t ADDR_MOD);
    void calculate_sqrt(
        bool APPROXIMATION_MODE, 
        int ITERATIONS, 
        int RECIPROCAL_ITERATIONS, 
        uint32_t ADDR_MOD);
    void calculate_dropout(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t prob, 
        uint32_t scale);
    void calculate_lrelu(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t slope);
    void calculate_power(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t exponent);
    void calculate_multiply(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_log_body(
        bool HAS_BASE_SCALING, 
        uint32_t ADDR_MOD, 
        uint32_t log_base_scale_factor);
    void calculate_log(
        bool APPROXIMATION_MODE, 
        bool HAS_BASE_SCALING, 
        uint32_t ADDR_MOD, 
        uint32_t log_base_scale_factor);
    void calculate_comp(
        bool APPROXIMATION_MODE, 
        SfpuType COMP_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t exponent_size_8);
    void calculate_clamp(
        bool APPROXIMATION_MODE, 
        uint32_t ADDR_MOD, 
        uint32_t param0, 
        uint32_t param1, 
        uint32_t param2);
    void calculate_abs(bool APPROXIMATION_MODE, uint32_t ADDR_MOD);
    void calculate_sign(bool APPROXIMATION_MODE, uint32_t ADDR_MOD, uint32_t exponent_size_8);
    void sfpu_exp();
    void sfpu_reciprocal(int max_iter, uint32_t ADDR_MOD);
    void sfpu_reciprocal_lreg_reduced(uint32_t ADDR_MOD, uint32_t max_iter);
    // SFPU helper functions
    void sfpu_push_cc();
    void sfpu_pop_cc();
    void sfpu_comp_cc();
    void sfpu_toggle_enable_cc();
    void sfpu_enable_cc();
    void sfpu_disable_cc();
    void sfpu_flip_cc_flag();
    void sfpu_set_cc_flag();
    void sfpu_set_cc_from_reg0_sign();
    void sfpu_set_cc_from_reg1_sign();
    void sfpu_set_cc_from_reg2_sign();
    void sfpu_set_cc_from_reg3_sign();
    void sfpu_set_cc_from_reg3_if_zero();
    void sfpu_set_cc_from_reg2_if_zero();
    void sfpu_set_cc_if_sign_neg(uint32_t reg);
    void sfpu_set_cc_if_sign_pos(uint32_t reg);
    void sfpu_set_cc_if_zero(uint32_t reg);
    void sfpu_set_cc_if_not_zero(uint32_t reg);
    void sfpu_load_imm32(
        const uint32_t dest, 
        const uint32_t upper16, 
        const uint32_t lower16);
    void sfpu_load_imm32(const uint32_t dest, const uint32_t val);
#if 0 // TODO: Revise this
    void sfpu_access_even_cols();
    void sfpu_access_odd_cols();
#endif
protected:
    void reset_counters(uint32_t setrwc);
    void set_dst_write_addr(
        DstTileLayout layout, 
        DstTileShape tile_shape,
        uint32_t tile_index); 
    void clear_dst_reg_addr();
protected:
    uint32_t get_dest_buffer_base();
    uint32_t get_math_sync_tile_dst_index();
protected:
    void math_pack_sync_init(DstSync Dst, bool is_fp32_dest_acc_en);
protected:
    ThreadParams m_thread_params;
    std::function<void (Math *)> m_main;
};

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

