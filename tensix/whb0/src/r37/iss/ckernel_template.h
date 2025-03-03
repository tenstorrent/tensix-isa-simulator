// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "ckernel_ops.h"

namespace ckernel {

// defined in "ckernel_thread.h"
class ThreadBase;

//
//    ckernel_template
//

//
//    Here is the basic outline of a MOP loop and the definition of the variables used.
//
//    LOOP_OUTER: <OUTER_LOOP_COUNT>
//      START_OP
//      LOOP_INNER: <INNER_LOOP_COUNT>
//        LOOP_OP0
//        LOOP_OP1
//      END_LOOP_INNER
//      END_OP0
//      END_OP1
//    END_LOOP_OUTER
//

//
//     Note: The last iteration of inner loop will also be the last iteration
//         of the outer loop when outer loop length = 1.
//         This means that in this case, last_inner_loop_instr will be replaced 
//         by the last_outer_loop_instr
//
//     This is how m_loop0_last_instr and m_loop1_last_instr are executed:
//
//     if (last_inner_loop_iter && last_outer_loop_iter)
//         m_loop_op1 = m_loop0_last_instr;
//     else if (last_inner_loop_iter)
//         m_loop_op1 = m_loop1_last_instr;
//     else
//         m_loop_op1 = m_loop_op1;
//

class ckernel_template {
public:
    ckernel_template() = default;
    ckernel_template(const ckernel_template &) = default;
    ckernel_template(
        uint32_t outer_loop_len, 
        uint32_t inner_loop_len, 
        uint32_t loop_op);
    ckernel_template(
        uint32_t outer_loop_len, 
        uint32_t inner_loop_len, 
        uint32_t loop_op0, 
        uint32_t loop_op1);
public:
    void set_loop_op0(uint32_t loop_op);
    void set_loop_op1(uint32_t loop_op);
    void set_end_ops(uint32_t end_op0, uint32_t end_op1);
    void set_end_op(uint32_t end_op0);
    void set_start_op(uint32_t start_op0);
    void set_last_inner_loop_instr(uint32_t op);
    void set_last_outer_loop_instr(uint32_t op);
    void program(uint32_t *instrn_buffer);
    static void run(uint32_t *instrn_buffer);
    void program_and_run(uint32_t *instrn_buffer);
    void run_mop(ThreadBase *thread);
private:
    uint32_t m_outer_loop_len;
    uint32_t m_inner_loop_len;
    Op m_loop_op0;
    Op m_loop_op1;
    Op m_end_op0;
    Op m_end_op1;
    Op m_start_op0;
    Op m_loop0_last_instr; 
    Op m_loop1_last_instr; 
};

//
//    ckernel_unpack_template
//

//
//    Unpack template is a single-loop template that allows for some
//    dynamic selection of instructions based on zmask
//    The basic template looks like this:
//
//    LOOP:
//      if (zmask[iteration]):
//        UNPACR_A0
//        UNPACR_A1
//        UNPACR_A2
//        UNPACR_A3
//        UNPACR_B
//      else
//        SKIP_A0
//        SKIP_A1
//        SKIP_A2
//        SKIP_A3
//        SKIP_B
//
//     The configuration allows the following changes:
//       - B enable - if 0, removes the UNPACR_B/SKIP_B instruction
//       - HALO enable - if 0, removes A1/2/3 instructions
//       - Each of the UNPACR/SKIP instructions can be anything at all,
//         although the most common use is for UNPACR to unpack, and SKIP to be an
//         unpack NOP that increments the context counter
//

class ckernel_unpack_template {
public:
    ckernel_unpack_template() = default;
    ckernel_unpack_template(const ckernel_unpack_template &) = default;
    ckernel_unpack_template(
        bool unpackB, 
        bool unpackHalo, 
        uint32_t A0_instr, 
        uint32_t A1_instr, 
        uint32_t A2_instr, 
        uint32_t A3_instr, 
        uint32_t skipA_instr,
        uint32_t B_instr, 
        uint32_t skipB_instr);
public:
    // SKIPPED: Default ops and convenience factory methods
    void program(uint32_t *instrn_buffer) const;    
    static void run(uint32_t *instrn_buffer, uint32_t count, uint32_t zmask);
    static void run(uint32_t *instrn_buffer, uint32_t count);
    void program_and_run(uint32_t *instrn_buffer, uint32_t count, uint32_t zmask = 0);
    void run_mop(ThreadBase *thread, uint32_t loop_count, uint32_t zmask);
private:
    bool m_unpackB;
    bool m_unpack_halo;
    Op m_A0_instr;
    Op m_A1_instr;
    Op m_A2_instr;
    Op m_A3_instr;
    Op m_B_instr;
    Op m_skipA_instr;
    Op m_skipB_instr;
};

} // namespace ckernel

