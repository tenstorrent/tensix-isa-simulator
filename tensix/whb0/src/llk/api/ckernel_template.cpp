// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_template.h"
#include "ckernel_thread.h"
#include "ckernel_asm.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

namespace ckernel = llk::api;

//
//    ckernel_template
//

ckernel_template::ckernel_template(
        uint32_t outer_loop_len, 
        uint32_t inner_loop_len, 
        const Op &loop_op): 
            m_outer_loop_len(outer_loop_len), 
            m_inner_loop_len(inner_loop_len), 
            m_loop_op0(loop_op), 
            m_loop_op1(TT_OP_NOP), 
            m_end_op0(TT_OP_NOP), 
            m_end_op1(TT_OP_NOP), 
            m_start_op0(TT_OP_NOP) {
    m_loop0_last_instr = loop_op;
    m_loop1_last_instr = loop_op;
}

ckernel_template::ckernel_template(
        uint32_t outer_loop_len, 
        uint32_t inner_loop_len, 
        const Op &loop_op0, 
        const Op &loop_op1): 
            m_outer_loop_len(outer_loop_len), 
            m_inner_loop_len(inner_loop_len), 
            m_loop_op0(loop_op0), 
            m_loop_op1(loop_op1), 
            m_end_op0(TT_OP_NOP), 
            m_end_op1(TT_OP_NOP), 
            m_start_op0(TT_OP_NOP) {
    m_loop0_last_instr = loop_op1;
    m_loop1_last_instr = loop_op1;
}

void ckernel_template::set_loop_op0(const Op &loop_op) {
    m_loop_op0 = loop_op;
}

void ckernel_template::set_loop_op1(const Op &loop_op) {
    m_loop_op1 = loop_op;
}

void ckernel_template::set_end_ops(const Op &end_op0, const Op &end_op1) {
    m_end_op0 = end_op0;
    m_end_op1 = end_op1;
}

void ckernel_template::set_end_op(const Op &end_op0) {
    set_end_ops(end_op0, TT_OP_NOP);
}

void ckernel_template::set_start_op(const Op &start_op0) {
    m_start_op0 = start_op0;
}

void ckernel_template::set_last_inner_loop_instr(const Op &op) {
    m_loop1_last_instr = op;
}

void ckernel_template::set_last_outer_loop_instr(const Op &op) {
    m_loop0_last_instr = op;
}

void ckernel_template::program(uint32_t *instrn_buffer) {
    get_thread()->set_mop_templ_1(*this);
}

void ckernel_template::run(uint32_t *instrn_buffer) {
    // run the double-loop template
    TTI_MOP(1, 0, 0); 
}

void ckernel_template::program_and_run(uint32_t *instrn_buffer) {
    program(instrn_buffer);
    run(instrn_buffer);
}

void ckernel_template::run_mop(ThreadBase *thread) {
    bool have_loop_op1 = (m_loop_op1.type != OpType::NOP); 
    bool have_last_inner_op = (m_loop1_last_instr.type != OpType::NOP);
    bool have_last_outer_op = (m_loop0_last_instr.type != OpType::NOP);
    Op &loop_op0 = have_loop_op1 ? m_loop_op0 : m_loop_op1;
    Op &loop_op1 = have_loop_op1 ? m_loop_op1 : m_loop_op0;
    Op &last_inner_op = have_last_inner_op ? m_loop1_last_instr : loop_op1;
    Op &last_outer_op = have_last_outer_op ? m_loop0_last_instr : last_inner_op;
    for (uint32_t i = 0; i < m_outer_loop_len; i++) {
        thread->run_op(m_start_op0);
        for (uint32_t k = 0; k < m_inner_loop_len; k++) {
            thread->run_op(loop_op0);
            if (k + 1 < m_inner_loop_len) {
                thread->run_op(loop_op1);
            } else if (i + 1 < m_outer_loop_len) {
                thread->run_op(last_inner_op);
            } else {
                thread->run_op(last_outer_op);
            }
        }
        thread->run_op(m_end_op0);
        thread->run_op(m_end_op1);
    }
}

//
//    ckernel_unpack_template
//

ckernel_unpack_template::ckernel_unpack_template(
        bool unpackB, 
        bool unpackHalo, 
        const Op &A0_instr, 
        const Op &A1_instr, 
        const Op &A2_instr, 
        const Op &A3_instr, 
        const Op &skipA_instr,
        const Op &B_instr, 
        const Op &skipB_instr): 
            m_unpackB(unpackB), 
            m_unpack_halo(unpackHalo), 
            m_A0_instr(A0_instr), 
            m_A1_instr(A1_instr), 
            m_A2_instr(A2_instr), 
            m_A3_instr(A3_instr), 
            m_B_instr(B_instr), 
            m_skipA_instr(skipA_instr), 
            m_skipB_instr(skipB_instr) { }

void ckernel_unpack_template::program(uint32_t *instrn_buffer) const {
    get_thread()->set_mop_templ_0(*this);
}

void ckernel_unpack_template::run(uint32_t *instrn_buffer, uint32_t count, uint32_t zmask) {
    // Set the top 16 bits of zmask - we could skip this for count <= 16
    TT_MOP_CFG(zmask >> 16);              
    TT_MOP(0, count - 1, zmask & 0xFFFF);
}

void ckernel_unpack_template::run(uint32_t *instrn_buffer, uint32_t count) {
    TT_MOP(0, count - 1, 0);
}

void ckernel_unpack_template::program_and_run(
        uint32_t *instrn_buffer, 
        uint32_t count, 
        uint32_t zmask/* = 0*/) {
    program(instrn_buffer);
    run(instrn_buffer, count, zmask);
}

void ckernel_unpack_template::run_mop(ThreadBase *thread, uint32_t loop_count, uint32_t zmask) {
    // performs (loop_count + 1) iterations
    // assume (loop_count < 32) ?
    uint32_t mask = 1;
    for (uint32_t i = 0; i <= loop_count; i++) {
        // TODO: Figure out correct interpretation of zmask
        if ((mask & zmask) == 0) {
            thread->run_op(m_A0_instr);
            if (m_unpack_halo) {
                thread->run_op(m_A1_instr);
                thread->run_op(m_A2_instr);
                thread->run_op(m_A3_instr);
            }
            if (m_unpackB) {
                thread->run_op(m_B_instr);
            }
        } else {
            thread->run_op(m_skipA_instr);
            if (m_unpackB) {
                thread->run_op(m_skipB_instr);
            }
        }
        mask <<= 1;
    }
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

