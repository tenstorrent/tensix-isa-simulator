// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

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

using core::MopTemplate1;
using core::DataFormat;

//
//    Unpack
//

void Unpack::unpack_AB_matmul_hw_configure_disaggregated( 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format,
        DataFormat unpa_dst_format,
        DataFormat unpb_src_format,
        DataFormat unpb_dst_format,
        uint32_t transpose_xy_srca) {
    uint32_t srca_height = 16;
    uint32_t srcb_height = 16;
    bool is_row_pool = false;
    configure_unpack_AB(
        unpa_src_format,
        unpa_dst_format,
        unpb_src_format,
        unpb_dst_format,
        srca_height, 
        srcb_height, 
        is_row_pool, 
        transpose_xy_srca, 
        is_fp32_dest_acc_en); 
}

void Unpack::unpack_AB_matmul_init(uint32_t transpose) { 
    unpack_AB_matmul_mop_config(transpose != 0); 
} 

void Unpack::unpack_AB_matmul(uint32_t address_a, uint32_t address_b) {
    // Clear z/w start counters
    SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    // note inverted address order
    // (MVMUL computes srcB * SrcA, therefore address_a is unpacked to SrcB)
    start_unpack_AB(address_b, address_a);

    // Run MOP
    MOP(1, 0, 0);

    end_unpack_AB();
} 

void Unpack::unpack_AB_matmul_mop_config(bool transpose) {
    static constexpr uint32_t RAREFYB_DISABLE = p_unpacr::RAREFYB_DISABLE;

    // Unpacks all 4 faces into SrcA and SrcB to get [32 x 32] matrix
    // Sets Dvalid only after full unpack is completed
    // Column major order requires reading faces 0, 2, 1, 3
    // This is achieved by setting CH0_Z_INC = 2 and
    //     using SETADCZW after unpacking two faces
    // TODO: Figure out meaning of CH1_Z_INC = 1
    //     (does CH1_Z_INC default to CH0_Z_INC?)
    // Note that SrcA contains *second* operand

    // UNPACR
    //     addr_mode = 0b1           CH0_Z_INC = 1
    //     addr_mode = 0b00010010    CH0_Z_INC = 2, CH1_Z_INC = 1

    // SrcB will be converted from row major to column major
    auto unpack_srcb_set_dvalid =
        TT_OP(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

    if (transpose) {
        // SrcA unpacked as column major layout, follows src B
        auto unpack_srca_set_dvalid =
            TT_OP(UNPACR(SrcA, 0b00010010, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

        uint32_t replay_buf_len = 7;
        REPLAY(0, replay_buf_len, 0, 1);
        // Don't set Dvalid
        TT_RUN(UNPACR(SrcA, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcA, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(SETADCZW(0b011, 0, 0, 0, 1, 0b0001)); // UNPACK SRC A and B Z 0,2,1,3
        TT_RUN(UNPACR(SrcA, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

        MopTemplate1 m;
        m.loop_outer = 1;
        m.loop_inner = 1;
        m.loop_op0 = TT_OP(REPLAY(0, replay_buf_len, 0, 0));
        m.end_op0 = unpack_srca_set_dvalid;
        m.end_op1 = unpack_srcb_set_dvalid;
        set_mop_templ_1(m);
    } else {
#if 0 // TODO: Revise this
        // SrcA unpacked as row major layout
        auto unpack_srca_set_dvalid =
            TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

        uint32_t replay_buf_len = 7;
        REPLAY(0, replay_buf_len, 0, 1);
        // Don't set Dvalid
        TT_RUN(UNPACR(SrcA,        0b1, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcA,        0b1, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(SETADCZW(0b010, 0, 0, 0, 1, 0b0001)); // UNPACK SRCB Z 0,2,1,3
        TT_RUN(UNPACR(SrcA,        0b1, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
#endif
        // SrcA unpacked as row major layout
        auto unpack_srca_set_dvalid =
            TT_OP(UNPACR(SrcA, 0b00010001, 0, 0, 0, 1, 1, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

        uint32_t replay_buf_len = 7;
        REPLAY(0, replay_buf_len, 0, 1);
        // Don't set Dvalid
        TT_RUN(UNPACR(SrcA, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcA, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(SETADCZW(0b010, 0, 0, 0, 1, 0b0001)); // UNPACK SRCB Z 0,2,1,3
        TT_RUN(UNPACR(SrcA, 0b00010001, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));
        TT_RUN(UNPACR(SrcB, 0b00010010, 0, 0, 0, 1, 0, RAREFYB_DISABLE, 0, 0, 0, 0, 1));

        MopTemplate1 m;
        m.loop_outer = 1;
        m.loop_inner = 1;
        m.loop_op0 = TT_OP(REPLAY(0, replay_buf_len, 0, 0));
        m.end_op0 = unpack_srca_set_dvalid;
        m.end_op1 = unpack_srcb_set_dvalid;
        set_mop_templ_1(m);
    }
} 

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

