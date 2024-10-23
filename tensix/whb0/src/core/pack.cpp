
#include <cstdint>
#include <cassert>

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

void pack_float32(Memory *memory, uint32_t dst, const float *src, uint32_t count) {
    assert(dst + count * 4 <= memory->size());
    float *ptr = reinterpret_cast<float *>(memory->map_addr(dst));
    for (uint32_t i = 0; i < count; i++) {
        ptr[i] = src[i];
    }
}

void pack_float16b(Memory *memory, uint32_t dst, const float *src, uint32_t count) {
    assert(dst + count * 2 <= memory->size());
    U32 u32;
    uint16_t *ptr = reinterpret_cast<uint16_t *>(memory->map_addr(dst));
    for (uint32_t i = 0; i < count; i++) {
        u32.f = src[i];
        ptr[i] = uint16_t(u32.i >> 16);
    }
}

void pack_em_float32(Memory *memory, uint32_t mask, uint32_t dst, const float *src) {
    assert(dst + 16 * 4 <= memory->size());
    float *ptr = reinterpret_cast<float *>(memory->map_addr(dst));
    for (uint32_t i = 0; i < 16; i++) {
        if ((mask & 0x1) != 0) {
            ptr[i] = src[i];
        }
        mask >>= 1;
    }
}

void pack_em_float16b(Memory *memory, uint32_t mask, uint32_t dst, const float *src) {
    assert(dst + 16 * 2 <= memory->size());
    U32 u32;
    uint16_t *ptr = reinterpret_cast<uint16_t *>(memory->map_addr(dst));
    for (uint32_t i = 0; i < 16; i++) {
        if ((mask & 0x1) != 0) {
            u32.f = src[i];
            ptr[i] = uint16_t(u32.i >> 16);
        }
        mask >>= 1;
    }
}

void pack_update_rwc(
        uint32_t incr,
        uint32_t clr,
        uint32_t cr,
        uint32_t &rwc,
        uint32_t &rwc_cr) {
    // common case
    if (cr == 0) {
        // update Counter only
        if (clr != 0) {
            rwc = rwc_cr;
        }
        rwc += incr;
    } else {
        // update Counter_CR
        if (clr != 0) {
            rwc_cr = 0;
        }
        rwc_cr += incr;
        // also reset Counter
        rwc = rwc_cr;
    }
}

void pack_update_rwc_nocr(uint32_t incr, uint32_t clr, uint32_t &rwc) {
    if (clr != 0) {
        rwc = 0;
    }
    rwc += incr;
}

} // namespace

//
//    Tensix
//

//
//    PACR: Pack row from DST registers to L0
//
//    addr_mode         2-bit index of cfg register ADDR_MOD_REG to use
//                      for describing addressing mode 
//    zero_write        Will cause zeros to be written regardless of source data
//    pack_sel          Bit mask to select up to 4 packers. When 'pack_sel' is 0 then
//                      packer 0 will be selected by default
//    ovrd_thread_id    Use context ID instead of thread ID to select packer configuration.
//                      context id is set in the packer configuration register
//                      (What register? [-A.G.])
//    concat            To be set to 1 when packed data needs to be concatenated to the output
//                      of the previous PACR instruction. Must be set to 1 for
//                      the first pack instruction and 0 for the last PACR in a row
//    flush             When this bit is set the PACR instruction will just cause
//                      a flush of non-empty WAs in the packer
//    last              To be set to 0 for all invocations of PACR except for
//                      the last in a tile, when it should be set to 1
//

