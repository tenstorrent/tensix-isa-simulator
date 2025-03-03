// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <functional>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/common.hpp"
#include "llk/basic/pack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::DataFormat;
using core::Tensix;
using core::ThreadType;
using core::ThreadSync;
using core::Thread;

//
//    Pack
//

Pack::Pack(
        Tensix *tensix, 
        ThreadSync *thread_sync, 
        const ThreadParams &thread_params,
        std::function<void (Pack *)> main):
            Thread(ThreadType::Pack, tensix, thread_sync),
            m_thread_params(thread_params),
            m_main(main),
            m_pack_sync_tile_dst_ptr(0) { }

Pack::~Pack() { }

void Pack::run() {
    m_main(this);
}

// acquire / release Dest lock

void Pack::acquire_dst(DstMode mode) {
    // ACHTUNG: Why 'mode' is not used?
    packer_wait_for_math_done();
}

void Pack::release_dst(DstMode mode) {
    // ACHTUNG: Why 'mode' is not used? Why 'is_fp32_dest_acc_en' is always false?
    pack_dest_section_done(m_thread_params.SYNC, false);
}

// eltwise_binary

void Pack::binary_op_init_common(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0,
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(m_thread_params.SYNC, DstTileFaceLayout::RowMajor, false, false);
}

void Pack::binary_op_specific_init(int op_code) {
    // TODO: Represent 'op_code' as enum?
    if (op_code == 0) {
        add_tiles_init_nof();
    } else if (op_code == 1) {
        sub_tiles_init_nof();
    } else if (op_code == 2) {
        mul_tiles_init_f(); 
    }
}

void Pack::add_tiles_init() {
    init_packer_dest_offset_registers(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false);
}

void Pack::add_tiles_init_nof() {
    // nothing to do
}

void Pack::add_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::sub_tiles_init() {
    init_packer_dest_offset_registers(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false);
}

void Pack::sub_tiles_init_nof() {
    // nothing to do
}

void Pack::sub_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::mul_tiles_init() {
    init_packer_dest_offset_registers(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false);
}

void Pack::mul_tiles_init_f() {
    // nothing to do
}

void Pack::mul_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

// eltwise_unary_datacopy

void Pack::copy_tile_init() {
    copy_tile_to_dst_init_short(); 
    init_packer_dest_offset_registers(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false);
}

void Pack::copy_tile_to_dst_init_short() {
    // nothing to do
}

void Pack::copy_tile(uint32_t in_addr, uint32_t idst) {
    // nothing to do
}

// eltwise_unary_sfpu

void Pack::init_sfpu(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    unary_op_init_common(
        in_src_format, 
        in_dst_format,
        out_src_format,
        out_dst_format);
}

void Pack::unary_op_init_common(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0, 
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(m_thread_params.SYNC, DstTileFaceLayout::RowMajor, false, false); 
}

void Pack::gelu_tile_init() {
    // nothing to do
}

void Pack::gelu_tile(uint32_t idst) {
    // nothing to do
}

void Pack::recip_tile_init() {
    // nothing to do
}

void Pack::recip_tile(uint32_t idst) {
    // nothing to do
}

void Pack::exp_tile_init() {
    // nothing to do
}

void Pack::exp_tile(uint32_t idst) {
    // nothing to do
}

void Pack::sqrt_tile_init() {
    // nothing to do
}

void Pack::sqrt_tile(uint32_t idst) {
    // nothing to do
}

void Pack::sigmoid_tile_init() {
    // nothing to do
}

void Pack::sigmoid_tile(uint32_t idst) {
    // nothing to do
}

void Pack::log_tile_init() {
    // nothing to do
}

void Pack::log_tile(uint32_t idst) {
    // nothing to do
}

void Pack::tanh_tile_init() {
    // nothing to do
}

void Pack::tanh_tile(uint32_t idst) {
    // nothing to do
}

// pack_relu

void Pack::pack_relu_tile_to_stream(uint32_t idst, uint32_t addr) {
    pack(m_thread_params.SYNC, false, false, false, idst, addr, false);
}

void Pack::pack_relu_config(uint32_t enable) {
    pack_set_relu_config(enable);    
}

// eltwise_binary (bcast)

void Pack::init_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0,
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false, false);
}

void Pack::any_tiles_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    // nothing to do
}

void Pack::add_bcast_rows_init_short() {
    // nothing to do
}

void Pack::add_bcast_cols_init_short() {
    // nothing to do
}

void Pack::add_bcast_scalar_init_short() {
    // nothing to do
}

void Pack::add_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWADD, tBcastDim, in0_addr, in1_addr, idst);
}

