// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

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

class Unpack: public Thread {
public:
    Unpack(
        Tensix *tensix, 
        ThreadSync *thread_sync, 
        const ThreadParams &thread_params,
        std::function<void (Unpack *)> main);
    ~Unpack();
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
    void unpack_A_hw_configure_disaggregated(
        BroadcastType BType, 
        bool transpose_xy, 
        bool transpose_xy_srca, 
        bool acc_to_dest, 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format);
    void unpack_A_init(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest,
        uint32_t within_face_16x16_transpose);
    void unpack_A(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest,
        uint32_t address);
public:
    void unpack_AB_hw_configure_disaggregated(
        BroadcastType BType,
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format);
    void unpack_AB_init(BroadcastType BType);
    void unpack_AB(
        BroadcastType BType,
        uint32_t address_a,
        uint32_t address_b);
private:
    void unpack_A_mop_config(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest); 
private:
    void unpack_AB_mop_config(BroadcastType BType);
public:
    void unpack_AB_matmul_hw_configure_disaggregated( 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format,
        uint32_t transpose_xy_srca);
    void unpack_AB_matmul_init(uint32_t transpose);
    void unpack_AB_matmul(uint32_t address_a, uint32_t address_b);
private:
    void unpack_AB_matmul_mop_config(bool transpose);
public:
    void unpack_reduce_hw_configure_disaggregated(
        PoolType type, 
        ReduceDim dim, 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        float mult);
    void unpack_reduce_init(PoolType type, ReduceDim dim);
    void unpack_reduce(PoolType type, ReduceDim dim, uint32_t addr); 
private:
    void unpack_reduce_mop_config(PoolType type, ReduceDim dim);
public:
    void unpack_tilize_hw_configure_disaggregated(
        DataFormat unpa_src_format, DataFormat unpa_dst_format);
    void unpack_tilize_init(
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        uint32_t unpA_block_c_tiles);
    void unpack_tilize_block(
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        uint32_t base_addr, 
        uint32_t block_c_tiles);
    void unpack_tilize_uninit();
private:
    void unpack_tilize_mop_config();
public:
    void unpack_untilize_hw_configure_disaggregated(
        DataFormat unpa_src_format, DataFormat unpa_dst_format);
    void unpack_untilize_init(DataFormat unpa_src_format, DataFormat unpa_dst_format);
    void unpack_untilize(uint32_t base_addr, uint32_t block_c_tiles);
    void unpack_untilize_uninit(DataFormat unpa_src_format, DataFormat unpa_dst_format);
private:
    void unpack_untilize_mop_config();
    void unpack_untilize_pass(
        bool first_pass, 
        uint32_t base_addr, 
        uint32_t block_c_tiles);
protected:
    void configure_unpack_AB(
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format,
        uint32_t srca_face_height,
        uint32_t srcb_face_height,
        bool row_pool,
        bool transpose_xy_srca_en,
        bool is_fp32_dest_acc_en);
    void start_unpack_A(BroadcastType BType, bool acc_to_dest, uint32_t address);
    void end_unpack_A();
    void start_unpack_AB(uint32_t address_a, uint32_t address_b);
    void end_unpack_AB();
    void start_unpack_A_untilize(uint32_t address);
    void end_unpack_A_untilize();
    void mop_run(uint32_t count);
protected:
    ThreadParams m_thread_params;
    std::function<void (Unpack *)> m_main;
    uint32_t m_l1_buffer_addr;
    uint32_t m_tile_size; // temporary: used by unpack_untilize
};

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

