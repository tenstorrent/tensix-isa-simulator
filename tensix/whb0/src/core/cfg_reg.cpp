// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "system/cfg_defines.h"

#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Tensix
//

// CFG state update triggers

void Tensix::on_setc16(uint32_t reg) {
    uint32_t value = m_thd_reg[m_thread_id][reg] & 0xffff;
    switch (reg) {
    case 1:
        // DEST_TARGET_REG_CFG_MATH_Offset
        m_dest_offset = value & 0xfff;
        break;
    case 2:
        // ADDR_MOD_SET_Base
        // TODO: Figure out how this register inteferes with bias set via ADDR_MOD
        m_bias = ((value & 0x1) != 0) ? 1 : 0;
        break;
    case 7:
        // ADDR_MOD_AB_SEC0_SrcAIncr
        // ADDR_MOD_AB_SEC0_SrcACR
        // ADDR_MOD_AB_SEC0_SrcAClear
        // ADDR_MOD_AB_SEC0_SrcBIncr
        // ADDR_MOD_AB_SEC0_SrcBCR
        // ADDR_MOD_AB_SEC0_SrcBClear
        set_reg_addr_mod_ab(0, value);
        break;
    case 9:
        // ADDR_MOD_AB_SEC1_SrcAIncr
        // ADDR_MOD_AB_SEC1_SrcACR
        // ADDR_MOD_AB_SEC1_SrcAClear
        // ADDR_MOD_AB_SEC1_SrcBIncr
        // ADDR_MOD_AB_SEC1_SrcBCR
        // ADDR_MOD_AB_SEC1_SrcBClear
        set_reg_addr_mod_ab(1, value);
        break;
    case 11:
        // ADDR_MOD_AB_SEC2_SrcAIncr
        // ADDR_MOD_AB_SEC2_SrcACR
        // ADDR_MOD_AB_SEC2_SrcAClear
        // ADDR_MOD_AB_SEC2_SrcBIncr
        // ADDR_MOD_AB_SEC2_SrcBCR
        // ADDR_MOD_AB_SEC2_SrcBClear
        set_reg_addr_mod_ab(2, value);
        break;
    case 13:
        // ADDR_MOD_AB_SEC3_SrcAIncr
        // ADDR_MOD_AB_SEC3_SrcACR
        // ADDR_MOD_AB_SEC3_SrcAClear
        // ADDR_MOD_AB_SEC3_SrcBIncr
        // ADDR_MOD_AB_SEC3_SrcBCR
        // ADDR_MOD_AB_SEC3_SrcBClear
        set_reg_addr_mod_ab(3, value);
        break;
    case 15:
        // ADDR_MOD_AB_SEC4_SrcAIncr
        // ADDR_MOD_AB_SEC4_SrcACR
        // ADDR_MOD_AB_SEC4_SrcAClear
        // ADDR_MOD_AB_SEC4_SrcBIncr
        // ADDR_MOD_AB_SEC4_SrcBCR
        // ADDR_MOD_AB_SEC4_SrcBClear
        set_reg_addr_mod_ab(4, value);
        break;
    case 17:
        // ADDR_MOD_AB_SEC5_SrcAIncr
        // ADDR_MOD_AB_SEC5_SrcACR
        // ADDR_MOD_AB_SEC5_SrcAClear
        // ADDR_MOD_AB_SEC5_SrcBIncr
        // ADDR_MOD_AB_SEC5_SrcBCR
        // ADDR_MOD_AB_SEC5_SrcBClear
        set_reg_addr_mod_ab(5, value);
        break;
    case 19:
        // ADDR_MOD_AB_SEC6_SrcAIncr
        // ADDR_MOD_AB_SEC6_SrcACR
        // ADDR_MOD_AB_SEC6_SrcAClear
        // ADDR_MOD_AB_SEC6_SrcBIncr
        // ADDR_MOD_AB_SEC6_SrcBCR
        // ADDR_MOD_AB_SEC6_SrcBClear
        set_reg_addr_mod_ab(6, value);
        break;
    case 21:
        // ADDR_MOD_AB_SEC7_SrcAIncr
        // ADDR_MOD_AB_SEC7_SrcACR
        // ADDR_MOD_AB_SEC7_SrcAClear
        // ADDR_MOD_AB_SEC7_SrcBIncr
        // ADDR_MOD_AB_SEC7_SrcBCR
        // ADDR_MOD_AB_SEC7_SrcBClear
        set_reg_addr_mod_ab(7, value);
        break;
    case 23:
        // ADDR_MOD_DST_SEC0_DestIncr
        // ADDR_MOD_DST_SEC0_DestCR
        // ADDR_MOD_DST_SEC0_DestClear
        // ADDR_MOD_DST_SEC0_DestCToCR
        // ADDR_MOD_DST_SEC0_FidelityIncr
        // ADDR_MOD_DST_SEC0_FidelityClear
        set_reg_addr_mod_dst(0, value);
        break;
    case 24:
        // ADDR_MOD_DST_SEC1_DestIncr
        // ADDR_MOD_DST_SEC1_DestCR
        // ADDR_MOD_DST_SEC1_DestClear
        // ADDR_MOD_DST_SEC1_DestCToCR
        // ADDR_MOD_DST_SEC1_FidelityIncr
        // ADDR_MOD_DST_SEC1_FidelityClear
        set_reg_addr_mod_dst(1, value);
        break;
    case 25:
        // ADDR_MOD_DST_SEC2_DestIncr
        // ADDR_MOD_DST_SEC2_DestCR
        // ADDR_MOD_DST_SEC2_DestClear
        // ADDR_MOD_DST_SEC2_DestCToCR
        // ADDR_MOD_DST_SEC2_FidelityIncr
        // ADDR_MOD_DST_SEC2_FidelityClear
        set_reg_addr_mod_dst(2, value);
        break;
    case 26:
        // ADDR_MOD_DST_SEC3_DestIncr
        // ADDR_MOD_DST_SEC3_DestCR
        // ADDR_MOD_DST_SEC3_DestClear
        // ADDR_MOD_DST_SEC3_DestCToCR
        // ADDR_MOD_DST_SEC3_FidelityIncr
        // ADDR_MOD_DST_SEC3_FidelityClear
        set_reg_addr_mod_dst(3, value);
        break;
    case 27:
        // ADDR_MOD_DST_SEC4_DestIncr
        // ADDR_MOD_DST_SEC4_DestCR
        // ADDR_MOD_DST_SEC4_DestClear
        // ADDR_MOD_DST_SEC4_DestCToCR
        // ADDR_MOD_DST_SEC4_FidelityIncr
        // ADDR_MOD_DST_SEC4_FidelityClear
        set_reg_addr_mod_dst(4, value);
        break;
    case 28:
        // ADDR_MOD_DST_SEC5_DestIncr
        // ADDR_MOD_DST_SEC5_DestCR
        // ADDR_MOD_DST_SEC5_DestClear
        // ADDR_MOD_DST_SEC5_DestCToCR
        // ADDR_MOD_DST_SEC5_FidelityIncr
        // ADDR_MOD_DST_SEC5_FidelityClear
        set_reg_addr_mod_dst(5, value);
        break;
    case 29:
        // ADDR_MOD_DST_SEC6_DestIncr
        // ADDR_MOD_DST_SEC6_DestCR
        // ADDR_MOD_DST_SEC6_DestClear
        // ADDR_MOD_DST_SEC6_DestCToCR
        // ADDR_MOD_DST_SEC6_FidelityIncr
        // ADDR_MOD_DST_SEC6_FidelityClear
        set_reg_addr_mod_dst(6, value);
        break;
    case 30:
        // ADDR_MOD_DST_SEC7_DestIncr
        // ADDR_MOD_DST_SEC7_DestCR
        // ADDR_MOD_DST_SEC7_DestClear
        // ADDR_MOD_DST_SEC7_DestCToCR
        // ADDR_MOD_DST_SEC7_FidelityIncr
        // ADDR_MOD_DST_SEC7_FidelityClear
        set_reg_addr_mod_dst(7, value);
        break;
    case 31:
        // ADDR_MOD_PACK_SEC0_YsrcIncr
        // ADDR_MOD_PACK_SEC0_YsrcCR
        // ADDR_MOD_PACK_SEC0_YsrcClear
        // ADDR_MOD_PACK_SEC0_YdstIncr
        // ADDR_MOD_PACK_SEC0_YdstCR
        // ADDR_MOD_PACK_SEC0_YdstClear
        // ADDR_MOD_PACK_SEC0_ZsrcIncr
        // ADDR_MOD_PACK_SEC0_ZsrcClear
        // ADDR_MOD_PACK_SEC0_ZdstIncr
        // ADDR_MOD_PACK_SEC0_ZdstClear
        set_reg_addr_mod_pack(0, value);
        break;
    case 32:
        // ADDR_MOD_PACK_SEC1_YsrcIncr
        // ADDR_MOD_PACK_SEC1_YsrcCR
        // ADDR_MOD_PACK_SEC1_YsrcClear
        // ADDR_MOD_PACK_SEC1_YdstIncr
        // ADDR_MOD_PACK_SEC1_YdstCR
        // ADDR_MOD_PACK_SEC1_YdstClear
        // ADDR_MOD_PACK_SEC1_ZsrcIncr
        // ADDR_MOD_PACK_SEC1_ZsrcClear
        // ADDR_MOD_PACK_SEC1_ZdstIncr
        // ADDR_MOD_PACK_SEC1_ZdstClear
        set_reg_addr_mod_pack(1, value);
        break;
    case 33:
        // ADDR_MOD_PACK_SEC2_YsrcIncr
        // ADDR_MOD_PACK_SEC2_YsrcCR
        // ADDR_MOD_PACK_SEC2_YsrcClear
        // ADDR_MOD_PACK_SEC2_YdstIncr
        // ADDR_MOD_PACK_SEC2_YdstCR
        // ADDR_MOD_PACK_SEC2_YdstClear
        // ADDR_MOD_PACK_SEC2_ZsrcIncr
        // ADDR_MOD_PACK_SEC2_ZsrcClear
        // ADDR_MOD_PACK_SEC2_ZdstIncr
        // ADDR_MOD_PACK_SEC2_ZdstClear
        set_reg_addr_mod_pack(2, value);
        break;
    case 34:
        // ADDR_MOD_PACK_SEC3_YsrcIncr
        // ADDR_MOD_PACK_SEC3_YsrcCR
        // ADDR_MOD_PACK_SEC3_YsrcClear
        // ADDR_MOD_PACK_SEC3_YdstIncr
        // ADDR_MOD_PACK_SEC3_YdstCR
        // ADDR_MOD_PACK_SEC3_YdstClear
        // ADDR_MOD_PACK_SEC3_ZsrcIncr
        // ADDR_MOD_PACK_SEC3_ZsrcClear
        // ADDR_MOD_PACK_SEC3_ZdstIncr
        // ADDR_MOD_PACK_SEC3_ZdstClear
        set_reg_addr_mod_pack(3, value);
        break;
    case 35:
        // ADDR_MOD_PACK_SEC4_YsrcIncr
        // ADDR_MOD_PACK_SEC4_YsrcCR
        // ADDR_MOD_PACK_SEC4_YsrcClear
        // ADDR_MOD_PACK_SEC4_YdstIncr
        // ADDR_MOD_PACK_SEC4_YdstCR
        // ADDR_MOD_PACK_SEC4_YdstClear
        // ADDR_MOD_PACK_SEC4_ZsrcIncr
        // ADDR_MOD_PACK_SEC4_ZsrcClear
        // ADDR_MOD_PACK_SEC4_ZdstIncr
        // ADDR_MOD_PACK_SEC4_ZdstClear
        set_reg_addr_mod_pack(4, value);
        break;
    case 36:
        // ADDR_MOD_PACK_SEC5_YsrcIncr
        // ADDR_MOD_PACK_SEC5_YsrcCR
        // ADDR_MOD_PACK_SEC5_YsrcClear
        // ADDR_MOD_PACK_SEC5_YdstIncr
        // ADDR_MOD_PACK_SEC5_YdstCR
        // ADDR_MOD_PACK_SEC5_YdstClear
        // ADDR_MOD_PACK_SEC5_ZsrcIncr
        // ADDR_MOD_PACK_SEC5_ZsrcClear
        // ADDR_MOD_PACK_SEC5_ZdstIncr
        // ADDR_MOD_PACK_SEC5_ZdstClear
        set_reg_addr_mod_pack(5, value);
        break;
    case 37:
        // ADDR_MOD_PACK_SEC6_YsrcIncr
        // ADDR_MOD_PACK_SEC6_YsrcCR
        // ADDR_MOD_PACK_SEC6_YsrcClear
        // ADDR_MOD_PACK_SEC6_YdstIncr
        // ADDR_MOD_PACK_SEC6_YdstCR
        // ADDR_MOD_PACK_SEC6_YdstClear
        // ADDR_MOD_PACK_SEC6_ZsrcIncr
        // ADDR_MOD_PACK_SEC6_ZsrcClear
        // ADDR_MOD_PACK_SEC6_ZdstIncr
        // ADDR_MOD_PACK_SEC6_ZdstClear
        set_reg_addr_mod_pack(6, value);
        break;
    case 38:
        // ADDR_MOD_PACK_SEC7_YsrcIncr
        // ADDR_MOD_PACK_SEC7_YsrcCR
        // ADDR_MOD_PACK_SEC7_YsrcClear
        // ADDR_MOD_PACK_SEC7_YdstIncr
        // ADDR_MOD_PACK_SEC7_YdstCR
        // ADDR_MOD_PACK_SEC7_YdstClear
        // ADDR_MOD_PACK_SEC7_ZsrcIncr
        // ADDR_MOD_PACK_SEC7_ZsrcClear
        // ADDR_MOD_PACK_SEC7_ZdstIncr
        // ADDR_MOD_PACK_SEC7_ZdstClear
        set_reg_addr_mod_pack(7, value);
        break;
    case 39:
        // UNPACK_MISC_CFG_CfgContextOffset_0
        m_adc[0].unp_cntx_offset = value & 0xf; 
        // UNPACK_MISC_CFG_CfgContextCntReset_0
        // UNPACK_MISC_CFG_CfgContextCntInc_0
        // UNPACK_MISC_CFG_CfgContextOffset_1
        m_adc[1].unp_cntx_offset = (value & 0xf00) >> 8;
        // UNPACK_MISC_CFG_CfgContextCntReset_1
        // UNPACK_MISC_CFG_CfgContextCntInc_1
        break;
    case 48:
        // ADDR_MOD_BIAS_SEC0_BiasIncr
        // ADDR_MOD_BIAS_SEC0_BiasClear
        set_reg_addr_mod_bias(0, value);
        break;
    case 49:
        // ADDR_MOD_BIAS_SEC1_BiasIncr
        // ADDR_MOD_BIAS_SEC1_BiasClear
        set_reg_addr_mod_bias(1, value);
        break;
    case 50:
        // ADDR_MOD_BIAS_SEC2_BiasIncr
        // ADDR_MOD_BIAS_SEC2_BiasClear
        set_reg_addr_mod_bias(2, value);
        break;
    case 51:
        // ADDR_MOD_BIAS_SEC3_BiasIncr
        // ADDR_MOD_BIAS_SEC3_BiasClear
        set_reg_addr_mod_bias(3, value);
        break;
    case 52:
        // ADDR_MOD_BIAS_SEC4_BiasIncr
        // ADDR_MOD_BIAS_SEC4_BiasClear
        set_reg_addr_mod_bias(4, value);
        break;
    case 53:
        // ADDR_MOD_BIAS_SEC5_BiasIncr
        // ADDR_MOD_BIAS_SEC5_BiasClear
        set_reg_addr_mod_bias(5, value);
        break;
    case 54:
        // ADDR_MOD_BIAS_SEC6_BiasIncr
        // ADDR_MOD_BIAS_SEC6_BiasClear
        set_reg_addr_mod_bias(6, value);
        break;
    case 55:
        // ADDR_MOD_BIAS_SEC7_BiasIncr
        // ADDR_MOD_BIAS_SEC7_BiasClear
        set_reg_addr_mod_bias(7, value);
        break;
    default:
        // unsupported register
        break;
    }
}

