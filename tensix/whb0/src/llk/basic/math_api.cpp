// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <functional>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/common.hpp"
#include "llk/basic/math.hpp"

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
//    Math
//

Math::Math(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        const ThreadParams &thread_params,
        std::function<void (Math *)> main):
            Thread(ThreadType::Math, tensix, thread_sync),
            m_thread_params(thread_params),
            m_main(main) { }

Math::~Math() { }

void Math::run() {
    m_main(this);
}

// acquire / release Dest lock

void Math::acquire_dst(DstMode mode) {
    // ACHTUNG: Why 'mode' is not used?
    math_wait_for_dest_available(m_thread_params.SYNC);
}

void Math::release_dst(DstMode mode) {
    // ACHTUNG: Why 'mode' is not used? Why 'is_fp32_dest_acc_en' is always false?
    math_dest_section_done(m_thread_params.SYNC, false);
}

// eltwise_binary

void Math::binary_op_init_common(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_pack_sync_init(m_thread_params.SYNC, false);
}

void Math::binary_op_specific_init(int op_code) {
    // TODO: Represent 'op_code' as enum?
    if (op_code == 0) {
        add_tiles_init_nof();
    } else if (op_code == 1) {
        sub_tiles_init_nof();
    } else if (op_code == 2) {
        mul_tiles_init_f(); 
    }
}

void Math::add_tiles_init() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::NONE, 
        0,
        false);
}

void Math::add_tiles_init_nof() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::NONE, 
        0,
        false);
}

void Math::add_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::NONE, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::sub_tiles_init() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::NONE, 
        0,
        false);
}

void Math::sub_tiles_init_nof() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::NONE, 
        0,
        false);
}

void Math::sub_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::NONE, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::mul_tiles_init() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::NONE, 
        m_thread_params.MATH_FIDELITY,
        false);
}

void Math::mul_tiles_init_f() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::NONE, 
        m_thread_params.MATH_FIDELITY,
        false);
}

void Math::mul_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::NONE, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

// eltwise_unary_datacopy

void Math::copy_tile_init() {
    copy_tile_to_dst_init_short(); 
}

void Math::copy_tile_to_dst_init_short() {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0);
}

void Math::copy_tile(uint32_t in_addr, uint32_t idst) {
    math_eltwise_unary_datacopy(
        DataCopyType::A2D, BroadcastType::NONE, DstSync::SyncHalf, false, idst);
}

// eltwise_unary_sfpu

void Math::init_sfpu(
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

void Math::unary_op_init_common(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0);
    math_pack_sync_init(m_thread_params.SYNC, false); 
}

void Math::gelu_tile_init() {
    math_eltwise_unary_sfpu_gelu_init(m_thread_params.APPROX);
}

void Math::gelu_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_gelu(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::recip_tile_init() {
    math_eltwise_unary_sfpu_reciprocal_init(m_thread_params.APPROX);
}

void Math::recip_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_reciprocal(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::exp_tile_init() {
    math_eltwise_unary_sfpu_exponential_init(m_thread_params.APPROX);
}

void Math::exp_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_exponential(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::sqrt_tile_init() {
    math_eltwise_unary_sfpu_sqrt_init(m_thread_params.APPROX);
}

void Math::sqrt_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_sqrt(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::sigmoid_tile_init() {
    math_eltwise_unary_sfpu_sigmoid_init(m_thread_params.APPROX);
}

void Math::sigmoid_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_sigmoid(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::log_tile_init() {
    math_eltwise_unary_sfpu_log_init(m_thread_params.APPROX);
}

void Math::log_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_log(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

void Math::tanh_tile_init() {
    math_eltwise_unary_sfpu_tanh_init(m_thread_params.APPROX);
}

void Math::tanh_tile(uint32_t idst) {
    math_eltwise_unary_sfpu_tanh(m_thread_params.APPROX, DstSync::SyncHalf, idst);
}

// pack_relu

void Math::pack_relu_tile_to_stream(uint32_t idst, uint32_t addr) {
    // nothing to do
}

void Math::pack_relu_config(uint32_t enable) {
    // nothing to do
}

// eltwise_binary (bcast)

void Math::init_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    if (tBcastOp == EltwiseBinaryType::ELWMUL) {
        math_eltwise_binary_init(tBcastOp, tBcastDim, m_thread_params.MATH_FIDELITY, false);
    } else {
        math_eltwise_binary_init(tBcastOp, tBcastDim, 0, false); 
    }
    math_pack_sync_init(DstSync::SyncHalf, false); 
}

void Math::any_tiles_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    math_eltwise_binary(
        tBcastOp, 
        tBcastDim, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false, 
        false,
        idst,
        false);
}

void Math::add_bcast_rows_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWADD, BroadcastType::ROW, 0, false);
}

void Math::add_bcast_cols_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWADD, BroadcastType::COL, 0, false);
}

void Math::add_bcast_scalar_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWADD, BroadcastType::SCALAR, 0, false);
}

void Math::add_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWADD, tBcastDim, in0_addr, in1_addr, idst);
}

