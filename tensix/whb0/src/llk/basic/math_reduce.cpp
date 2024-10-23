
#include <cstdint>
#include <cassert>

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

using core::AddrMod;
using core::MopTemplate1;

using core::p_setrwc;
using core::p_movb2d;
using core::p_movd2b;
using core::p_movb2a;
using core::p_stall;
using core::p_zeroacc;
using core::p_gpool;

//
//    Math
//

void Math::math_reduce_init(PoolType type, ReduceDim dim, int num_fidelity_phases) {
    bool high_fidelity = (num_fidelity_phases > 0 && num_fidelity_phases <= 4);

    reduce_configure_addrmod(type, high_fidelity);
    if (high_fidelity) {
        reduce_configure_mop(dim, num_fidelity_phases);
    }

    reset_counters(p_setrwc::SET_ABD_F); 
}

void Math::math_reduce(
        PoolType type, 
        ReduceDim dim, 
        int num_fidelity_phases, 
        bool is_fp32_dest_acc_en,
        uint32_t dst_index) {
    static constexpr uint32_t
        CLR_AB = p_setrwc::CLR_AB,
        CLR_NONE = p_setrwc::CLR_NONE,
        SET_AB = p_setrwc::SET_AB,
        SET_D = p_setrwc::SET_D,
        SET_AD = p_setrwc::SET_AD,
        SET_BD = p_setrwc::SET_BD,
        CR_D = p_setrwc::CR_D,
        SRCA_VLD = p_stall::SRCA_VLD,
        SRCB_VLD = p_stall::SRCB_VLD,
        STALL_MATH = p_stall::STALL_MATH,
        CLR_SPECIFIC = p_zeroacc::CLR_SPECIFIC,
        CLR_SPECIFIC_32B = p_zeroacc::CLR_SPECIFIC_32B,
        DIM_16X16 = p_gpool::DIM_16X16,
        INDEX_DIS = p_gpool::INDEX_DIS;

    bool high_fidelity = (num_fidelity_phases > 0 && num_fidelity_phases <= 4);
    set_dst_write_addr(DstTileLayout::Default, DstTileShape::Tile32x32, dst_index);
    if (dim == ReduceDim::REDUCE_ROW) {
        static constexpr uint32_t
            SRC_ROW16_OFFSET = p_movb2d::SRC_ROW16_OFFSET,
            MOV_1_ROW = p_movb2d::MOV_1_ROW,
            MOV_4_ROWS = p_movb2d::MOV_4_ROWS;

        // Transpose for each face in src A done at unpacker, and pool
        STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);

        if (type == PoolType::MAX) {
            GMPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
        } else {
            if (high_fidelity) {
                MOP(1, 0, 0);
                CLEARDVALID(CLR_AB, 0);
            } else {
                GAPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
            }
        }
        STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);

        if (type == PoolType::MAX) {
            GMPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
        } else {
            if (high_fidelity) {
                MOP(1, 0, 0);
            } else {
                GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
            }
        }

        // Move back to B and transpose
        // we avoid clobbering weights in src B by moving to rows 16 - 31
        SETRWC(CLR_NONE, CR_D, 0, 0, 0, SET_AB);
        MOVD2B(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        GATESRCRST(0b1, 0b1);
        // Note: transpose on src B on works on rows 16 - 31
        TRNSPSRCB();
        // gate math instructions until src B has been updated
        GATESRCRST(0b1, 0b1);
        // Only move rows 2-15 back to dest, so no need to change counters
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);

        // Switch to moving 4 rows for speed up, since no longer need row by row offset
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);

        // Increment dest by 32 for next accumulation
        SETRWC(CLR_NONE, CR_D, 8, 0, 0, SET_D);
        SETRWC(CLR_NONE, CR_D, 8, 0, 0, SET_D);
        SETRWC(CLR_NONE, CR_D, 8, 0, 0, SET_D);
        SETRWC(CLR_AB, CR_D, 8, 0, 0, SET_BD);

        //
        //    Second Tile Row
        //

        // Transpose at unpacker and pool
        STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);

        if (type == PoolType::MAX) {
            GMPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
        } else {
            if (high_fidelity) {
                MOP(1, 0, 0);
                CLEARDVALID(CLR_AB, 0);
            } else {
                GAPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
            }
        }
        STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);

        if (type == PoolType::MAX) {
            GMPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
        } else {
            if (high_fidelity) {
                MOP(1, 0, 0);
            } else {
                GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
            }
        }

        // Move back to B and transpose
        SETRWC(CLR_NONE, CR_D, 0, 0, 0, SET_AB);
        MOVD2B(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        GATESRCRST(0b1, 0b1);
        // Note: transpose on src B on works on rows 16 - 31
        TRNSPSRCB();
        // gate math instructions until src B has been updated
        GATESRCRST(0b1, 0b1);
        // Only move rows 2-15 back to dest, so no need to change counters
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 1, MOV_1_ROW, 0);

        // Switch to moving 4 rows for speed up, since no longer need row by row offset
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);
        MOVB2D(0, SRC_ROW16_OFFSET, 2, MOV_4_ROWS, 0);

        // Increment dest by 32 for next accumulation
        SETRWC(CLR_AB, 0, 0, 0, 0, SET_BD);
    } else if (dim == ReduceDim::REDUCE_COL) {
        for (int row_tile = 0; row_tile < 2; row_tile++) {
            // Just pool
            STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);

            if (type == PoolType::MAX) {
                GMPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
            } else {
                if (high_fidelity) {
                    MOP(1, 0, 0);
                } else {
                    GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
                }
            }
            SETRWC(CLR_NONE, CR_D, 8, 0, 0, SET_D);
            SETRWC(CLR_AB, CR_D, 8, 0, 0, SET_D);

            STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);
            if (type == PoolType::MAX) {
                GMPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
            } else {
                if (high_fidelity) {
                    MOP(1, 0, 0);
                } else {
                    GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0);
                }
            }
            // Reset Dest Counter
            SETRWC(CLR_AB, 0, 0, 0, 0, SET_AD);
        }
    } else if (dim == ReduceDim::REDUCE_SCALAR) {
        static constexpr uint32_t
            // MOVD2B
            SRC_ROW16_OFFSET = p_movd2b::SRC_ROW16_OFFSET,
            MOV_1_ROW = p_movd2b::MOV_1_ROW,
            // MOVB2A
            SRCA_ZERO_OFFSET = p_movb2a::SRCA_ZERO_OFFSET, 
            SRCB_ROW16_OFFSET = p_movb2a::SRCB_ROW16_OFFSET,
            MOV_4_ROWS = p_movb2a::MOV_4_ROWS;

        // ACHTUNG: Why Dest row 4 is used to store results of first pooling pass?
        for (int tile = 0; tile < 3; tile++) {
            // Wait and pool
            STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);
            if (type == PoolType::MAX) {
                GMPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 4);
            } else {
                if (high_fidelity) {
                    MOP(1, 0, 0);
                    CLEARDVALID(CLR_AB, 0);
                } else {
                    GAPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 4);
                }
            }
        }
        // Wait and pool
        STALLWAIT(STALL_MATH, SRCA_VLD | SRCB_VLD);
        if (type == PoolType::MAX) {
            GMPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 4);
        } else {
            if (high_fidelity) {
                MOP(1, 0, 0);
            } else {
                GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 4);
            }
        }

        // Need row in dest as column in src A
        SETRWC(CLR_NONE, CR_D, 0, 0, 0, SET_AB);
        // copy over from dest to B and do transpose
        // use rows 16 - 31 in src B as scratch
        MOVD2B(0, p_movd2b::SRC_ROW16_OFFSET, 0, p_movd2b::MOV_1_ROW, 4);
        GATESRCRST(0b1, 0b1);
        TRNSPSRCB();
        // gate math instructions until src B has been updated
        GATESRCRST(0b1, 0b1);
        // copy over all 16 rows from B to A
        MOVB2A(SRCA_ZERO_OFFSET + 0, 0, MOV_4_ROWS, SRCB_ROW16_OFFSET + 0);
        MOVB2A(SRCA_ZERO_OFFSET + 4, 0, MOV_4_ROWS, SRCB_ROW16_OFFSET + 4);
        MOVB2A(SRCA_ZERO_OFFSET + 8, 0, MOV_4_ROWS, SRCB_ROW16_OFFSET + 8);
        MOVB2A(SRCA_ZERO_OFFSET + 12, 0, MOV_4_ROWS, SRCB_ROW16_OFFSET + 12);
        // gate math instructions until src A has been updated by MOV instructions
        GATESRCRST(0b1, 0b1);
        // zero out scratch in dest
        uint32_t ZERO_ACC_MODE = is_fp32_dest_acc_en ? CLR_SPECIFIC_32B : CLR_SPECIFIC;
        ZEROACC(ZERO_ACC_MODE, 0, 4);

        if (type == PoolType::MAX) {
            GMPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
        } else {
            if (high_fidelity) {
                for (int i = 0; i < num_fidelity_phases - 1; i++) {
                    GAPOOL(CLR_NONE, DIM_16X16, 3, INDEX_DIS, 0);
                }
            }
            GAPOOL(CLR_AB, DIM_16X16, 0, INDEX_DIS, 0);
        }
    } 
}

