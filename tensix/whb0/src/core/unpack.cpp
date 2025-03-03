// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cassert>

#include "core/instr_params.hpp"
#include "core/tensix_types.hpp"
#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

namespace {

union U32 {
    float f;
    uint32_t i;
};

void unpack_float32(Memory *memory, float *dst, uint32_t src, uint32_t count) {
    assert(src + count * 4 <= memory->size());
    float *ptr = reinterpret_cast<float *>(memory->map_addr(src));
    for (uint32_t i = 0; i < count; i++) {
        dst[i] = ptr[i];
    }
}

void unpack_float16b(Memory *memory, float *dst, uint32_t src, uint32_t count) {
    assert(src + count * 2 <= memory->size());
    U32 u32;
    uint16_t *ptr = reinterpret_cast<uint16_t *>(memory->map_addr(src));
    for (uint32_t i = 0; i < count; i++) {
        u32.i = uint32_t(ptr[i]) << 16;
        dst[i] = u32.f;
    }
}

void transpose(float *reg, uint32_t count) {
    if (count != 256) {
        // TODO: Figure out whether this is legal and if yes, then how to support it
        assert(false);
        return;
    }
    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < i; k++) {
            int pos1 = i * 16 + k;
            int pos2 = k * 16 + i;
            float temp = reg[pos1];
            reg[pos1] = reg[pos2];
            reg[pos2] = temp;
        }
    }
}

void diag_unpacr_addr(
        uint32_t base_addr, 
        uint32_t offset_addr, 
        uint32_t src_addr, 
        uint32_t dst_addr, 
        int32_t count) {
    printf("UNPACR: base %x offset %x src_addr %x dst_addr %x count %d\n", 
        int(base_addr), int(offset_addr), int(src_addr), int(dst_addr), int(count));
}

} // namespace

//
//    Tensix
//

//
//    UNPACR: Search and unpack row from tile in L0 to SRCA/SRCB registers (by UNPACK0/UNPACK1)
//
//    Terms used in operand descriptions (ACHTUNG: work in progress):
//      - Unpackers 0 and 1 unpack to SrcA and SrcB respectively.
//      - Channels 0 and 1 refer to source (L1) and dest (SrcA/SrcB) respectively
//      - Address counter context contains set of X/Y/Z/W counters for channels 0/1.
//        There are 3 address counter contexts: two for unpackers and one for packer.
//      - Config context contains few values configured via THCON CFG registers.
//        These values include, among others, base L1 addresses for both unpackers
//        (in 16B words). There are up to 8 distinct config contexts but (according to TPM)
//        actual number (as power of 2) is specified via THCON CFG registers.
//        Use of multile config contexts must be enabled by setting 'ovrd_thread_id'
//        operand to 1. This setting enables few other operands including 'cfg_context_id',
//        'auto_inc_context_id', and 'cfg_context_cnt_inc'.
//        Autoincrement wraps around.
//        According to TPM:
//            - 'auto_inc_context_id' overrides 'cfg_context_id'.
//            - autoincrement implementation is buggy
//            - THREAD UNPACK_MISC registers specify context ID offsets for each unpacker
//        
//    unpack_block_selection    Select between two unpackers 
//                                  0 - unpack0
//                                  1 - unpack1
//    addr_mode                 Addressing mode: CR, CH1_Y_INC, CH1_Z_INC, CH0_Y_INC, CH0_Z_INC.
//                              CR=1 bit, INCs are 2 bits
//                                  CH0_Z_INC(1:0)
//                                  CH0_Y_INC(3:2)
//                                  CH1_Z_INC(5:4)
//                                  CH1_Y_INC(7:6)
//                                  CR(8)
//    cfg_context_cnt_inc       Increment config context counter without performing search and unpack
//    cfg_context_id            Config context ID 0..7
//    addr_cnt_context_id       Address counter context ID 0..2
//                                  0 - unpack0
//                                  1 - unpack1
//                                  2 - pack
//                              (See definition of address counter context above.
//                              Note that this interpretation differs from TPM
//                              which specifies this as TRISC ID. TODO: Clarify this.
//                              Usage scenario is not clear. No use cases available.)
//    ovrd_thread_id            Use context ID instead of thread ID to select unpacker configuration
//    set_dat_valid             Unpacker will set data valid bit for the registers
//                              it is unpacking into once data has been written
//    rareb_en                  Will cause srcb rarefy to trigger on uncompress_done
//                                  p_unpacr::RAREFYB_DISABLE
//                                  p_unpacr::RAREFYB_ENABLE
//    zero_write2               Will cause zeros to be written regardless of source data
//    auto_inc_context_id       Auto increment config context ID after every unpack instruction
//    row_search                Search for start and end of selected row(s) within tile.
//                              If not set search is done per datum(s).
//                              (TODO: Figure out what this means)
//    search_cache_flush        Flush row pointer cache in the search block
//    last                      To be set to 0 for all invocations of UNPACR except for
//                              the last in a task of assembling input data for a thread
//                              (in which it should be set to 1).
//                              Causes data accumulation buffers in unpacker to flush  to memory.
//

