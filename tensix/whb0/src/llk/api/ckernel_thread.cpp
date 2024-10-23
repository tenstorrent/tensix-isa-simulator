
#include <cstdint>
#include <cassert>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "circular_buffer.h"
#include "ckernel_ops.h"
#include "ckernel_thread.h"
#include "ckernel_gpr_map.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

using core::DataFormat;
using core::Tensix;
using core::ThreadType;
using core::ThreadSync;
using core::Thread;

using llk::api::CBInterface;

//
//    ThreadContext
//

ThreadContext::ThreadContext() {
    reset();
}

ThreadContext::~ThreadContext() { }

void ThreadContext::reset() {
    for (uint32_t i = 0; i < 32; i++) {
        CBInterface &cb = m_cb_interface[i];
        cb.fifo_size = 0;
        cb.fifo_limit = 0;
        cb.fifo_page_size = 0;
        cb.fifo_num_pages = 0;
        cb.fifo_rd_ptr = 0;
        cb.fifo_wr_ptr = 0;
        cb.tiles_acked = 0;
        cb.tiles_received = 0;
        cb.fifo_wr_tile_ptr = 0;
        m_cb_src_format[i] = DataFormat::Float32;
        m_cb_dst_format[i] = DataFormat::Float32;
    }
}

// CB API for external use only

void ThreadContext::setup_cb(
        uint32_t cb_id,
        uint32_t fifo_addr,
        uint32_t fifo_size,
        uint32_t fifo_num_pages,
        uint32_t fifo_page_size) {
    // NOTE: fifo_addr, fifo_size and fifo_limit in 16B words
    assert(cb_id < 32);
    CBInterface &cb = m_cb_interface[cb_id];
    cb.fifo_size = fifo_size;
    cb.fifo_limit = fifo_addr + fifo_size - 1;
    cb.fifo_page_size = fifo_page_size;
    cb.fifo_num_pages = fifo_num_pages;
    cb.fifo_rd_ptr = fifo_addr;
    cb.fifo_wr_ptr = fifo_addr;
    cb.tiles_acked = 0;
    cb.tiles_received = 0;
    // this is currently used for in-order packing (the default mode)
    cb.fifo_wr_tile_ptr = 0;
}

void ThreadContext::cb_push_back(uint32_t cb_id, uint32_t num_pages) {
    assert(cb_id < 32);
    CBInterface &cb = m_cb_interface[cb_id];
    uint32_t num_words = num_pages * cb.fifo_page_size;
    cb.tiles_received += num_pages;
    cb.fifo_wr_ptr += num_words;
    // this will basically reset fifo_wr_ptr to fifo_addr -- no other wrap is legal
    // producer always writes into contiguous memory, it cannot wrap
    if (cb.fifo_wr_ptr > cb.fifo_limit) {
        // TODO: change this to fifo_wr_ptr
        cb.fifo_wr_ptr -= cb.fifo_size;
    }
}

void ThreadContext::cb_pop_front(uint32_t cb_id, uint32_t num_pages) {
    assert(cb_id < 32);
    CBInterface &cb = m_cb_interface[cb_id];
    uint32_t num_words = num_pages * cb.fifo_page_size;
    cb.tiles_acked += num_pages;
    cb.fifo_rd_ptr += num_words;
    // this will basically reset fifo_rd_ptr to fifo_addr -- no other wrap is legal
    // consumer always reads from contiguous memory, it cannot wrap
    if (cb.fifo_rd_ptr > cb.fifo_limit) {
        cb.fifo_rd_ptr -= cb.fifo_size;
    }
}

uint32_t ThreadContext::cb_rd_tile_addr(uint32_t cb_id, uint32_t tile_index) {
    // returns address in 16B words
    assert(cb_id < 32);
    CBInterface &cb = m_cb_interface[cb_id];
    return cb.fifo_rd_ptr + cb.fifo_page_size * tile_index;
}

uint32_t ThreadContext::cb_wr_tile_addr(uint32_t cb_id, uint32_t tile_index) {
    // returns address in 16B words
    assert(cb_id < 32);
    CBInterface &cb = m_cb_interface[cb_id];
    return cb.fifo_wr_ptr + cb.fifo_page_size * tile_index;
}

//
//    ThreadBase
//

