// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/addr_mod.hpp"
#include "core/template.hpp"
#include "core/instr_params.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/math.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_setrwc;
using core::p_stall;

using core::AddrMod;
using core::MopTemplate1;

/*
ACHTUNG: This algorithm has bug as it produces transposed faces in Dest.

Proof:

Let A = SrcA, B = SrcB, D = Dest, a = opA, b = opB, d = desired result

Then algorithm computes:

D00 = B00 * A00 + B10 * A10 = a00 * b00 + a01 * b10 = d00
D01 = B01 * A00 + B11 * A10 = a10 * b00 + a11 * b10 = d10  # want d01
D10 = B00 * A01 + B10 * A11 = a00 * b01 + a01 * b11 = d01  # want d10
D11 = B01 * A01 + B11 * A11 = a10 * b01 + a11 * b11 = d11

Correct algorithm must produce:

D00 = B00 * A00 + B10 * A10 = a00 * b00 + a01 * b10 = d00
D01 = B00 * A01 + B10 * A11 = a00 * b01 + a01 * b11 = d01
D10 = B01 * A00 + B11 * A10 = a10 * b00 + a11 * b10 = d10
D11 = B01 * A01 + B11 * A11 = a10 * b01 + a11 * b11 = d11
*/

//
//    Math
//

void Math::math_matmul_init(uint32_t NUM_FIDELITY_PHASES, uint32_t transpose) {
    math_matmul_configure_addrmod(NUM_FIDELITY_PHASES);
    math_matmul_configure_mop(NUM_FIDELITY_PHASES, (transpose != 0));
    reset_counters(p_setrwc::SET_ABD_F);
} 

void Math::math_matmul(uint32_t NUM_FIDELITY_PHASES, uint32_t dst_index) {
    static constexpr uint32_t
        CLR_AB = p_setrwc::CLR_AB,
        CLR_NONE = p_setrwc::CLR_NONE,
        SET_D = p_setrwc::SET_D,
        SET_ABD = p_setrwc::SET_ABD;

    set_dst_write_addr(DstTileLayout::Default, DstTileShape::Tile32x32, dst_index);

    bool high_fidelity = (NUM_FIDELITY_PHASES > 0);

    if (high_fidelity) {
        MOP(1, 0, 0);
        MOP(1, 0, 0);
        INCRWC(2, 0, 8, 0);
        INCRWC(2, 0, 8, 0);
        INCRWC(2, 0, 8, 0);
        INCRWC(2, 0, 8, 0);
        SETRWC(CLR_NONE, 0, 0, 0, 0, SET_D);
        MOP(1, 0, 0);
        MOP(1, 0, 0);  // Run 4 times
        SETRWC(CLR_AB, 0, 0, 0, 0, SET_ABD);
    } else {
        MOP(1, 0, 0);
        MOP(1, 0, 0);  // Run mop twice
        SETRWC(CLR_AB, 0, 0, 0, 0, 0);
    }
} 