void Tensix::on_wrcfg32(uint32_t reg) {
    on_wrcfg(reg, 0xFFFFFFFF);
}

void Tensix::on_wrcfg128(uint32_t reg) {
    // optimizations for avoiding repeated expensve state updates can be placed here
    on_wrcfg32(reg);
    on_wrcfg32(reg + 1);
    on_wrcfg32(reg + 2);
    on_wrcfg32(reg + 3);
}

void Tensix::on_wrcfg(uint32_t reg, uint32_t mask) {
    uint32_t cfg_state_id = m_cfg_state_id[m_thread_id];
    uint32_t value = m_cfg_reg[cfg_state_id][reg];

    // For simplicity, 'mask' is ignored in most cases
    switch (reg) {
    case 2:
        // ALU_ACC_CTRL_Zero_Flag_disabled_src
        // ALU_ACC_CTRL_Zero_Flag_disabled_dst
        // STACC_RELU_ApplyRelu
        m_relu_type = (value & 0x3c) >> 2;
        // STACC_RELU_ReluThreshold
        m_relu_threshold = (value & 0x3fffc0) >> 6;
        // DISABLE_RISC_BP_Disable_main
        // DISABLE_RISC_BP_Disable_trisc
        // DISABLE_RISC_BP_Disable_ncrisc
        // DISABLE_RISC_BP_Disable_bmp_clear_main
        // DISABLE_RISC_BP_Disable_bmp_clear_trisc
        // DISABLE_RISC_BP_Disable_bmp_clear_ncrisc
        break;
    case 8:
        // PCK0_ADDR_CTRL_XY_REG_0_Xstride
        m_pack_ch1_x_stride = value & 0xfff;
        // PCK0_ADDR_CTRL_XY_REG_0_Ystride
        m_pack_ch1_y_stride = (value & 0xfff000) >> 12;
        m_pack_update_strides = true;
        break;
    case 9:
        // PCK0_ADDR_CTRL_ZW_REG_0_Zstride
        m_pack_ch1_z_stride = value & 0xfff;
        // PCK0_ADDR_CTRL_ZW_REG_0_Wstride
        m_pack_ch1_w_stride = (value & 0xfff000) >> 12;
        m_pack_update_strides = true;
        break;
    case 14:
        // PCK_DEST_RD_CTRL_Read_32b_data
        m_pack_fp32_dest = ((value & 0x1) != 0); 
        // PCK_DEST_RD_CTRL_Read_unsigned
        // PCK_DEST_RD_CTRL_Read_int8
        // PCK_DEST_RD_CTRL_Round_10b_mant
        break;
    case 16:
        // TILE_ROW_SET_MAPPING_0_row_set_mapping_[0-15]
        m_tile_row_set_mapping[0] = value;
        break;
    case 17:
        // TILE_ROW_SET_MAPPING_1_row_set_mapping_[0-15]
        m_tile_row_set_mapping[1] = value;
        break;
    case 18:
        // TILE_ROW_SET_MAPPING_2_row_set_mapping_[0-15]
        m_tile_row_set_mapping[2] = value;
        break;
    case 19:
        // TILE_ROW_SET_MAPPING_3_row_set_mapping_[0-15]
        m_tile_row_set_mapping[3] = value;
        break;
    case 20:
        // PCK_EDGE_OFFSET_SEC0_mask
        m_pack_edge_offset_mask[0] = value & 0xffff;
        // PCK_EDGE_MODE_mode
        // PCK_EDGE_TILE_ROW_SET_SELECT
        m_pack_edge_row_set_select[0] = (value >> 17) & 0x3;
        m_pack_edge_row_set_select[1] = (value >> 19) & 0x3;
        m_pack_edge_row_set_select[2] = (value >> 21) & 0x3;
        m_pack_edge_row_set_select[3] = (value >> 23) & 0x3;
        break;
    case 21:
        // PCK_EDGE_OFFSET_SEC1_mask
        m_pack_edge_offset_mask[1] = value & 0xffff;
        break;
    case 22:
        // PCK_EDGE_OFFSET_SEC2_mask
        m_pack_edge_offset_mask[2] = value & 0xffff;
        break;
    case 23:
        // PCK_EDGE_OFFSET_SEC3_mask
        m_pack_edge_offset_mask[3] = value & 0xffff;
        break;
    case 52:
        // THCON_SEC0_REG0_TileDescriptor
        // word 0
        // in_data_format : 4
        // uncompressed : 1
        // reserved_0 : 3
        // blobs_per_xy_plane : 4
        // reserved_1 : 4
        // x_dim : 16
        m_adc[0].tile_in_data_format = DataFormat(value & 0x7);
        m_adc[0].tile_x_dim = (value >> 16) & 0xffff;
        m_adc[0].update_strides = true;
        break;
    case 53:
        // THCON_SEC0_REG0_TileDescriptor
        // word 1
        // y_dim : 16
        // z_dim : 16
        m_adc[0].tile_y_dim = value & 0xffff;
        m_adc[0].tile_z_dim = (value >> 16) & 0xffff;
        m_adc[0].update_strides = true;
        break;
    case 54:
        // THCON_SEC0_REG0_TileDescriptor
        // word 2
        // w_dim : 16
        // blobs_y_start_lo : 16
        // nothing to do (w_dim is unused)
        break;
    case 55:
        // THCON_SEC0_REG0_TileDescriptor
        // word 3
        // blobs_y_start_hi : 16
        // digest_type : 8
        // digest_size : 8
        // nothing to do so far
        break;
    case 57:
        // THCON_SEC0_REG1_L1_Dest_addr
        // ACHTUNG: MSB is temporarily ignored as its purpose is not clear
        // TODO: Clarify its purpose
        m_pack_dest_addr = value & 0x7fffffff;
        break;
    case 58:
        // THCON_SEC0_REG1_Disable_zero_compress
        // THCON_SEC0_REG1_Add_l1_dest_addr_offset
        // THCON_SEC0_REG1_Addr_cnt_context
        // THCON_SEC0_REG1_Out_data_format
        m_pack_dst_format = DataFormat((value & 0xf0) >> 4);
        // THCON_SEC0_REG1_In_data_format
        m_pack_src_format = DataFormat((value & 0xf00) >> 8);
        // THCON_SEC0_REG1_Dis_shared_exp_assembler
        // THCON_SEC0_REG1_Force_pack_per_max_xy_plane
        // THCON_SEC0_REG1_Enable_out_fifo
        // THCON_SEC0_REG1_Sub_l1_tile_header_size
        // THCON_SEC0_REG1_Source_interface_selection
        // THCON_SEC0_REG1_All_pack_disable_zero_compress
        // THCON_SEC0_REG1_All_pack_disable_zero_compress_ovrd
        // THCON_SEC0_REG1_Add_tile_header_size
        // THCON_SEC0_REG1_L1_source_addr
        break;
    case 60:
        // THCON_SEC0_REG2_Out_data_format
        // THCON_SEC0_REG2_Throttle_mode
        // THCON_SEC0_REG2_Context_count
        // THCON_SEC0_REG2_Haloize_mode
        m_haloize_mode = (value & 0x100) >> 8;
        // THCON_SEC0_REG2_Tileize_mode
        m_tileize_mode = (value & 0x200) >> 9;
        // THCON_SEC0_REG2_Unpack_Src_Reg_Set_Upd
        // THCON_SEC0_REG2_Unpack_If_Sel
        // THCON_SEC0_REG2_Upsample_rate
        // THCON_SEC0_REG2_Ovrd_data_format
        // THCON_SEC0_REG2_Upsample_and_interleave
        // THCON_SEC0_REG2_Shift_amount_cntx0
        m_shift_amount = (value & 0xffff0000) >> 16;
        // THCON_SEC0_REG2_Shift_amount_cntx1
        // THCON_SEC0_REG2_Shift_amount_cntx2
        // THCON_SEC0_REG2_Shift_amount_cntx3
        break;
    case 64:
        // THCON_SEC0_REG3_Base_address
        m_adc[0].cntx[0].base_addr = value;
        break;
    case 65:
        // THCON_SEC0_REG3_Base_cntx1_address
        m_adc[0].cntx[1].base_addr = value;
        break;
    case 66:
        // THCON_SEC0_REG3_Base_cntx2_address
        m_adc[0].cntx[2].base_addr = value;
        break;
    case 67:
        // THCON_SEC0_REG3_Base_cntx3_address
        m_adc[0].cntx[3].base_addr = value;
        break;
    case 68:
        // THCON_SEC0_REG3_Base_cntx4_address
        m_adc[0].cntx[4].base_addr = value;
        break;
    case 69:
        // THCON_SEC0_REG3_Base_cntx5_address
        m_adc[0].cntx[5].base_addr = value;
        break;
    case 70:
        // THCON_SEC0_REG3_Base_cntx6_address
        m_adc[0].cntx[6].base_addr = value;
        break;
    case 71:
        // THCON_SEC0_REG3_Base_cntx7_address
        m_adc[0].cntx[7].base_addr = value;
        break;
    case 74:
        // THCON_SEC0_REG5_Tile_x_dim_cntx0
        m_tile_x_dim = value & 0xffff;
        // THCON_SEC0_REG5_Tile_x_dim_cntx1
        m_adc[0].update_strides = true;
        break;
    case 80:
        // THCON_SEC0_REG7_Offset_address
        m_adc[0].cntx[0].offset_addr = value & 0xffff;
        // THCON_SEC0_REG7_Unpack_data_format_cntx0
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx0
        // THCON_SEC0_REG7_Unpack_data_format_cntx4
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx4
        break;
    case 81:
        // THCON_SEC0_REG7_Offset_cntx1_address
        m_adc[0].cntx[1].offset_addr = value & 0xffff;
        // THCON_SEC0_REG7_Unpack_data_format_cntx1
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx1
        // THCON_SEC0_REG7_Unpack_data_format_cntx5
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx5
        break;
    case 82:
        // THCON_SEC0_REG7_Offset_cntx2_address
        m_adc[0].cntx[2].offset_addr = value & 0xffff;
        // THCON_SEC0_REG7_Unpack_data_format_cntx2
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx2
        // THCON_SEC0_REG7_Unpack_data_format_cntx6
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx6
        break;
    case 83:
        // THCON_SEC0_REG7_Offset_cntx3_address
        m_adc[0].cntx[3].offset_addr = value & 0xffff;
        // THCON_SEC0_REG7_Unpack_data_format_cntx3
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx3
        // THCON_SEC0_REG7_Unpack_data_format_cntx7
        // THCON_SEC0_REG7_Unpack_out_data_format_cntx7
        break;
    case 85:
        // THCON_SEC0_REG8_L1_Dest_addr
        // +1 to compensate for tile header offset (see "cpack_common.cpp")
        m_pack_l1_offset = value + 1;
        break;
    case 92:
        // THCON_SEC1_REG0_TileDescriptor
        // word 0
        // in_data_format : 4
        // uncompressed : 1
        // reserved_0 : 3
        // blobs_per_xy_plane : 4
        // reserved_1 : 4
        // x_dim : 16
        m_adc[1].tile_in_data_format = DataFormat(value & 0x7);
        m_adc[1].tile_x_dim = (value >> 16) & 0xffff;
        m_adc[1].update_strides = true;
        break;
    case 93:
        // THCON_SEC1_REG0_TileDescriptor
        // word 1
        // y_dim : 16
        // z_dim : 16
        m_adc[1].tile_y_dim = value & 0xffff;
        m_adc[1].tile_z_dim = (value >> 16) & 0xffff;
        m_adc[1].update_strides = true;
        break;
    case 94:
        // THCON_SEC1_REG0_TileDescriptor
        // word 2
        // w_dim : 16
        // blobs_y_start_lo : 16
        // nothing to do (w_dim is unused)
        break;
    case 95:
        // THCON_SEC1_REG0_TileDescriptor
        // word 3
        // blobs_y_start_hi : 16
        // digest_type : 8
        // digest_size : 8
        // nothing to do so far
        break;
    case 97:
        // THCON_SEC1_REG1_L1_Dest_addr
        // Ignored in this implementation
        // (setting m_pack_l1_offset at case 85 controls everything)
        // To be revised
        break;
    case 104:
        // THCON_SEC1_REG3_Base_address
        m_adc[1].cntx[0].base_addr = value;
        break;
    case 105:
        // THCON_SEC1_REG3_Base_cntx1_address
        m_adc[1].cntx[1].base_addr = value;
        break;
    case 106:
        // THCON_SEC1_REG3_Base_cntx2_address
        m_adc[1].cntx[2].base_addr = value;
        break;
    case 107:
        // THCON_SEC1_REG3_Base_cntx3_address
        m_adc[1].cntx[3].base_addr = value;
        break;
    case 108:
        // THCON_SEC1_REG3_Base_cntx4_address
        m_adc[1].cntx[4].base_addr = value;
        break;
    case 109:
        // THCON_SEC1_REG3_Base_cntx5_address
        m_adc[1].cntx[5].base_addr = value;
        break;
    case 110:
        // THCON_SEC1_REG3_Base_cntx6_address
        m_adc[1].cntx[6].base_addr = value;
        break;
    case 111:
        // THCON_SEC1_REG3_Base_cntx7_address
        m_adc[1].cntx[7].base_addr = value;
        break;
    case 120:
        // THCON_SEC1_REG7_Offset_address
        m_adc[1].cntx[0].offset_addr = value & 0xffff;
        // THCON_SEC1_REG7_Unpack_data_format_cntx0
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx0
        // THCON_SEC1_REG7_Unpack_data_format_cntx4
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx4
        break;
    case 121:
        // THCON_SEC1_REG7_Offset_cntx1_address
        m_adc[1].cntx[1].offset_addr = value & 0xffff;
        // THCON_SEC1_REG7_Unpack_data_format_cntx1
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx1
        // THCON_SEC1_REG7_Unpack_data_format_cntx5
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx5
        break;
    case 122:
        // THCON_SEC1_REG7_Offset_cntx2_address
        m_adc[1].cntx[2].offset_addr = value & 0xffff;
        // THCON_SEC1_REG7_Unpack_data_format_cntx2
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx2
        // THCON_SEC1_REG7_Unpack_data_format_cntx6
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx6
        break;
    case 123:
        // THCON_SEC1_REG7_Offset_cntx3_address
        m_adc[1].cntx[3].offset_addr = value & 0xffff;
        // THCON_SEC1_REG7_Unpack_data_format_cntx3
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx3
        // THCON_SEC1_REG7_Unpack_data_format_cntx7
        // THCON_SEC1_REG7_Unpack_out_data_format_cntx7
        break;
    case 125:
        // THCON_SEC1_REG8_L1_Dest_addr
        // Ignored in this implementation
        // (setting m_pack_l1_offset at case 85 controls everything)
        // To be revised
        break;
    case 152:
        // DEST_TARGET_REG_CFG_PACK_SEC0_Offset
        m_pack_dest_offset[0] = value & 0xfff;
        // DEST_TARGET_REG_CFG_PACK_SEC0_ZOffset
        break;
    case 153:
        // DEST_TARGET_REG_CFG_PACK_SEC1_Offset
        m_pack_dest_offset[1] = value & 0xfff;
        // DEST_TARGET_REG_CFG_PACK_SEC1_ZOffset
        break;
    case 154:
        // DEST_TARGET_REG_CFG_PACK_SEC2_Offset
        m_pack_dest_offset[2] = value & 0xfff;
        // DEST_TARGET_REG_CFG_PACK_SEC2_ZOffset
        break;
    case 155:
        // DEST_TARGET_REG_CFG_PACK_SEC3_Offset
        m_pack_dest_offset[3] = value & 0xfff;
        // DEST_TARGET_REG_CFG_PACK_SEC3_ZOffset
        break;
    default:
        // unsupported register
        break;
    }
}

