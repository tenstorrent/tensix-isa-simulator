// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
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

class Pack: public Thread {
public:
    Pack(
        Tensix *tensix, 
        ThreadSync *thread_sync, 
        const ThreadParams &thread_params,
        std::function<void (Pack *)> main);
    ~Pack();
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
    void pack_tile(uint32_t ifrom_dst, uint32_t output_addr);
public:
    void packer_wait_for_math_done();
    void pack_dest_section_done(DstSync Dst, bool is_fp32_dest_acc_en);
public:
    // SKIPPED: Arguments related to 'pack_output' (always 16)
    void pack_hw_configure_disaggregated(
        bool untilize, 
        bool is_fp32_dest_acc_en, 
        ReluType relu_type, 
        uint32_t relu_threshold,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void pack_init(bool untilize, bool zero_output, DstTileFaceLayout FaceLayout);
    // SKIPPED: argument 'out_of_order_output' (handled by caller)
    void pack(
        DstSync Dst, 
        bool untilize, 
        bool is_fp32_dest_acc_en, 
        bool pack_l1_acc_en,
        uint32_t tile_index, 
        uint32_t output_addr,  
        bool pack_l1_acc); 
protected:
    void pack_mop_config(bool untilize, bool zero_output, DstTileFaceLayout FaceLayout);
public:
    void pack_reduce_hw_configure_disaggregated(
        bool untilize, 
        PoolType type, 
        ReduceDim dim, 
        bool is_fp32_dest_acc_en, 
        ReluType relu_type, 
        uint32_t relu_threshold,
        DataFormat out_src_format,
        DataFormat out_dst_format);
    void pack_reduce_config_v2(
        ReduceDim dim, 
        bool at_kernel_start, 
        bool revert, 
        bool is_fp32_dest_acc_en,
        DataFormat out_src_format,
        DataFormat out_dst_format);
protected:
    void pack_dest_init(
        DstSync Dst, 
        DstTileFaceLayout FaceLayout, 
        bool untilize, 
        bool is_fp32_dest_acc_en);
    void init_packer_dest_offset_registers(
        DstSync Dst, 
        DstTileFaceLayout FaceLayout, 
        bool untilize);
    void pack_set_relu_config(uint32_t config);
    void packer_addr_counter_init();
#if 0 // TODO: Revise this
    void configure_pack(
        bool is_fp32_dest_acc_en,
        DataFormat src_format,
        DataFormat dst_format, 
        ReluType relu_type, 
        uint32_t relu_threshold);
#endif
    void configure_pack(
        bool is_fp32_dest_acc_en,
        bool untilize,
        DataFormat src_format,
        DataFormat dst_format, 
        ReluType relu_type, 
        uint32_t relu_threshold);
    void program_packer_destination(PackSelMask PackSel, uint32_t addr);
    void program_packer_l1_acc(bool pack_l1_acc);
protected:
    uint32_t get_pack_sync_tile_dst_ptr() {
        return m_pack_sync_tile_dst_ptr;
    }
    void set_pack_sync_tile_dst_ptr(uint32_t value) {
        m_pack_sync_tile_dst_ptr = value;
    }
    void incr_pack_sync_tile_dst_ptr(uint32_t value, uint32_t wrap) {
        m_pack_sync_tile_dst_ptr = (m_pack_sync_tile_dst_ptr + value) % wrap;
    }
protected:
    static uint32_t PACK_SEL(uint32_t pack_count) {
        return (pack_count == 1) ? 0x1 :
            (pack_count == 2) ? 0x3 :
            (pack_count == 4) ? 0xF : 0x0;
    } 
protected:
    static constexpr uint32_t PACK_CNT = 4;
protected:
    ThreadParams m_thread_params;
    std::function<void (Pack *)> m_main;
    uint32_t m_pack_sync_tile_dst_ptr;
};

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