void Math::reduce_configure_addrmod(PoolType type, bool is_high_fidelity) {
    AddrMod a0;
    a0.srca.incr = 0;
    a0.srcb.incr = 0;
    a0.dest.incr = 0;
    a0.fidelity.incr = 0;
    a0.fidelity.clr = 1;
    set_addr_mod(0, a0);

    AddrMod a1;
    a1.srca.incr = 0;
    a1.srcb.incr = 1;
    a1.dest.incr = 1;
    set_addr_mod(1, a1);

    AddrMod a2;
    a2.srca.incr = 0;
    a2.srcb.incr = 4;
    a2.dest.incr = 4;
    set_addr_mod(2, a2);

    if (is_high_fidelity) {
        AddrMod a3;
        a3.srca.incr = 0;
        a3.srcb.incr = 0;
        a3.dest.incr = 0;
        a3.fidelity.incr = 1;
        set_addr_mod(3, a3);
    } 
}
 
void Math::reduce_configure_mop(ReduceDim dim, int num_fidelity_phases) {
    static constexpr uint32_t
        CLR_NONE = p_setrwc::CLR_NONE,
        DIM_16X16 = p_gpool::DIM_16X16,
        INDEX_DIS = p_gpool::INDEX_DIS;

    if (dim == ReduceDim::REDUCE_SCALAR) {
        MopTemplate1 m;
        m.loop_outer = 1;
        m.loop_inner = num_fidelity_phases;
        m.loop_op0 = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 3, INDEX_DIS, 4));
        m.last_inner_op = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 4));
        m.last_outer_op = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 4));
        set_mop_templ_1(m);
    } else {
        MopTemplate1 m;
        m.loop_outer = 1;
        m.loop_inner = num_fidelity_phases;
        m.loop_op0 = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 3, INDEX_DIS, 0));
        m.last_inner_op = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0));
        m.last_outer_op = TT_OP(GAPOOL(CLR_NONE, DIM_16X16, 0, INDEX_DIS, 0));
        set_mop_templ_1(m);
    } 
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