void Tensix::UNPACR(
        uint32_t unpack_block_selection,
        uint32_t addr_mode,
        uint32_t cfg_context_cnt_inc,
        uint32_t cfg_context_id,
        uint32_t addr_cnt_context_id,
        uint32_t ovrd_thread_id,
        uint32_t set_dat_valid,
        uint32_t rareb_en,
        uint32_t zero_write2,
        uint32_t auto_inc_context_id,
        uint32_t row_search,
        uint32_t search_cache_flush,
        uint32_t last) {
    assert(unpack_block_selection == 0 || unpack_block_selection == 1);

    unpack_update_strides(unpack_block_selection);

    AddrCtrl &adc = m_adc[unpack_block_selection];

    CfgCntx &cntx = adc.cntx[adc.unp_cntx_offset];

    // +1 to skip header and convert 16B words into bytes
    uint32_t base_addr = (cntx.base_addr + 1) << 4;
    uint32_t offset_addr = cntx.offset_addr << 4;
    
    uint32_t ch0_x_stride = adc.ch0_x_stride;
    uint32_t ch0_y_stride = adc.ch0_y_stride;
    uint32_t ch0_z_stride = adc.ch0_z_stride;
    uint32_t ch0_w_stride = adc.ch0_w_stride;
    uint32_t ch1_y_stride = adc.ch1_y_stride;
    uint32_t ch1_z_stride = adc.ch1_z_stride;
    uint32_t ch1_w_stride = adc.ch1_w_stride;
    if (ovrd_thread_id != 0 && unpack_block_selection == 0) {
        // override only for SrcA
        ch0_x_stride = m_ovrd_ch0_x_stride;
        ch0_y_stride = m_ovrd_ch0_y_stride;
        ch0_z_stride = m_ovrd_ch0_z_stride;
        ch0_w_stride = m_ovrd_ch0_w_stride;
        ch1_y_stride = m_ovrd_ch1_y_stride;
        ch1_z_stride = m_ovrd_ch1_z_stride;
        ch1_w_stride = m_ovrd_ch1_w_stride;
    }

    uint32_t src_addr = 
        base_addr + 
            offset_addr +
            adc.ch0_x * ch0_x_stride + 
            adc.ch0_y * ch0_y_stride + 
            adc.ch0_z * ch0_z_stride + 
            adc.ch0_w * ch0_w_stride;
    uint32_t dst_addr = 
        // no X component, according to TPM
        adc.ch1_y * ch1_y_stride + 
        adc.ch1_z * ch1_z_stride + 
        adc.ch1_w * ch1_w_stride;
    uint32_t count = adc.x_end - adc.ch0_x + 1;
#if 0
    diag_unpacr_addr(base_addr, offset_addr, src_addr, dst_addr, count);
#endif

    if (unpack_block_selection == 0) {
        assert(dst_addr + count <= SRCA_ROWS * ROW_SIZE);
        if (m_tileize_mode) {
            unpack_tileized(m_srca[0] + dst_addr, src_addr, count, adc.tile_in_data_format);
        } else {
            unpack(m_srca[0] + dst_addr, src_addr, count, adc.tile_in_data_format);
        }
        if (m_haloize_mode != 0) {
            transpose(m_srca[0] + dst_addr, count);
        }
    } else if (unpack_block_selection == 1) {
        assert(dst_addr + count <= SRCB_ROWS * ROW_SIZE);
        if (m_tileize_mode) {
            unpack_tileized(m_srcb[0] + dst_addr, src_addr, count, adc.tile_in_data_format);
        } else {
            unpack(m_srcb[0] + dst_addr, src_addr, count, adc.tile_in_data_format);
        }
    }

    // TODO: Clarify ch1_z auto-increment (may be set via some config register?)

    uint32_t ch0_z_inc = addr_mode & 0x3;
    uint32_t ch0_y_inc = (addr_mode >> 2) & 0x03;
    uint32_t ch1_z_inc = (addr_mode >> 4) & 0x03;
    uint32_t ch1_y_inc = (addr_mode >> 6) & 0x03;
    adc.ch0_y += ch0_y_inc;
    adc.ch0_z += ch0_z_inc;
    adc.ch1_y += ch1_y_inc;
    adc.ch1_z += ch1_z_inc;
    // according to TPM, CR has no effect

    if (set_dat_valid != 0) {
        if (unpack_block_selection == 0) {
            m_srca_dvalid = true;
        } else if (unpack_block_selection == 1) {
            m_srcb_dvalid = true;
        }
    }
}

