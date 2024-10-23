
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

using core::MopTemplate0;

using core::p_unpacr;
using core::p_unpacr_nop;
using core::p_stall;

//
//    Unpack
//

void Unpack::unpack_reduce_hw_configure_disaggregated(
        PoolType type, 
        ReduceDim dim, 
        bool is_fp32_dest_acc_en,
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        float mult) {
    uint32_t srca_height = 16;
    uint32_t srcb_height = 16;
    bool is_row_pool = true;
    bool transpose_xy_per_face = (dim == ReduceDim::REDUCE_ROW);

    configure_unpack_AB(
        unpa_src_format, 
        unpa_dst_format,
        unpa_src_format, 
        unpa_dst_format,
        srca_height,
        srcb_height,
        is_row_pool,
        transpose_xy_per_face,
        is_fp32_dest_acc_en); 

    union {
        float f;
        uint32_t u;
    } f2u;
    f2u.f = mult;

    // Load const into L1 buffer
    uint32_t *l1_buffer = reinterpret_cast<uint32_t *>(map_addr(m_l1_buffer_addr));
    for (int i = 0; i < 16; i++) {
        l1_buffer[i] = f2u.u;
    }

    // Unpacker is programmed to automatically skip tile header (+1) 
    // Since address_b points to tile content, we need to -1 address
    // (in terms of 16B words) to offet unpacker's automatic +1.
    set_base_addr(1, 0, (m_l1_buffer_addr >> 4) - 1);
}

void Unpack::unpack_reduce_init(PoolType type, ReduceDim dim) {
    unpack_reduce_mop_config(type, dim); 
}

void Unpack::unpack_reduce(PoolType type, ReduceDim dim, uint32_t addr) {
    // Clear z/w start counters
    SETADCZW(0b011, 0, 0, 0, 0, 0b1111);

    start_unpack_A(BroadcastType::NONE, false, addr);

    // Stall unpacker until pending CFG writes from Trisc have completed
    STALLWAIT(p_stall::STALL_UNPACK, p_stall::TRISC_CFG);

    // Run MOP
    mop_run(4);

    end_unpack_A();
}

void Unpack::unpack_reduce_mop_config(PoolType type, ReduceDim dim) {
    auto unpack_srca =
        TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
    auto unpack_zerosrca = TT_OP(UNPACR_NOP(p_unpacr_nop::UNP0, p_unpacr_nop::UNP_ZEROSRC));
    auto unpack_srcb =
        TT_OP(UNPACR(SrcB, 0b0, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
    MopTemplate0 m;
    m.unpack_b = true;
    m.unpack_halo = true;
    m.a0_instr = unpack_zerosrca;
    m.a1_instr = unpack_srca;
    m.a2_instr = TT_OP(NOP());
    m.a3_instr = TT_OP(NOP());
    m.b_instr = unpack_srcb;
    set_mop_templ_0(m);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