void Math::add_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::ROW, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::add_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::COL, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::add_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::SCALAR, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::sub_bcast_rows_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWSUB, BroadcastType::ROW, 0, false);
}

void Math::sub_bcast_cols_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWSUB, BroadcastType::COL, 0, false);
}

void Math::sub_bcast_scalar_init_short() {
    math_eltwise_binary_init(EltwiseBinaryType::ELWSUB, BroadcastType::SCALAR, 0, false);
}

void Math::sub_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWSUB, tBcastDim, in0_addr, in1_addr, idst);
}

void Math::sub_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::ROW, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::sub_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::COL, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::sub_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWSUB, 
        BroadcastType::SCALAR, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::mul_bcast_rows_init_short() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::ROW, 
        m_thread_params.MATH_FIDELITY, 
        false);
}

void Math::mul_bcast_cols_init_short() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::COL, 
        m_thread_params.MATH_FIDELITY, 
        false);
}

void Math::mul_bcast_scalar_init_short() {
    math_eltwise_binary_init(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::SCALAR, 
        m_thread_params.MATH_FIDELITY, 
        false);
}

void Math::mul_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWMUL, tBcastDim, in0_addr, in1_addr, idst);
}

void Math::mul_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::ROW, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::mul_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::COL, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

void Math::mul_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    math_eltwise_binary(
        EltwiseBinaryType::ELWMUL, 
        BroadcastType::SCALAR, 
        DstSync::SyncHalf, 
        m_thread_params.MATH_FIDELITY, 
        false,
        false,
        idst,
        false);
}

// matmul

void Math::mm_init(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_matmul_init(m_thread_params.MATH_FIDELITY, 0);
    math_pack_sync_init(m_thread_params.SYNC, false); 
}

void Math::mm_init_short() {
    math_matmul_init(m_thread_params.MATH_FIDELITY, 0);
}

void Math::mm_init_once() { }

void Math::matmul_tiles(
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst,
        bool transpose) {
    math_matmul(m_thread_params.MATH_FIDELITY, idst);
}

// reduce

void Math::reduce_init(
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        float scaler) {
    math_reduce_init(reduce_op, dim, m_thread_params.MATH_FIDELITY);
    math_pack_sync_init(DstSync::SyncFull, false);
}

void Math::reduce_init_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_reduce_init(reduce_op, dim, m_thread_params.MATH_FIDELITY);
    math_pack_sync_init(m_thread_params.SYNC, false);
}

void Math::reduce_init_delta_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_reduce_init(reduce_op, dim, m_thread_params.MATH_FIDELITY);
}

void Math::reduce_revert_delta_v2(
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // nothing to do
}

void Math::reduce_tile(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t addr,
        uint32_t idst, 
        float scaler) {
    math_reduce(reduce_op, dim, m_thread_params.MATH_FIDELITY, false, idst);
}

void Math::reduce_tile_v2(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    math_reduce(reduce_op, dim, m_thread_params.MATH_FIDELITY, false, idst); 
}

// transpose_wh

void Math::transpose_wh_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, true, 0);
    math_pack_sync_init(DstSync::SyncHalf, false); 
}

void Math::transpose_wh_tile(uint32_t addr, uint32_t idst) {
    math_eltwise_unary_datacopy(
        DataCopyType::A2D, 
        BroadcastType::NONE, 
        DstSync::SyncHalf, 
        false, 
        idst); 
}

// tilize

void Math::tilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0);
    math_pack_sync_init(DstSync::SyncHalf, false); 
}

void Math::tilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0); 
}

void Math::tilize_block(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    for (uint32_t t = 0; t < block; t++) {
        // Acquire dst
        math_wait_for_dest_available(m_thread_params.SYNC);
        // Datacopy
        math_eltwise_unary_datacopy(
            DataCopyType::A2D, 
            BroadcastType::NONE, 
            DstSync::SyncHalf, 
            false, 
            0);
        // Release dest
        math_dest_section_done(m_thread_params.SYNC, false);
    } 
}

void Math::tilize_uninit() {
    // nothing to do
}

// untilize

void Math::untilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0);
    math_pack_sync_init(DstSync::SyncHalf, false);
}

void Math::untilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    math_eltwise_unary_datacopy_init(DataCopyType::A2D, BroadcastType::NONE, false, 0);
}

void Math::untilize_block(
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    for (uint32_t t = 0; t < block; t++) {
        // Acquire dst
        math_wait_for_dest_available(m_thread_params.SYNC);
        // Datacopy
        math_eltwise_unary_datacopy(
            DataCopyType::A2D, 
            BroadcastType::NONE, 
            DstSync::SyncHalf, 
            false,
            0);
        // Release dest
        math_dest_section_done(m_thread_params.SYNC, false);
    } 
}

void Math::untilize_uninit(DataFormat in_src_format, DataFormat in_dst_format) {
    // nothing to do
}

// pack

void Math::pack_tile(uint32_t ifrom_dst, uint32_t addr) {
    // nothing to do
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