void Math::math_matmul_configure_addrmod(uint32_t NUM_FIDELITY_PHASES) {
    // MVMUL does D = B*A

    // Inner Loop --> 32/8 = 4 times for the full 32x16 face
    // DEST -- 8 rows are calculated each time
    // SRCB -- 8 rows are needed
    // SRCA -- full 16x16 gets used -- hardware will pair cols of A with rows of B
    // D[8,16] = B[8,16] * A[16,16]
    AddrMod a0;
    a0.srca.incr = 0;
    a0.srca.clr = 0;
    a0.srca.cr = 0;
    a0.srcb.incr = 8;
    a0.srcb.clr = 0;
    a0.srcb.cr = 0;
    a0.dest.incr = 8;
    a0.dest.clr = 0;
    a0.dest.cr = 0;
    set_addr_mod(0, a0);

    bool high_fidelity = (NUM_FIDELITY_PHASES > 0);

    if (high_fidelity) {
        // Fidelity Loop
        // DEST -- CR on dest for next fidelity phase
        // SRCB -- CR on srcB for next fidelity phase
        // SRCA -- CR on srcA for next fidelity phase
        // Fidelity -- increment phase
        AddrMod a1;
        a1.srca.incr = 0;
        a1.srca.clr = 0;
        a1.srca.cr = 1;
        a1.srcb.incr = 0;
        a1.srcb.clr = 0;
        a1.srcb.cr = 1;
        a1.dest.incr = 0;
        a1.dest.clr = 0;
        a1.dest.cr = 1;
        a1.fidelity.incr = 1;
        a1.fidelity.clr = 0;
        set_addr_mod(1, a1);

        // Last outer loop,
        // DEST -- keep incrementing
        // SRCB -- CR to either go back to beginning or to next 32x16
        // SRCA -- increment to next 16x16
        // Fidelity -- reset fidelity
        AddrMod a2;
        a2.srca.incr = 16;
        a2.srca.clr = 0;
        a2.srca.cr = 1;
        a2.srcb.incr =  0;
        a2.srcb.clr = 0;
        a2.srcb.cr = 1;
        a2.dest.incr = 32;
        a2.dest.clr = 0;
        a2.dest.cr = 1;
        a2.fidelity.incr = 0;
        a2.fidelity.clr = 1;
        set_addr_mod(2, a2);
    } else {
        // Last inner loop,
        // DEST -- keep incrementing
        // SRCB -- CR to either go back to beginning or to next 32x16
        // SRCA -- increment to next 16x16
        // Fidelity -- reset fidelity
        AddrMod a2;
        a2.srca.incr = 16;
        a2.srca.clr = 0;
        a2.srca.cr = 0;
        a2.srcb.incr = 0;
        a2.srcb.clr = 0;
        a2.srcb.cr = 1;
        a2.dest.incr = 32;
        a2.dest.clr = 0;
        a2.dest.cr = 1;
        a2.fidelity.incr = 0;
        a2.fidelity.clr = 1;
        set_addr_mod(2, a2);

        // Last outer loop,
        // DEST -- clear and CR on dest for next 32x16 x 16x16 matmul to accumulate on top
        // SRCB -- increment to next 32x16
        // SRCA -- increment to next 16x16
        AddrMod a3;
        a3.srca.incr = 16;
        a3.srca.clr = 0;
        a3.srca.cr = 0;
        a3.srcb.incr = 32;
        a3.srcb.clr = 0;
        a3.srcb.cr = 1;
        a3.dest.incr = 0;
        a3.dest.clr = 1;
        a3.dest.cr = 1;
        a3.fidelity.incr = 0;
        a3.fidelity.clr = 0;
        set_addr_mod(3, a3);
    }
} 
void Math::math_matmul_configure_mop(uint32_t NUM_FIDELITY_PHASES, bool transpose) {
    static constexpr uint32_t 
        CLR_NONE = p_setrwc::CLR_NONE,
        SRCA_VLD = p_stall::SRCA_VLD,
        SRCB_VLD = p_stall::SRCB_VLD,
        STALL_MATH = p_stall::STALL_MATH;

    bool high_fidelity = (NUM_FIDELITY_PHASES > 0);

    uint32_t num_inner_loops = 32 >> 3; // 8 rows produced per op

    if (high_fidelity) {
        MopTemplate1 m;
        m.loop_outer = NUM_FIDELITY_PHASES;
        m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
        m.loop_inner = num_inner_loops;
        m.loop_op0 = TT_OP(MVMUL(CLR_NONE, 0, 0, 0));
        m.last_inner_op = TT_OP(MVMUL(CLR_NONE, 0, 1, 0));
        m.last_outer_op = TT_OP(MVMUL(CLR_NONE, 0, 2, 0));
        set_mop_templ_1(m);
    } else {
        // performs a 32x16 x 16x32 matmul
        MopTemplate1 m;
        m.loop_outer = 2;
        m.start_op = TT_OP(STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD));
        m.loop_inner = num_inner_loops;
        m.loop_op0 = TT_OP(MVMUL(CLR_NONE, 0, 0, 0));
        m.last_inner_op = TT_OP(MVMUL(CLR_NONE, 0, 2, 0));
        m.last_outer_op = TT_OP(MVMUL(CLR_NONE, 0, 3, 0));
        set_mop_templ_1(m);
    }
} 

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