void Pack::add_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::add_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::add_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::sub_bcast_rows_init_short() {
    // nothing to do
}

void Pack::sub_bcast_cols_init_short() {
    // nothing to do
}

void Pack::sub_bcast_scalar_init_short() {
    // nothing to do
}

void Pack::sub_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWSUB, tBcastDim, in0_addr, in1_addr, idst);
}

void Pack::sub_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::sub_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::sub_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::mul_bcast_rows_init_short() {
    // nothing to do
}

void Pack::mul_bcast_cols_init_short() {
    // nothing to do
}

void Pack::mul_bcast_scalar_init_short() {
    // nothing to do
}

void Pack::mul_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWMUL, tBcastDim, in0_addr, in1_addr, idst);
}

void Pack::mul_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::mul_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

void Pack::mul_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    // nothing to do
}

// matmul

void Pack::mm_init(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0,
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(m_thread_params.SYNC, DstTileFaceLayout::RowMajor, false, false);
#if 0 // SKIPPED
    // TODO(AP): ZM-only kernel
    // ACHTUNG: Similar function is already called from 'pack_dest_output'.
    //     Why one more call is needed? [-A.G.]
    init_packer_dest_offset_registers(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false);
#endif
}

void Pack::mm_init_short() {
    // nothing to do
}

void Pack::mm_init_once() {
    // nothing to do
}

void Pack::matmul_tiles(
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst,
        bool transpose) {
    // nothing to do
}

// reduce

void Pack::reduce_init(
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        float scaler) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_reduce_hw_configure_disaggregated(
        false, 
        reduce_op, 
        dim, 
        false, 
        ReluType::NO_RELU, 
        0,
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(DstSync::SyncFull, DstTileFaceLayout::RowMajor, false, false);
}

void Pack::reduce_init_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_reduce_config_v2(dim, at_start, false, false, out_src_format, out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(m_thread_params.SYNC, DstTileFaceLayout::RowMajor, false, false);
}

void Pack::reduce_init_delta_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_reduce_config_v2(dim, at_start, false, false, out_src_format, out_dst_format);
}

void Pack::reduce_revert_delta_v2(
        ReduceDim dim,
        DataFormat out_src_format, 
        DataFormat out_dst_format) {
    pack_reduce_config_v2(dim, false, true, false, out_src_format, out_dst_format);
}

void Pack::reduce_tile(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t addr,
        uint32_t idst, 
        float scaler) {
    // nothing to do
}

void Pack::reduce_tile_v2(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    // nothing to do
}

// transpose_wh

void Pack::transpose_wh_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0,
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false, false); 
}

void Pack::transpose_wh_tile(uint32_t addr, uint32_t idst) {
    // nothing to do
}

// tilize

void Pack::tilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0, 
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false, false); 
}

void Pack::tilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    // nothing to do
}

void Pack::tilize_block(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    uint32_t tile_size = get_l1_tile_size(out_dst_format);
    for (uint32_t t = 0; t < block; t++) {
        // Acquire dst
        packer_wait_for_math_done();
        // Datacopy
        pack(m_thread_params.SYNC, false, false, false, 0, out_addr, false);
        out_addr += tile_size;
        // Release dest
        pack_dest_section_done(m_thread_params.SYNC, false);
    } 
}

void Pack::tilize_uninit() {
    // nothing to do
}

// untilize

void Pack::untilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    pack_init(false, false, DstTileFaceLayout::RowMajor);
    pack_hw_configure_disaggregated(
        false, 
        false, 
        ReluType::NO_RELU, 
        0, 
        out_src_format, 
        out_dst_format);
    // SKIPPED: setup_outputs
    pack_dest_init(DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false, false);
}

void Pack::untilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // nothing to do
}

void Pack::untilize_block(
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    uint32_t tile_size = get_l1_tile_size(out_dst_format);
    for (uint32_t t = 0; t < block; t++) {
        // Acquire dst
        packer_wait_for_math_done();
        // Datacopy
        pack(m_thread_params.SYNC, false, false, false, 0, out_addr, false);
        out_addr += tile_size;
        // Release dest
        pack_dest_section_done(m_thread_params.SYNC, false);
    } 
}

void Pack::untilize_uninit(DataFormat in_src_format, DataFormat in_dst_format) {
    // nothing to do
}

// pack

void Pack::pack_tile(uint32_t ifrom_dst, uint32_t output_addr) {
    pack(
        m_thread_params.SYNC, 
        false, 
        false, 
        false, 
        ifrom_dst, 
        output_addr, 
        false);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