ThreadBase *ThreadBase::m_curr_thread = nullptr;
ThreadContext *ThreadBase::m_curr_thread_context = nullptr;

ThreadBase::ThreadBase(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context):
            Thread(thread_type, tensix, thread_sync),
            m_thread_context(thread_context),
            // TODO: Figure out correct value for replay buffer depth
            m_log2_replay_buffer_depth(5), 
            m_replay_buffer(size_t(1) << m_log2_replay_buffer_depth),
            m_replay_index(0),
            m_replay_left(0),
            m_replay_execute_while_loading(false),
            m_mop_type(~uint32_t(0)),
            m_mop_zmask_hi16(0),
            m_cfg_state_id(0),
            m_unp_cfg_context(0),
            m_pack_sync_tile_dst_ptr(0),
            m_math_sync_tile_dst_index(0),
            m_dest_offset_id(0),
            m_l1_buffer_addr(0) { 
    reset();
}

ThreadBase::~ThreadBase() { }

void ThreadBase::reset() {
    // Initialize GPRs to all 0s
    for (uint32_t i = 0; i < 64; i++) {
        write_dma_reg(i, 0);
    }

    // Init L1 buffer with 1.0f (used for reduce max)
    // (used by unpacker only)
    m_l1_buffer_addr = m_local_base;
    float *ptr = reinterpret_cast<float *>(map_addr(m_l1_buffer_addr));
    for (uint32_t i = 0; i < 16; i++) {
        ptr[i] = 1.0f;
    }

    // Store L1 buffer address for reduce input 1
    // Unpacker is programmed to automatically skip tile header (+1) 
    // Since address_b points to tile content, we need to -1 address
    // (in terms of 16B words) to offet unpacker's automatic +1.
    write_dma_reg(p_gpr_unpack::L1_BUFFER_ADDR, (m_l1_buffer_addr >> 4) - 1);
}

void ThreadBase::read_globals() {
    m_cfg_state_id = cfg_state_id;
    m_unp_cfg_context = unp_cfg_context;
    m_pack_sync_tile_dst_ptr = pack_sync_tile_dst_ptr;
    m_math_sync_tile_dst_index = math_sync_tile_dst_index;
    m_dest_offset_id = dest_offset_id;
}

void ThreadBase::write_globals() {
    cfg_state_id = m_cfg_state_id;
    unp_cfg_context = m_unp_cfg_context;
    pack_sync_tile_dst_ptr = m_pack_sync_tile_dst_ptr;
    math_sync_tile_dst_index = m_math_sync_tile_dst_index;
    dest_offset_id = m_dest_offset_id;
}

void ThreadBase::wait(std::function<bool ()> cond) {
    m_thread_sync->wait(cond);
}

void ThreadBase::run_op(const Op &op) {
    if (op.type == OpType::MOP_CFG) {
        assert(m_replay_left == 0);
        m_mop_zmask_hi16 = op.args[0] & 0xFFFF;
    } else if (op.type == OpType::MOP) {
        assert(m_replay_left == 0);
        const uint32_t *ap = op.args;
        uint32_t mop_type = ap[0];
        assert(mop_type == m_mop_type);
        if (mop_type == 0) {
            uint32_t loop_count = ap[1];
            uint32_t zmask_lo16 = ap[2] & 0xFFFF;
            uint32_t zmask = (m_mop_zmask_hi16 << 16) | zmask_lo16;
            m_mop_templ_0.run_mop(this, loop_count, zmask);
        } else if (mop_type == 1) {
            m_mop_templ_1.run_mop(this);
        } else {
            assert(false);
        }
    } else if (op.type == OpType::REPLAY) {
        const uint32_t *ap = op.args;
        run_replay(ap[0], ap[1], ap[2], ap[3]);
    } else if (m_replay_left > 0) {
        replay_op(op);
    } else {
        run_thread_op(this, op);
    }
}

void ThreadBase::set_mop_templ_0(const ckernel_unpack_template &templ) {
    m_mop_type = 0;
    m_mop_templ_0 = templ;
}

void ThreadBase::set_mop_templ_1(const ckernel_template &templ) {
    m_mop_type = 1;
    m_mop_templ_1 = templ;
}

//
//    ACHTUNG: This is simplified implemenation of CB interfaces as
//        it does not manage the external tiles_received / tiles_acked registers
//