//
//    UNPACR_NOP: Trigger side-effect once unpack is done. Skips actual search and unpack.
//
//    unpack_block_selection    Select between two unpackers
//                                  p_unpacr_nop::UNP0
//                                  p_unpacr_nop::UNP1
//    no_op                     Side-effect flags
//                                  p_unpacr_nop::UNP_POP
//                                      freeing up data allocation buffer in noc overlay through
//                                      side band signal. Amount of data to be freed is set
//                                      through NOC_OVERLAY_MSG_CLEAR register.
//                                      Can be optionally combined with stream_id.
//                                  p_unpacr_nop::UNP_NOP
//                                      inject cycle delay between back to back unpack
//                                  p_unpacr_nop::UNP_ZEROSRC
//                                      clear srcA or srcB to 0 when unpack is done
//                                  p_unpacr_nop::UNP_NEGINFSRC
//                                      clear srcA or srcB to neginf when unpack is done
//                                  p_unpacr_nop::UNP_RESET_ALL_BANKS
//                                      clear all banks (if not set, clear current bank)    
//                                  p_unpacr_nop::UNP_STALL_RESET_WR_RDY
//                                      stall clear until write ready is 1 
//                                      (when not set, stall clear until data ready is 0)
//                                  p_unpacr_nop::UNP_SET_DVALID
//                                      set Dvalid on SrcA or SrcB
//

void Tensix::UNPACR_NOP(uint32_t unpack_block_selection, uint32_t no_op) {
    assert(unpack_block_selection == 0 || unpack_block_selection == 1);

    uint32_t no_op_base = no_op & 0x7;

    bool zero_src = (no_op_base == p_unpacr_nop::UNP_ZEROSRC);
    bool neginf_src = (no_op_base == p_unpacr_nop::UNP_NEGINFSRC);
    if (zero_src || neginf_src) {
        float value = neginf_src ? -m_neginf : 0.0f;
        // SrcA / SrcB banking is not simulated, ignore p_unpacr_nop::UNP_RESET_ALL_BANKS
        if (unpack_block_selection == 0) {
            float *ptr = m_srca[0];
            for (uint32_t i = 0; i < SRCA_ROWS * ROW_SIZE; i++) {
                ptr[i] = value;
            }
        } else {
            float *ptr = m_srcb[0];
            for (uint32_t i = 0; i < SRCB_ROWS * ROW_SIZE; i++) {
                ptr[i] = value;
            }
        }
    }

    if (no_op_base == p_unpacr_nop::UNP_SET_DVALID) {
        if (unpack_block_selection == 0) {
            m_srca_dvalid = true;
        } else if (unpack_block_selection == 1) {
            m_srcb_dvalid = true;
        }
    }
}

void Tensix::unpack_update_strides(uint32_t unp_id) {
    AddrCtrl &adc = m_adc[unp_id];
    if (!adc.update_strides) {
        return;
    }

    uint32_t x_dim = adc.tile_x_dim;
    uint32_t y_dim = adc.tile_y_dim;
    uint32_t z_dim = adc.tile_z_dim;
    uint32_t y_stride = x_dim;
    uint32_t z_stride = y_dim * y_stride;
    uint32_t w_stride = z_dim * z_stride;
    uint32_t item_size = get_df_item_size(adc.tile_in_data_format);

    adc.ch0_x_stride = item_size;
    adc.ch0_y_stride = y_stride * item_size;
    adc.ch0_z_stride = z_stride * item_size;
    adc.ch0_w_stride = w_stride * item_size;
    adc.ch1_y_stride = y_stride;
    adc.ch1_z_stride = z_stride;
    adc.ch1_w_stride = w_stride;

    // recompute ovrd strides (only unpack0)
    if (unp_id == 0) {
        y_stride = m_tile_x_dim;
        z_stride = y_dim * y_stride;
        w_stride = z_dim * z_stride;
        m_ovrd_ch0_x_stride = item_size;
        m_ovrd_ch0_y_stride = y_stride * item_size;
        m_ovrd_ch0_z_stride = z_stride * item_size;
        m_ovrd_ch0_w_stride = w_stride * item_size;
        m_ovrd_ch1_y_stride = y_stride;
        m_ovrd_ch1_z_stride = z_stride;
        m_ovrd_ch1_w_stride = w_stride;
    }

    adc.update_strides = false;
}

void Tensix::unpack_tileized(float *dst, uint32_t src, uint32_t count, DataFormat df) {
    assert(count % m_tile_x_dim == 0);
    uint32_t shift_amount = m_shift_amount << 4;
    for (uint32_t i = 0; i < count; i += m_tile_x_dim) {
        unpack(dst + i, src, m_tile_x_dim, df);
        src += shift_amount;
    }
}

void Tensix::unpack(float *dst, uint32_t src, uint32_t count, DataFormat df) {
    switch (df) {
    case DataFormat::Float32:
        unpack_float32(m_memory, dst, src, count);
        break;
    case DataFormat::Float16_b:
        unpack_float16b(m_memory, dst, src, count);
        break;
    default:
        // TODO: Implement other formats
        assert(false);
        break;
    }
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

