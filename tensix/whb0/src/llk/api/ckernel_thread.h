// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <functional>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "circular_buffer.h"
#include "ckernel_ops.h"
#include "ckernel_template.h"

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

//
//    ThreadContext
//

// state shared by all threads running on one tensix core
// contais LLK-specific data that do not fit into core::Tensix
// (there is 1:1 association between ThreadContext and core::Tensix instances)

class ThreadContext {
    friend class ThreadBase;
public:
    ThreadContext();
    ~ThreadContext();
public:
    void reset();
    void setup_cb(
        uint32_t cb_id,
        uint32_t fifo_addr,
        uint32_t fifo_size,
        uint32_t fifo_num_pages,
        uint32_t fifo_page_size);
    void cb_push_back(uint32_t cb_id, uint32_t num_pages);
    void cb_pop_front(uint32_t cb_id, uint32_t num_pages);
    uint32_t cb_rd_tile_addr(uint32_t cb_id, uint32_t tile_index);
    uint32_t cb_wr_tile_addr(uint32_t cb_id, uint32_t tile_index);
    CBInterface &get_cb_interface(uint32_t cb_id) {
        assert(cb_id < 32);
        return m_cb_interface[cb_id];
    }
    DataFormat get_cb_src_format(uint32_t cb_id) {
        assert(cb_id < 32);
        return m_cb_src_format[cb_id];
    }
    DataFormat get_cb_dst_format(uint32_t cb_id) {
        assert(cb_id < 32);
        return m_cb_dst_format[cb_id];
    }
private:
    CBInterface m_cb_interface[32];
    DataFormat m_cb_src_format[32];
    DataFormat m_cb_dst_format[32];
};

//
//    ThreadBase
//

class ThreadBase: public Thread {
public:
    ThreadBase(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context);
    ~ThreadBase();
public:
    void reset();
    static void set_curr_thread(ThreadBase *thread) {
        m_curr_thread = thread;
        m_curr_thread_context = thread->m_thread_context;
    }
    static ThreadBase *get_curr_thread() {
        return m_curr_thread;
    }
    static ThreadContext *get_curr_thread_context() {
        return m_curr_thread_context;
    }
    void read_globals();
    void write_globals();
    void wait(std::function<bool ()> cond);
    void run_op(const Op &op);
    void set_mop_templ_0(const ckernel_unpack_template &templ);
    void set_mop_templ_1(const ckernel_template &templ);
    uint32_t *map_l1_addr(uint32_t addr) {
        return reinterpret_cast<uint32_t *>(map_addr(addr));
    }
    uint32_t *get_l1_buffer_ptr() {
        return reinterpret_cast<uint32_t *>(map_addr(m_l1_buffer_addr));
    }
    void cb_wait_tiles(uint32_t operand, uint32_t num_tiles);
    void cb_pop_tiles(uint32_t operand, uint32_t num_tiles);
    void cb_wait_for_free_tiles(uint32_t operand, uint32_t num_tiles);
    void cb_push_tiles(uint32_t operand, uint32_t num_tiles);
private:
    void run_replay(
        uint32_t start_idx,
        uint32_t len,
        uint32_t execute_while_loading,
        uint32_t load_mode);
    void replay_op(const Op &op);
private:
    static ThreadBase *m_curr_thread;
    static ThreadContext *m_curr_thread_context;
private:
    ThreadContext *m_thread_context;
private:
    uint32_t m_log2_replay_buffer_depth;
    std::vector<Op> m_replay_buffer;
    uint32_t m_replay_index;
    uint32_t m_replay_left;
    bool m_replay_execute_while_loading;
private:
    uint32_t m_mop_type;
    uint32_t m_mop_zmask_hi16;
    ckernel_unpack_template m_mop_templ_0;
    ckernel_template m_mop_templ_1;
private:
    uint32_t m_cfg_state_id;
    uint32_t m_unp_cfg_context;
    uint32_t m_pack_sync_tile_dst_ptr;
    uint32_t m_math_sync_tile_dst_index;
    uint32_t m_dest_offset_id;
    uint32_t m_l1_buffer_addr;
};

//
//    Global thread variables
//

extern uint32_t cfg_state_id;
extern uint32_t unp_cfg_context;
extern uint32_t pack_sync_tile_dst_ptr;
extern uint32_t math_sync_tile_dst_index;
extern uint32_t dest_offset_id;

//
//    Public functions
//

inline ThreadBase *get_thread() {
    return ThreadBase::get_curr_thread();
}

inline CBInterface &get_cb_interface(uint32_t cb_id) {
    return ThreadBase::get_curr_thread_context()->get_cb_interface(cb_id);
}

inline DataFormat get_pack_src_format(uint32_t cb_id) {
    return ThreadBase::get_curr_thread_context()->get_cb_src_format(cb_id);
}

inline DataFormat get_pack_dst_format(uint32_t cb_id) {
    return ThreadBase::get_curr_thread_context()->get_cb_dst_format(cb_id);
}

inline DataFormat get_unpack_src_format(uint32_t cb_id) {
    return ThreadBase::get_curr_thread_context()->get_cb_src_format(cb_id);
}

inline DataFormat get_unpack_dst_format(uint32_t cb_id) {
    return ThreadBase::get_curr_thread_context()->get_cb_dst_format(cb_id);
}

inline void wait(std::function<bool ()> cond) {
    ThreadBase::get_curr_thread()->wait(cond);    
}

inline uint32_t *get_l1_buffer_ptr() {
    return ThreadBase::get_curr_thread()->get_l1_buffer_ptr();
}

inline void cb_wait_tiles(uint32_t operand, uint32_t num_tiles) {
    ThreadBase::get_curr_thread()->cb_wait_tiles(operand, num_tiles);
}

inline void cb_pop_tiles(uint32_t operand, uint32_t num_tiles) {
    ThreadBase::get_curr_thread()->cb_pop_tiles(operand, num_tiles);
}

inline void cb_wait_for_free_tiles(uint32_t operand, uint32_t num_tiles) {
    ThreadBase::get_curr_thread()->cb_wait_for_free_tiles(operand, num_tiles);
}

inline void cb_push_tiles(uint32_t operand, uint32_t num_tiles) {
    ThreadBase::get_curr_thread()->cb_push_tiles(operand, num_tiles);
}

void run_op(
    OpType type,
    uint32_t arg0 = 0,
    uint32_t arg1 = 0,
    uint32_t arg2 = 0,
    uint32_t arg3 = 0,
    uint32_t arg4 = 0,
    uint32_t arg5 = 0,
    uint32_t arg6 = 0,
    uint32_t arg7 = 0,
    uint32_t arg8 = 0,
    uint32_t arg9 = 0,
    uint32_t arg10 = 0,
    uint32_t arg11 = 0,
    uint32_t arg12 = 0,
    uint32_t arg13 = 0,
    uint32_t arg14 = 0,
    uint32_t arg15 = 0);

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

