
#include <cstdint>
#include <cassert>

#include "core/template.hpp"
#include "core/instr_params.hpp"
#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/unpack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

using core::p_unpacr;
using core::p_unpacr_nop;
using core::p_stall;

using core::MopTemplate0;
using core::DataFormat;

//
//    Unpack
//

void Unpack::unpack_A_hw_configure_disaggregated(
        BroadcastType BType, 
        bool transpose_xy, 
        bool transpose_xy_srca, 
        bool acc_to_dest, 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format) {
    assert(!(acc_to_dest && BType != BroadcastType::NONE));
    assert(!(acc_to_dest && transpose_xy));
    assert(!(transpose_xy && BType != BroadcastType::NONE));
    assert(!(transpose_xy_srca && BType != BroadcastType::NONE));

    bool is_row_pool = false;

    uint32_t srca_height = 0;
    uint32_t srcb_height = 0;

    if (BType == BroadcastType::SCALAR || BType == BroadcastType::ROW) {
        srca_height = 1;
        srcb_height = 1;
    } else if (BType == BroadcastType::COL || acc_to_dest) {
        srca_height = 0; // TODO: Figure out how to handle zero value
        srcb_height = 16;
    } else {
        srca_height = 16;
        srcb_height = 16;
    } 

    configure_unpack_AB(
        unpa_src_format,
        unpa_dst_format,
        unpa_src_format,
        unpa_dst_format,
        srca_height,
        srcb_height,
        is_row_pool,
        transpose_xy_srca,
        is_fp32_dest_acc_en);
}

void Unpack::unpack_A_init(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest,
        uint32_t within_face_16x16_transpose) {
    unpack_A_mop_config(BType, transpose_xy, acc_to_dest);
    STALLWAIT(p_stall::STALL_THCON, p_stall::UNPACK0);
    set_haloize_mode(within_face_16x16_transpose);
}

void Unpack::unpack_A(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest,
        uint32_t address) {
    // Clear z/w start counters
    SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    start_unpack_A(BType, acc_to_dest, address);

    // Stall unpacker until pending CFG writes from Trisc have completed
    STALLWAIT(p_stall::STALL_UNPACK, p_stall::TRISC_CFG);

    // Run MOP
     if (BType == BroadcastType::ROW || transpose_xy) {
        mop_run(2);
    } else if (BType == BroadcastType::SCALAR || BType == BroadcastType::COL) {
        mop_run(1);
    } else {
        mop_run(4);
    }

    end_unpack_A();
}

void Unpack::unpack_A_mop_config(
        BroadcastType BType, 
        bool transpose_xy, 
        bool acc_to_dest) {
    assert(!(acc_to_dest && BType != BroadcastType::NONE));
    assert(!(acc_to_dest && transpose_xy));
    assert(!(transpose_xy && BType != BroadcastType::NONE));

    if (acc_to_dest) {
        auto unpack_srca =
            TT_OP(UNPACR_NOP(
                p_unpacr_nop::UNP0, p_unpacr_nop::UNP_ZEROSRC | p_unpacr_nop::UNP_SET_DVALID));
        auto unpack_srcb =
            TT_OP(UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        MopTemplate0 m;
        m.unpack_b = true;
        m.unpack_halo = false;
        m.a0_instr = unpack_srca;
        m.b_instr = unpack_srcb;
        set_mop_templ_0(m);
    } else if (BType == BroadcastType::COL) {
        auto unpack_srca =
            TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        auto unpack_srcb =
            TT_OP(UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        auto unpack_srcb_set_z = TT_OP(SETADCZW(0b010, 0, 0, 0, 2, 0b0001));
        MopTemplate0 m;
        m.unpack_b = false;
        m.unpack_halo = true;
        m.a0_instr = unpack_srcb;
        m.a1_instr = unpack_srcb_set_z;
        m.a2_instr = unpack_srcb;
        m.a3_instr = unpack_srca;
        set_mop_templ_0(m);
    } else if (BType == BroadcastType::ROW) {
        auto unpack_srcb =
            TT_OP(UNPACR(SrcB, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        auto unpack_srcb_clear_z = TT_OP(SETADCZW(0b010, 0, 0, 0, 0, 0b0001));
        MopTemplate0 m;
        m.unpack_b = false;
        m.unpack_halo = true;
        m.a0_instr = unpack_srcb;
        m.a1_instr = unpack_srcb;
        m.a2_instr = unpack_srcb_clear_z;
        m.a3_instr = TT_OP(NOP());
        set_mop_templ_0(m);
    } else if (BType == BroadcastType::SCALAR) {
        auto unpack_srcb =
            TT_OP(UNPACR(SrcB, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        // ckernel_unpack_template::lB
        MopTemplate0 m;
        m.unpack_b = false;
        m.unpack_halo = true;
        m.a0_instr = unpack_srcb;
        set_mop_templ_0(m);
    } else {
        if (transpose_xy) {
            auto unpack_srca_set_z = TT_OP(SETADCZW(0b001, 0, 0, 0, 1, 0b0001));
            auto unpack_srca =
                TT_OP(UNPACR(SrcA, 0b01000010, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
            auto unpack_srcb = 
                TT_OP(UNPACR_NOP(
                    p_unpacr_nop::UNP1, p_unpacr_nop::UNP_ZEROSRC | p_unpacr_nop::UNP_SET_DVALID));
            MopTemplate0 m;
            m.unpack_b = true;
            m.unpack_halo = true;
            m.a0_instr = unpack_srca;
            m.a1_instr = unpack_srcb;
            m.a2_instr = unpack_srca;
            m.a3_instr = unpack_srcb;
            m.b_instr = unpack_srca_set_z;
            set_mop_templ_0(m);
        } else {
            auto unpack_srca =
                TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
            auto unpack_srcb = 
                TT_OP(UNPACR_NOP(
                    p_unpacr_nop::UNP1, p_unpacr_nop::UNP_ZEROSRC | p_unpacr_nop::UNP_SET_DVALID));
            MopTemplate0 m;
            m.unpack_b = true;
            m.unpack_halo = false;
            m.a0_instr = unpack_srca;
            m.b_instr = unpack_srcb;
            set_mop_templ_0(m);
        }
    }
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

