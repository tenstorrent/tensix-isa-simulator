// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/tensix_types.hpp"
#include "core/tensix.hpp"

//
//    ACHTUNG: Once full support for Tensix configuration registers is available,
//        functions in this section can be moved to "llk/api" level.
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Tensix
//

// CFG primitives

void Tensix::set_addr_mod_base(uint32_t base) {
    // Emulate setting control register ADDR_MOD_SET_Base
    // TODO: Figure out how this register inteferes with bias set via ADDR_MOD
    m_bias = (base != 0) ? 1 : 0;
}

void Tensix::set_addr_mod(int index, const AddrMod &addr_mod) {
    //
    //    Emulate setting control registers
    //
    //    ADDR_MOD_AB_SEC[0-7]_SrcAIncr
    //    ADDR_MOD_DST_SEC[0-7]_DestIncr
    //    ADDR_MOD_BIAS_SEC[0-7]_BiasIncr
    //
    assert(index >= 0 && index < 8);
    m_addr_mod[index] = addr_mod;
}

void Tensix::set_addr_mod_pack(int index, const AddrModPack &addr_mod_pack) {
    //
    //    Emulate setting control registers
    //
    //    ADDR_MOD_PACK_SEC[0-7]_YsrcIncr
    //
    assert(index >= 0 && index < 8);
    m_addr_mod_pack[index] = addr_mod_pack;
}

void Tensix::set_dest_offset(uint32_t dst_index) {
    // Emulate setting control register DEST_TARGET_REG_CFG_MATH_Offset
    // On real hardware SETC16 must be preceded by
    //     STALLWAIT(p_stall::STALL_CFG, p_stall::MATH|p_stall::WAIT_SFPU)
    m_dest_offset = dst_index;
}

void Tensix::set_tile_descriptor(
        uint32_t unp_id,
        DataFormat in_data_format,
        uint32_t x_dim,
        uint32_t y_dim,
        uint32_t z_dim) {
    // Emulate setting THCON_SEC[0|1]_REG0_TileDescriptor
    assert(unp_id == 0 || unp_id == 1);
    AddrCtrl &adc = m_adc[unp_id];
    uint32_t y_stride = x_dim;
    uint32_t z_stride = y_dim * y_stride;
    uint32_t w_stride = z_dim * z_stride;
    uint32_t item_size = get_df_item_size(in_data_format);
    adc.tile_in_data_format = in_data_format;
    adc.tile_x_dim = x_dim;
    adc.tile_y_dim = y_dim;
    adc.tile_z_dim = z_dim;
    // derived parameters
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
    }
}

void Tensix::set_haloize_mode(uint32_t haloize_mode) {
    // Emulate setting control register THCON_SEC0_REG2_Haloize_mode
    // TODO: Implement support for this parameter
    m_haloize_mode = haloize_mode;
}

void Tensix::set_tileize_mode(uint32_t tileize_mode) {
    // Emulate setting control register THCON_SEC0_REG2_Tileize_mode
    m_tileize_mode = tileize_mode;
}

void Tensix::set_shift_amount(uint32_t shift_amount) {
    // Emulate setting control register THCON_SEC0_REG2_Shift_amount
    m_shift_amount = shift_amount;
}

void Tensix::set_tile_x_dim(uint32_t tile_x_dim) {
    // Emulate setting control registers THCON_SEC0_REG5_Tile_x_dim_cntx[0|1]
    // Generally two separate values for cntx[0|1] but set equal in actual LLKs,
    // hence only one value in this release.
    m_tile_x_dim = tile_x_dim;

    // compute ovrd strides (only unpack0)
    AddrCtrl &adc = m_adc[0];
    DataFormat in_data_format = adc.tile_in_data_format;
    uint32_t x_dim = tile_x_dim;
    uint32_t y_dim = adc.tile_y_dim;
    uint32_t z_dim = adc.tile_z_dim;
    uint32_t y_stride = x_dim;
    uint32_t z_stride = y_dim * y_stride;
    uint32_t w_stride = z_dim * z_stride;
    uint32_t item_size = get_df_item_size(in_data_format);
    m_ovrd_ch0_x_stride = item_size;
    m_ovrd_ch0_y_stride = y_stride * item_size;
    m_ovrd_ch0_z_stride = z_stride * item_size;
    m_ovrd_ch0_w_stride = w_stride * item_size;
    // ch1 strides do not depend on x_dim and are not affected
}

void Tensix::set_unp_cntx_offset(uint32_t unp0, uint32_t unp1) {
    // Emulate setting control registers UNPACK_MISC_CFG_CfgContextOffset_[0|1]
    m_adc[0].unp_cntx_offset = unp0;
    m_adc[1].unp_cntx_offset = unp1;
}

//
//    ACHTUNG: Base addresses are expressed in 16B words
//        Unpacker is programmed to automatically skip tile header (+1) 
//        In case there is no tile header, caller needs to -1 address (in terms of 16B words)
//        to offet unpacker's automatic +1.
//