void Tensix::PACR(
        uint32_t addr_mode,
        uint32_t zero_write,
        uint32_t pack_sel,
        uint32_t ovrd_thread_id,
        uint32_t concat,
        uint32_t flush,
        uint32_t last) {
    pack_update_strides();

    if (flush != 0) {
        pack_update_counters(addr_mode);
        return;
    }

    AddrCtrl &adc = m_adc[2];

    // ACHTUNG: This is minimalistic implementation (single config context)
    //     It should be sufficient for supporting available LLKs

    CfgCntx &cntx = adc.cntx[0];

    // TODO: Figure out where in original code 1 is subtracted
    // +1 to skip header and convert 16B words into bytes
    uint32_t base_addr = (m_pack_dest_addr + 1) << 4;

    uint32_t src_addr = 
        // no X component
        adc.ch0_y * m_pack_ch0_y_stride + 
        adc.ch0_z * m_pack_ch0_z_stride + 
        adc.ch0_w * m_pack_ch0_w_stride;
    uint32_t dst_addr = 
        base_addr + 
            adc.ch1_x * m_pack_ch1_x_stride + 
            adc.ch1_y * m_pack_ch1_y_stride + 
            adc.ch1_z * m_pack_ch1_z_stride + 
            adc.ch1_w * m_pack_ch1_w_stride;
    uint32_t count = adc.x_end - adc.ch0_x + 1;

    uint32_t sel_mask = (pack_sel != 0) ? pack_sel : 1;
    for (uint32_t packer = 0; packer < 4; packer++) { 
        if ((sel_mask & (1 << packer)) == 0) {
            continue;
        }
        // TODO: Support 'zero_write'
        // do wrap around here?
        uint32_t curr_src_addr = src_addr + m_pack_dest_offset[packer] * ROW_SIZE;
        // will produce gaps in L1 for disabled packers: is this correct?
        uint32_t curr_dst_addr = dst_addr + packer * m_pack_l1_offset * ROW_SIZE;
        assert(curr_src_addr + count <= DEST_ROWS * ROW_SIZE);
        pack_row(packer, curr_dst_addr, m_dest[0] + curr_src_addr, count, m_pack_dst_format);
    }

    pack_update_counters(addr_mode);
}

// implementation

void Tensix::pack_update_strides() {
    if (!m_pack_update_strides) {
        return;
    }
    uint32_t item_size = get_df_item_size(m_pack_src_format);
    m_pack_ch0_y_stride = m_pack_ch1_y_stride / item_size;
    m_pack_ch0_z_stride = m_pack_ch1_z_stride / item_size;
    m_pack_ch0_w_stride = m_pack_ch1_w_stride / item_size;
#if 0 // TODO: Revise this (m_pack_*_dim are unused and deprecated)
    m_pack_x_dim = m_pack_ch0_y_stride;
    m_pack_y_dim = m_pack_ch0_z_stride / m_pack_ch0_y_stride;
    m_pack_z_dim = m_pack_ch0_w_stride / m_pack_ch0_z_stride;
#endif
    m_pack_update_strides = false;
}

void Tensix::pack_row(
        uint32_t packer, 
        uint32_t dst, 
        const float *src, 
        uint32_t count, 
        DataFormat df) {
    assert(count % 16 == 0);
    uint32_t row_count = count / 16;
    uint32_t dst_incr = 16 * get_df_item_size(df);
    uint32_t s = m_pack_edge_row_set_select[packer];
    uint32_t rm = m_tile_row_set_mapping[s];
    for (uint32_t row = 0; row < row_count; row++) {
        uint32_t em = m_pack_edge_offset_mask[rm & 0x3];
        if (em == 0xffff) {
            pack(dst, src, 16, df); 
        } else if (em != 0) {
            pack_em(em, dst, src, df); 
        }
        dst += dst_incr;
        src += 16;
        rm >>= 2; 
    }
}

void Tensix::pack(uint32_t dst, const float *src, uint32_t count, DataFormat df) {
    switch (df) {
    case DataFormat::Float32:
        pack_float32(m_memory, dst, src, count);
        break;
    case DataFormat::Float16_b:
        pack_float16b(m_memory, dst, src, count);
        break;
    default:
        // TODO: Implement other formats
        assert(false);
        break;
    }
}

void Tensix::pack_em(uint32_t mask, uint32_t dst, const float *src, DataFormat df) {
    switch (df) {
    case DataFormat::Float32:
        pack_em_float32(m_memory, mask, dst, src);
        break;
    case DataFormat::Float16_b:
        pack_em_float16b(m_memory, mask, dst, src);
        break;
    default:
        // TODO: Implement other formats
        assert(false);
        break;
    }
}

void Tensix::pack_update_counters(uint32_t addr_mode) {
    AddrCtrl &adc = m_adc[2];
    assert(addr_mode >= 0 && addr_mode < 4);
    AddrModPack &m = m_addr_mod_pack[addr_mode];
    auto &y_src = m.y_src; 
    pack_update_rwc(
        y_src.incr,
        y_src.clr,
        y_src.cr,
        adc.ch0_y,
        adc.ch0_y_cr);
    auto &y_dst = m.y_dst; 
    pack_update_rwc(
        y_dst.incr,
        y_dst.clr,
        y_dst.cr,
        adc.ch1_y,
        adc.ch1_y_cr);
    auto &z_src = m.z_src; 
    pack_update_rwc_nocr(z_src.incr, z_src.clr, adc.ch0_z);
    auto &z_dst = m.z_dst; 
    pack_update_rwc_nocr(z_dst.incr, z_dst.clr, adc.ch1_z);
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