void ThreadBase::cb_wait_tiles(uint32_t operand, uint32_t num_tiles) {
    CBInterface *cb = &m_thread_context->get_cb_interface(operand);
    wait([cb, num_tiles]() -> bool {
        return (cb->tiles_received - cb->tiles_acked >= num_tiles); 
    });
}

void ThreadBase::cb_pop_tiles(uint32_t operand, uint32_t num_tiles) {
    CBInterface &cb = m_thread_context->get_cb_interface(operand);
    uint32_t num_words = num_tiles * cb.fifo_page_size;

    cb.tiles_acked += num_tiles;
    cb.fifo_rd_ptr += num_words;

    if (cb.fifo_rd_ptr > cb.fifo_limit) {
        cb.fifo_rd_ptr -= cb.fifo_size;
    }
}

void ThreadBase::cb_wait_for_free_tiles(uint32_t operand, uint32_t num_tiles) {
    CBInterface *cb = &m_thread_context->get_cb_interface(operand);
    wait([cb, num_tiles]() -> bool {
        uint32_t free_tiles_wrap = cb->fifo_num_pages - (cb->tiles_received - cb->tiles_acked);
        return ((int32_t)free_tiles_wrap >= (int32_t)num_tiles);
    });
}

void ThreadBase::cb_push_tiles(uint32_t operand, uint32_t num_tiles) {
    CBInterface &cb = m_thread_context->get_cb_interface(operand);
    uint32_t num_words = num_tiles * cb.fifo_page_size;

    cb.fifo_wr_ptr += num_words;
    cb.fifo_wr_tile_ptr = 0;

    if (cb.fifo_wr_ptr > cb.fifo_limit) {
        cb.fifo_wr_ptr -= cb.fifo_size;
    }

    cb.tiles_received += num_tiles;
}

void ThreadBase::run_replay(
        uint32_t start_idx,
        uint32_t len,
        uint32_t execute_while_loading,
        uint32_t load_mode) {
    assert(load_mode == 0 || load_mode == 1);
    uint32_t mask = (1 << m_log2_replay_buffer_depth) - 1;
    start_idx &= mask;
    len &= mask;
    assert(size_t(start_idx + len) <= m_replay_buffer.size());
    if (load_mode == 0) {
        for (uint32_t i = 0; i < len; i++) {
            run_thread_op(this, m_replay_buffer[start_idx + i]);
        }
    } else {
        m_replay_index = start_idx;
        m_replay_left = len;
        m_replay_execute_while_loading = execute_while_loading;
    }
}

void ThreadBase::replay_op(const Op &op) {
    if (m_replay_execute_while_loading) {
        run_thread_op(this, op);
    }
    m_replay_buffer[m_replay_index] = op;
    m_replay_index++;
    m_replay_left--;
}

//
//    Global thread variables
//

uint32_t cfg_state_id = 0;
uint32_t unp_cfg_context = 0;
uint32_t pack_sync_tile_dst_ptr = 0;
uint32_t math_sync_tile_dst_index = 0;
uint32_t dest_offset_id = 0;

//
//    Public functions
//

void run_op(
        OpType type,
        uint32_t arg0/* = 0*/,
        uint32_t arg1/* = 0*/,
        uint32_t arg2/* = 0*/,
        uint32_t arg3/* = 0*/,
        uint32_t arg4/* = 0*/,
        uint32_t arg5/* = 0*/,
        uint32_t arg6/* = 0*/,
        uint32_t arg7/* = 0*/,
        uint32_t arg8/* = 0*/,
        uint32_t arg9/* = 0*/,
        uint32_t arg10/* = 0*/,
        uint32_t arg11/* = 0*/,
        uint32_t arg12/* = 0*/,
        uint32_t arg13/* = 0*/,
        uint32_t arg14/* = 0*/,
        uint32_t arg15/* = 0*/) {
    Op op = 
        make_op(
            type, 
            arg0, 
            arg1, 
            arg2, 
            arg3, 
            arg4, 
            arg5, 
            arg6, 
            arg7, 
            arg8, 
            arg9, 
            arg10, 
            arg11, 
            arg12, 
            arg13, 
            arg14, 
            arg15);
    get_thread()->run_op(op);
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

