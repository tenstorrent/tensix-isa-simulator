
#include <cstdint>
#include <functional>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "llk/basic/common.hpp"
#include "llk/basic/unpack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::DataFormat;
using core::Tensix;
using core::ThreadType;
using core::Thread;

//
//    Unpack
//

Unpack::Unpack(
        Tensix *tensix, 
        ThreadSync *thread_sync, 
        const ThreadParams &thread_params,
        std::function<void (Unpack *)> main):
            Thread(ThreadType::Unpack, tensix, thread_sync),
            m_thread_params(thread_params),
            m_main(main),
            m_l1_buffer_addr(0),
            m_tile_size(0) { 
    m_l1_buffer_addr = m_local_base;            
}

Unpack::~Unpack() { }

void Unpack::run() {
    m_main(this);
}

// acquire / release Dest lock

void Unpack::acquire_dst(DstMode mode) {
    // nothing to do
}

void Unpack::release_dst(DstMode mode) {
    // nothing to do
}

// eltwise_binary

void Unpack::binary_op_init_common(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_AB_init(BroadcastType::NONE);
    unpack_AB_hw_configure_disaggregated(
        BroadcastType::NONE,
        false,
        in0_src_format,
        in0_dst_format,
        in1_src_format,
        in1_dst_format);
}

void Unpack::binary_op_specific_init(int op_code) {
    // TODO: Represent 'op_code' as enum?
    if (op_code == 0) {
        add_tiles_init_nof();
    } else if (op_code == 1) {
        sub_tiles_init_nof();
    } else if (op_code == 2) {
        mul_tiles_init_f(); 
    }
}

void Unpack::add_tiles_init() {
    unpack_AB_init(BroadcastType::NONE);
}

void Unpack::add_tiles_init_nof() {
    // nothing to do
}

void Unpack::add_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::NONE, in0_addr, in1_addr);
}

void Unpack::sub_tiles_init() {
    unpack_AB_init(BroadcastType::NONE);
}

void Unpack::sub_tiles_init_nof() {
    // nothing to do
}

void Unpack::sub_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::NONE, in0_addr, in1_addr);
}

void Unpack::mul_tiles_init() {
    unpack_AB_init(BroadcastType::NONE);
}

void Unpack::mul_tiles_init_f() {
    // nothing to do
}

void Unpack::mul_tiles(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::NONE, in0_addr, in1_addr);
}

// eltwise_unary_datacopy

void Unpack::copy_tile_init() {
    copy_tile_to_dst_init_short(); 
}

void Unpack::copy_tile_to_dst_init_short() {
    unpack_A_init(BroadcastType::NONE, false, false, 0);
}

void Unpack::copy_tile(uint32_t in_addr, uint32_t idst) {
    unpack_A(BroadcastType::NONE, false, false, in_addr);
}

// eltwise_unary_sfpu

void Unpack::init_sfpu(
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

void Unpack::unary_op_init_common(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_A_init(BroadcastType::NONE, false, false, 0);
    unpack_A_hw_configure_disaggregated(
        BroadcastType::NONE, 
        false,
        false,
        false,
        false,
        in_src_format, 
        in_dst_format);
}

void Unpack::gelu_tile_init() {
    // nothing to do
}

void Unpack::gelu_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::recip_tile_init() {
    // nothing to do
}

void Unpack::recip_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::exp_tile_init() {
    // nothing to do
}

void Unpack::exp_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::sqrt_tile_init() {
    // nothing to do
}

void Unpack::sqrt_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::sigmoid_tile_init() {
    // nothing to do
}

void Unpack::sigmoid_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::log_tile_init() {
    // nothing to do
}

void Unpack::log_tile(uint32_t idst) {
    // nothing to do
}

void Unpack::tanh_tile_init() {
    // nothing to do
}

void Unpack::tanh_tile(uint32_t idst) {
    // nothing to do
}

// pack_relu

void Unpack::pack_relu_tile_to_stream(uint32_t idst, uint32_t addr) {
    // nothing to do
}

void Unpack::pack_relu_config(uint32_t enable) {
    // nothing to do
}

// eltwise_binary (bcast)

void Unpack::init_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_AB_init(tBcastDim);
    unpack_AB_hw_configure_disaggregated(
        tBcastDim,
        false,
        in0_src_format,
        in0_dst_format,
        in1_src_format,
        in1_dst_format);
}

void Unpack::any_tiles_bcast(
        EltwiseBinaryType tBcastOp, 
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    unpack_AB(tBcastDim, in0_addr, in1_addr);
}

void Unpack::add_bcast_rows_init_short() {
    unpack_AB_init(BroadcastType::ROW);
}

void Unpack::add_bcast_cols_init_short() {
    unpack_AB_init(BroadcastType::COL);
}

void Unpack::add_bcast_scalar_init_short() {
    unpack_AB_init(BroadcastType::SCALAR);
}

void Unpack::add_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWADD, tBcastDim, in0_addr, in1_addr, idst);
}

void Unpack::add_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::ROW, in0_addr, in1_addr);
}

void Unpack::add_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::COL, in0_addr, in1_addr);
}

void Unpack::add_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::SCALAR, in0_addr, in1_addr);
}

void Unpack::sub_bcast_rows_init_short() {
    unpack_AB_init(BroadcastType::ROW);
}

void Unpack::sub_bcast_cols_init_short() {
    unpack_AB_init(BroadcastType::COL);
}