void Tensix::set_reg_addr_mod_ab(int index, uint32_t value) {
    AddrMod &addr_mod = m_addr_mod[index];
    addr_mod.srca.incr = value & 0x3f;
    addr_mod.srca.clr = (value & 0x80) >> 7;
    addr_mod.srca.cr = (value & 0x40) >> 6;
    addr_mod.srcb.incr = (value & 0x3f00) >> 8;
    addr_mod.srcb.clr = (value & 0x8000) >> 15;
    addr_mod.srcb.cr = (value & 0x4000) >> 14;
}

void Tensix::set_reg_addr_mod_bias(int index, uint32_t value) {
    AddrMod &addr_mod = m_addr_mod[index];
    addr_mod.bias.incr = value & 0xf;
    addr_mod.bias.clr = (value & 0x10) >> 4;
}

void Tensix::set_reg_addr_mod_dst(int index, uint32_t value) {
    AddrMod &addr_mod = m_addr_mod[index];
    addr_mod.dest.incr = value & 0x3ff;
    addr_mod.dest.clr = (value & 0x800) >> 11;
    addr_mod.dest.cr = (value & 0x400) >> 10;
    addr_mod.dest.c_to_cr = (value & 0x1000) >> 12;
    addr_mod.fidelity.incr = (value & 0x6000) >> 13;
    addr_mod.fidelity.clr = (value & 0x8000) >> 15;
}

void Tensix::set_reg_addr_mod_pack(int index, uint32_t value) {
    AddrModPack &addr_mod_pack = m_addr_mod_pack[index];
    addr_mod_pack.y_src.incr = value & 0xf;
    addr_mod_pack.y_src.clr = (value & 0x20) >> 5;
    addr_mod_pack.y_src.cr = (value & 0x10) >> 4;
    addr_mod_pack.y_dst.incr = (value & 0x3c0) >> 6;
    addr_mod_pack.y_dst.clr = (value & 0x800) >> 11;
    addr_mod_pack.y_dst.cr = (value & 0x400) >> 10;
    addr_mod_pack.z_src.incr = (value & 0x1000) >> 12;
    addr_mod_pack.z_src.clr = (value & 0x2000) >> 13;
    addr_mod_pack.z_dst.incr = (value & 0x4000) >> 14;
    addr_mod_pack.z_dst.clr = (value & 0x8000) >> 15;
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