void Tensix::set_base_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr) {
    // Emulate setting THCON_SEC[0|1]_REG3{_cntx[1-7]}_Base_address
    assert(unp_id == 0 || unp_id == 1);
    assert(cntx_id < 8);
    m_adc[unp_id].cntx[cntx_id].base_addr = addr;
}

void Tensix::set_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t addr) {
    // Emulate setting THCON_SEC[0|1]_REG7_Offset{_cntx[1-3]}_address
    assert(unp_id == 0 || unp_id == 1);
    assert(cntx_id < 4);
    m_adc[unp_id].cntx[cntx_id].offset_addr = addr;
}

void Tensix::incr_offset_addr(uint32_t unp_id, uint32_t cntx_id, uint32_t incr) {
    // Emulate incrementing THCON_SEC[0|1]_REG7_Offset{_cntx[1-3]}_address
    assert(unp_id == 0 || unp_id == 1);
    assert(cntx_id < 4);
    m_adc[unp_id].cntx[cntx_id].offset_addr += incr;
}

void Tensix::set_pack_fp32_dest(bool en) {
    // Emulate setting PCK_DEST_RD_CTRL_Read_32b_data
    m_pack_fp32_dest = en;
}

void Tensix::set_pack_config(
        DataFormat src_format, 
        DataFormat dst_format, 
        uint32_t x_dim, 
        uint32_t y_dim, 
        uint32_t z_dim) {
    //
    //    Emulate setting config registers
    //
    //    PCK0_ADDR_CTRL_XY_REG_0_Xstride
    //    PCK0_ADDR_CTRL_ZW_REG_0_Zstride
    //    THCON_SEC[0|1]_REG[1|8]_Row_start_section_size
    //
    m_pack_src_format = src_format; 
    m_pack_dst_format = dst_format;
    m_pack_x_dim = x_dim; 
    m_pack_y_dim = y_dim; 
    m_pack_z_dim = z_dim;

    // derived parameters
    uint32_t item_size = get_df_item_size(m_pack_dst_format);
    m_pack_ch0_y_stride = m_pack_x_dim;
    m_pack_ch0_z_stride = m_pack_y_dim * m_pack_ch0_y_stride;
    m_pack_ch0_w_stride = m_pack_z_dim * m_pack_ch0_z_stride;
    m_pack_ch1_x_stride = item_size;
    m_pack_ch1_y_stride = m_pack_ch0_y_stride * item_size;
    m_pack_ch1_z_stride = m_pack_ch0_z_stride * item_size;
    m_pack_ch1_w_stride = m_pack_ch0_w_stride * item_size;

    //
    //    Emulate setting config registers
    //    (use one increment instead of three separate offsets)
    //
    //    THCON_SEC0_REG8_L1_Dest_addr
    //    THCON_SEC1_REG1_L1_Dest_addr
    //    THCON_SEC1_REG8_L1_Dest_addr
    //
    m_pack_l1_offset = 16 * item_size;
}

void Tensix::set_pack_edge_offset_mask(int index, uint32_t value) {
    // Emulate setting PCK_EDGE_OFFSET_SEC[0-3]_mask (partial for SEC0) 
    assert(index >= 0 && index < 4);
    m_pack_edge_offset_mask[index] = value;
}

void Tensix::set_pack_edge_row_set_select(int index, uint32_t value) {
    // Emulate setting PCK_EDGE_OFFSET_SEC0_mask (partial)
    assert(index >= 0 && index < 4);
    m_pack_edge_row_set_select[index] = value;
}

void Tensix::set_tile_row_set_mapping(int index, uint32_t value) {
    // emulate setting TILE_ROW_SET_MAPPING_[0-3]_row_set_mapping
    assert(index >= 0 && index < 4);
    m_tile_row_set_mapping[index] = value;
}

void Tensix::set_relu_config(uint32_t relu_type, uint32_t relu_threshold) {
    // emulate setting STACC_RELU_ApplyRelu
    m_relu_type = relu_type;
    m_relu_threshold = relu_threshold;
}

void Tensix::set_pack_dest_offset(
        uint32_t offset0, 
        uint32_t offset1, 
        uint32_t offset2, 
        uint32_t offset3) {
    // emulate setting DEST_TARGET_REG_CFG_PACK_SEC[0-3]_Offset
    m_pack_dest_offset[0] = offset0;
    m_pack_dest_offset[1] = offset1;
    m_pack_dest_offset[2] = offset2;
    m_pack_dest_offset[3] = offset3;
}

void Tensix::set_pack_dest_addr(uint32_t addr) {
    // emulate setting THCON_SEC0_REG1_L1_Dest_addr
    m_pack_dest_addr = addr;
}

void Tensix::set_pack_l1_acc(bool en) {
    // emulate setting THCON_SEC[0|1]_REG[1|8]_Pack_L1_Acc
    m_pack_l1_acc = en;
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