void Unpack::sub_bcast_scalar_init_short() {
    unpack_AB_init(BroadcastType::SCALAR);
}

void Unpack::sub_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWSUB, tBcastDim, in0_addr, in1_addr, idst);
}

void Unpack::sub_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::ROW, in0_addr, in1_addr);
}

void Unpack::sub_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::COL, in0_addr, in1_addr);
}

void Unpack::sub_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::SCALAR, in0_addr, in1_addr);
}

void Unpack::mul_bcast_rows_init_short() {
    unpack_AB_init(BroadcastType::ROW);
}

void Unpack::mul_bcast_cols_init_short() {
    unpack_AB_init(BroadcastType::COL);
}

void Unpack::mul_bcast_scalar_init_short() {
    unpack_AB_init(BroadcastType::SCALAR);
}

void Unpack::mul_tiles_bcast(
        BroadcastType tBcastDim,
        uint32_t in0_addr, 
        uint32_t in1_addr, 
        uint32_t idst) {
    any_tiles_bcast(EltwiseBinaryType::ELWMUL, tBcastDim, in0_addr, in1_addr, idst);
}

void Unpack::mul_tiles_bcast_rows(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::ROW, in0_addr, in1_addr);
}

void Unpack::mul_tiles_bcast_cols(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::COL, in0_addr, in1_addr);
}

void Unpack::mul_tiles_bcast_scalar(uint32_t in0_addr, uint32_t in1_addr, uint32_t idst) {
    unpack_AB(BroadcastType::SCALAR, in0_addr, in1_addr);
}

// matmul

void Unpack::mm_init(
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_AB_matmul_init(0);
    // note inverted order of in0/in1 formats (in1 will be unpacked to SrcA)
    unpack_AB_matmul_hw_configure_disaggregated(
        false,
        in1_src_format,
        in1_dst_format,
        in0_src_format,
        in0_dst_format,
        0); 
}

void Unpack::mm_init_short() {
    unpack_AB_matmul_init(0);
}

void Unpack::mm_init_once() { }

void Unpack::matmul_tiles(
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst,
        bool transpose) {
    unpack_AB_matmul(in0_addr, in1_addr);
}

// reduce

void Unpack::reduce_init(
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        float scaler) {
    // SKIPPED: setup_operands
    unpack_reduce_init(reduce_op, dim);
    unpack_reduce_hw_configure_disaggregated(
        reduce_op, 
        dim, 
        false,
        in_src_format, 
        out_src_format, 
        scaler);
}

void Unpack::reduce_init_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim, 
        DataFormat in0_src_format,
        DataFormat in0_dst_format,
        DataFormat in1_src_format,
        DataFormat in1_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_AB_init(BroadcastType::NONE);
    unpack_AB_hw_configure_disaggregated(
        BroadcastType::NONE,
        false,
        in0_src_format,
        in0_dst_format,
        in1_src_format,
        in1_dst_format);
}

void Unpack::reduce_init_delta_v2(
        bool at_start, 
        PoolType reduce_op, 
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    unpack_AB_init(BroadcastType::NONE); 
}

void Unpack::reduce_revert_delta_v2(
        ReduceDim dim,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // nothing to do
}

void Unpack::reduce_tile(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t addr,
        uint32_t idst, 
        float scaler) {
    unpack_reduce(reduce_op, dim, addr); 
}

void Unpack::reduce_tile_v2(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t in0_addr, 
        uint32_t in1_addr,
        uint32_t idst) {
    unpack_AB(BroadcastType::NONE, in0_addr, in1_addr);
}

// transpose_wh

void Unpack::transpose_wh_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_A_init(BroadcastType::NONE, true, false, 0);
    unpack_A_hw_configure_disaggregated(
        BroadcastType::NONE, 
        true, 
        true, 
        false, 
        false,
        in_src_format, 
        in_dst_format);
}

void Unpack::transpose_wh_tile(uint32_t addr, uint32_t idst) {
    unpack_A(BroadcastType::NONE, true, false, addr); 
}

// tilize

void Unpack::tilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    // SKIPPED: setup_operands
    unpack_tilize_hw_configure_disaggregated(in_src_format, in_dst_format);
    unpack_tilize_init(in_src_format, in_dst_format, block); 
}

void Unpack::tilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t block) {
    unpack_tilize_init(in_src_format, in_dst_format, block); 
}

void Unpack::tilize_block(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    unpack_tilize_block(in_src_format, in_dst_format, in_addr, block); 
}

void Unpack::tilize_uninit() {
    unpack_tilize_uninit();
}

// untilize

void Unpack::untilize_init(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // SKIPPED: setup_operands
    unpack_untilize_hw_configure_disaggregated(in_src_format, in_dst_format);
    // init must be after configure 
    unpack_untilize_init(in_src_format, in_dst_format);
}

void Unpack::untilize_init_short(
        DataFormat in_src_format, 
        DataFormat in_dst_format,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    unpack_untilize_init(in_src_format, in_dst_format);
}

void Unpack::untilize_block(
        uint32_t in_addr, 
        uint32_t block, 
        DataFormat out_src_format,
        DataFormat out_dst_format,
        uint32_t out_addr) {
    unpack_untilize(in_addr, block); 
}

void Unpack::untilize_uninit(DataFormat in_src_format, DataFormat in_dst_format) {
    unpack_untilize_uninit(in_src_format, in_dst_format);
}

// pack

void Unpack::pack_tile(uint32_t ifrom_dst, uint32_t addr) {
    // nothing to do
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

