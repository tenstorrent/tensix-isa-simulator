
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

void Unpack::unpack_tilize_hw_configure_disaggregated(
        DataFormat unpa_src_format, DataFormat unpa_dst_format) {
    configure_unpack_AB(
        unpa_src_format, 
        unpa_dst_format,
        unpa_src_format, 
        unpa_dst_format,
        16,
        16,
        false,
        false,
        false);
}

void Unpack::unpack_tilize_init(
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        uint32_t unpA_block_c_tiles) {
    unpack_tilize_mop_config();

    // SKIPPED: wait_for_idle

    // Override default settings
    set_tileize_mode(1);
    // shift between adjacent row blocks in 16B words
    set_shift_amount(((unpA_block_c_tiles << 5) * get_df_item_size(unpa_src_format)) >> 4);
    set_tile_x_dim(16);
}

void Unpack::unpack_tilize_block(
        DataFormat unpa_src_format, 
        DataFormat unpa_dst_format,
        uint32_t base_addr, 
        uint32_t block_c_tiles) {
    for (uint32_t tile_index = 0; tile_index < block_c_tiles; tile_index++) {
        uint32_t item_size = get_df_item_size(unpa_src_format);

        // Each iteration unpacks 2 16x16 faces (1st 0,1 2nd 2,3)
        // Offset address is in 16B words
        // Datum count = tile_index * 32 ( / 16 to get word count)
        uint32_t top_face_offset_addr = (tile_index * item_size) << 1;
        
        // * 32 rows / 16 to get 16B word aligned address
        uint32_t bot_face_offset_addr = (block_c_tiles << 5) * item_size;

        // Program srcA and srcB base addresses
        for (uint32_t n = 0; n < 2; n++) {
            uint32_t addr = 
                base_addr + top_face_offset_addr + ((n == 1) ? bot_face_offset_addr : 0);

            // Clear z/w start counters
            SETADCZW(0b001, 0, 0, 0, 0, 0b1111);

            start_unpack_A(BroadcastType::NONE, false, addr);

            // Stall unpacker until pending CFG writes from Trisc have completed
            STALLWAIT(p_stall::STALL_UNPACK, p_stall::TRISC_CFG);

            // Run MOP
            mop_run(2);

            end_unpack_A();
        }
    } 
}

void Unpack::unpack_tilize_uninit() {
    // Undo tilize if added

    // SKIPPED: wait_for_idle

    set_tileize_mode(0);
    set_shift_amount(0);
    set_tile_x_dim(256);
}

void Unpack::unpack_tilize_mop_config() {
    auto unpack_srca =
        TT_OP(UNPACR(SrcA, 0b1, 0, 0, 0, 1, 1, p_unpacr::RAREFYB_DISABLE, 0, 0, 0, 0, 1));
    auto unpack_srcb = 
        TT_OP(UNPACR_NOP(
            p_unpacr_nop::UNP1, p_unpacr_nop::UNP_ZEROSRC | p_unpacr_nop::UNP_SET_DVALID));
    // ckernel_unpack_template::lBA
    MopTemplate0 m;
    m.unpack_b = true;
    m.unpack_halo = false;
    m.a0_instr = unpack_srca;
    m.b_instr = unpack_srcb;
    set_mop_templ_0(m);
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

