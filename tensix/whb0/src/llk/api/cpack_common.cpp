
#include <cstdint>

#include "system/cfg_defines.h"

#include "core/tensix_types.hpp"

#include "fw_debug.h"
#include "ckernel_core.h"
#include "ckernel_instr_params.h"
#include "ckernel_gpr_map.h"
#include "ckernel_defs.h"
#include "ckernel_structs.h"
#include "ckernel_mem_ptr.h"
#include "ckernel.h"
#include "cpack_common.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace packer {

namespace {

uint32_t get_packer_dest_offset_index() {
    return (dest_offset_id != 0) ? p_gpr_pack::DEST_OFFSET_HI : p_gpr_pack::DEST_OFFSET_LO;
}

void set_packer_strides(uint32_t output_id) {
    // Get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    uint32_t x_stride = get_df_item_size(get_pack_src_format(output_id));
    uint32_t y_stride = 16 * x_stride;
    uint32_t z_stride = PACK_CNT * 16 * y_stride;
    uint32_t w_stride = z_stride;

    // x-stride not used!
    uint32_t xy_stride = (y_stride << PCK0_ADDR_CTRL_XY_REG_0_Ystride_SHAMT);
    TT_SETDMAREG(0, LOWER_HALFWORD(xy_stride), 0, LO_16(p_gpr_pack::TMP0)); 
    TT_SETDMAREG(0, UPPER_HALFWORD(xy_stride), 0, HI_16(p_gpr_pack::TMP0));
    TTI_WRCFG(p_gpr_pack::TMP0, p_cfg::WRCFG_32b, PCK0_ADDR_CTRL_XY_REG_0_Xstride_ADDR32);
    // z-stride not used!
    uint32_t zw_stride = (w_stride << PCK0_ADDR_CTRL_ZW_REG_0_Wstride_SHAMT);
    TT_SETDMAREG(0, LOWER_HALFWORD(zw_stride), 0, LO_16(p_gpr_pack::TMP0)); 
    TT_SETDMAREG(0, UPPER_HALFWORD(zw_stride), 0, HI_16(p_gpr_pack::TMP0));
    TTI_WRCFG(p_gpr_pack::TMP0, p_cfg::WRCFG_32b, PCK0_ADDR_CTRL_ZW_REG_0_Zstride_ADDR32);
    TTI_NOP; 
    TTI_NOP;
}

void set_packer_config(bool is_fp32_dest_acc_en, uint32_t output_id) {
    // Get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    DataFormat src_format = get_pack_src_format(output_id);
    DataFormat dst_format = get_pack_dst_format(output_id);

    uint32_t num_faces = get_num_faces(output_id);
    bool partial_face = get_partial_face(output_id);

    // Set packer config
    pack_config_u config;
    for (uint32_t i = 0; i < 4; i++) {
        config.val[i] = 0;
    }

    // set to num_faces as exp section size is not used for non-bfp formats except for lf8 / int8
    config.f.exp_section_size = 
        (dst_format == DataFormat::Lf8 || dst_format == DataFormat::Int8) ? 
            0 : (partial_face ? 1 : num_faces); 

    config.f.uncompress = 1;
    config.f.out_data_format = (uint32_t)dst_format;
    config.f.in_data_format = (uint32_t)src_format;
    config.f.pack_per_xy_plane = 1;

    // ACHTUNG: Irrelevant for simulator?
    // Workaround for bug in HW:
    // https://yyz-gitlab.local.tenstorrent.com/tenstorrent/budabackend/-/issues/1394
    if (is_fp32_dest_acc_en) {
        if (IS_BFP_A_FORMAT((uint32_t)dst_format)) {
            config.f.exp_threshold_en = 1;
            config.f.exp_threshold = 113;
        }
    }

    // Program:
    // THCON_SEC0_REG1_Row_start_section_size = cfg_reg_array[1][0 +: 16];
    // THCON_SEC0_REG1_Exp_section_size = cfg_reg_array[1][16 +: 16];
    // This is filled with garbage, and will be set up on every pack:
    //           THCON_SEC0_REG1_L1_Dest_addr = cfg_reg_array[1][32 +: 32];
    // THCON_SEC0_REG1_Disable_zero_compress = cfg_reg_array[1][64 +: 1];
    // THCON_SEC0_REG1_Add_l1_dest_addr_offset = cfg_reg_array[1][65 +: 1];
    // THCON_SEC0_REG1_Unused0 = cfg_reg_array[1][66 +: 2];
    // THCON_SEC0_REG1_Out_data_format = cfg_reg_array[1][68 +: 4];
    // THCON_SEC0_REG1_In_data_format = cfg_reg_array[1][72 +: 4];
    // THCON_SEC0_REG1_Unused00 = cfg_reg_array[1][76 +: 4];
    // THCON_SEC0_REG1_Source_interface_selection = cfg_reg_array[1][80 +: 1];
    // THCON_SEC0_REG1_Packs_per_xy_plane = cfg_reg_array[1][81 +: 7];
    // THCON_SEC0_REG1_L1_source_addr = cfg_reg_array[1][88 +: 8];
    // THCON_SEC0_REG1_Downsample_mask = cfg_reg_array[1][96 +: 16];
    // THCON_SEC0_REG1_Downsample_shift_count = cfg_reg_array[1][112 +: 3];
    // THCON_SEC0_REG1_Read_mode = cfg_reg_array[1][115 +: 1];
    // THCON_SEC0_REG1_Exp_threshold_en = cfg_reg_array[1][116 +: 1];
    // THCON_SEC0_REG1_Unused1 = cfg_reg_array[1][117 +: 3];
    // THCON_SEC0_REG1_Exp_threshold = cfg_reg_array[1][120 +: 8];

    cfg[THCON_SEC0_REG1_Row_start_section_size_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
    cfg[THCON_SEC0_REG1_Row_start_section_size_ADDR32 + 2] = config.val[2];
    cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 2] = config.val[2];
    cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 2] = config.val[2];
    cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 2] = config.val[2];
    cfg[THCON_SEC0_REG1_Row_start_section_size_ADDR32 + 3] = config.val[3];
    cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 3] = config.val[3];
    cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 3] = config.val[3];
    cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 3] = config.val[3];

    dest_rd_ctrl_u dest_rd_ctrl;
    dest_rd_ctrl.val = 0;
    dest_rd_ctrl.f.PCK_DEST_RD_CTRL_Read_32b_data = 
        (src_format == DataFormat::Int8) | (is_fp32_dest_acc_en ? 1 : 0);
    cfg[PCK_DEST_RD_CTRL_Read_32b_data_ADDR32] = dest_rd_ctrl.val;

    if (IS_BFP_FORMAT((uint32_t)dst_format)) {
        // Override exp section size for packers 1,2,3
        // Tile header + exp size + datum size
        DataFormat base_dst_format = DataFormat((uint32_t)dst_format & 0x1F);
        if (base_dst_format == DataFormat::Bfp8 || base_dst_format == DataFormat::Bfp8_b) {
            config.f.exp_section_size = 1 + ((num_faces > 2) ? 2 : 0) + 16;
            cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 1 + 32;
            cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 0 + 48;
            cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
        } else if (base_dst_format == DataFormat::Bfp4 || base_dst_format == DataFormat::Bfp4_b) {
            config.f.exp_section_size = 1 + ((num_faces > 2) ? 2 : 0) + 8;
            cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 1 + 16;
            cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 0 + 24;
            cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
        } else if (base_dst_format == DataFormat::Bfp2 || base_dst_format == DataFormat::Bfp2_b) {
            config.f.exp_section_size = 1 + ((num_faces > 2) ? 2 : 0) + 4;
            cfg[THCON_SEC0_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 1 + 8;
            cfg[THCON_SEC1_REG1_Row_start_section_size_ADDR32 + 0] = config.val[0];
            config.f.exp_section_size = 1 + 0 + 12;
            cfg[THCON_SEC1_REG8_Row_start_section_size_ADDR32 + 0] = config.val[0];
        } else {
            FWASSERT("Other data formats not supported", false);
        }
    }

    // Save to GPR for quick data format reconfig
    constexpr uint32_t shamt = THCON_SEC0_REG8_Exp_section_size_SHAMT;
    regfile[p_gpr_pack::EXP0_SEC_SIZE_BFP]  = num_faces << shamt;
    regfile[p_gpr_pack::EXP1_SEC_SIZE_BFP8] = (1 + ((num_faces > 2) ? 2 : 0) + 16) << shamt;
    regfile[p_gpr_pack::EXP2_SEC_SIZE_BFP8] = (1 + 1 + 32) << shamt;
    regfile[p_gpr_pack::EXP3_SEC_SIZE_BFP8] = (1 + 0 + 48) << shamt;
    regfile[p_gpr_pack::EXP1_SEC_SIZE_BFP4] = (1 + ((num_faces > 2) ? 2 : 0) + 8 ) << shamt;
    regfile[p_gpr_pack::EXP2_SEC_SIZE_BFP4] = (1 + 1 + 16) << shamt;
    regfile[p_gpr_pack::EXP3_SEC_SIZE_BFP4] = (1 + 0 + 24) << shamt;
    regfile[p_gpr_pack::EXP1_SEC_SIZE_BFP2] = (1 + ((num_faces> 2 ) ? 2 : 0) + 4 ) << shamt;
    regfile[p_gpr_pack::EXP2_SEC_SIZE_BFP2] = (1 + 1 + 8 ) << shamt;
    regfile[p_gpr_pack::EXP3_SEC_SIZE_BFP2] = (1 + 0 + 12) << shamt;
    sync_regfile_write(p_gpr_pack::EXP3_SEC_SIZE_BFP2);
}

void set_packer_l1_offset(uint32_t output_id){
    uint32_t face_r_dim = get_face_r_dim(output_id);
    uint32_t face_dim = face_r_dim * FACE_C_DIM;

    DataFormat dst_format = get_pack_dst_format(output_id);
    DataFormat base_dst_format = DataFormat((uint32_t)dst_format & 0x3);

    uint32_t l1_offset_1 = 
        IS_BFP_FORMAT((uint32_t)dst_format) ? 1 : 
        (base_dst_format == DataFormat::Float32) ? (face_dim / 16) * 4 :
        (base_dst_format == DataFormat::Float16) ? (face_dim / 16) * 2 : (face_dim / 16);
    uint32_t l1_offset_2 = 2 * l1_offset_1;
    uint32_t l1_offset_3 = 3 * l1_offset_1;

    // HW automatically offsets packers base address by tile header size
    // with new L1 addressing mode, the effective address for pack1/2/3
    // will be pack[i] += pack[0], which leads to double counting of tile header
    // subtract by this amount when programming the offset
    constexpr uint32_t PACK_TILE_HEADER_OFFSET = 1; // in 16B
    l1_offset_1 -= PACK_TILE_HEADER_OFFSET;
    l1_offset_2 -= PACK_TILE_HEADER_OFFSET;
    l1_offset_3 -= PACK_TILE_HEADER_OFFSET;
    uint32_t flop_index_1 = THCON_SEC0_REG8_L1_Dest_addr_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    uint32_t flop_index_2 = THCON_SEC1_REG1_L1_Dest_addr_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    uint32_t flop_index_3 = THCON_SEC1_REG8_L1_Dest_addr_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    TT_SETDMAREG(0, LOWER_HALFWORD(l1_offset_1), 0, LO_16(p_gpr_pack::TMP_LO));
    TTI_REG2FLOP(2, 0, 0, 0, flop_index_1, p_gpr_pack::TMP_LO);
    TT_SETDMAREG(0, LOWER_HALFWORD(l1_offset_2), 0, LO_16(p_gpr_pack::TMP_LO));
    TTI_REG2FLOP(2, 0, 0, 0, flop_index_2, p_gpr_pack::TMP_LO);
    TT_SETDMAREG(0, LOWER_HALFWORD(l1_offset_3), 0, LO_16(p_gpr_pack::TMP_LO));
    TTI_REG2FLOP(2, 0, 0, 0, flop_index_3, p_gpr_pack::TMP_LO);
}

} // namespace

void configure_pack_impl(
        bool is_fp32_dest_acc_en, 
        bool untilize,
        uint32_t pack_output_id, 
        uint32_t relu_config) {
    // Get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    DataFormat src_format = get_pack_src_format(pack_output_id);
    DataFormat dst_format = get_pack_dst_format(pack_output_id);

    if (src_format != dst_format) {
        TTI_STALLWAIT(p_stall::STALL_PACK, p_stall::PACK);
        tensix_sync();
    }

    set_packer_strides(pack_output_id);

    t6_mutex_acquire(mutex::REG_RMW);

    uint32_t alu_dst_format = (uint32_t)src_format;

    cfg_reg_rmw_tensix<ALU_FORMAT_SPEC_REG2_Dstacc_RMW>(alu_dst_format);

    t6_mutex_release(mutex::REG_RMW);

    set_packer_config(is_fp32_dest_acc_en, pack_output_id);

    set_packer_l1_offset(pack_output_id);

    // PACK_COUNTERS_SEC0_pack_per_xy_plane = cfg_reg_array[3][0 +: 8];
    // PACK_COUNTERS_SEC0_pack_reads_per_xy_plane = cfg_reg_array[3][8 +: 8];
    // PACK_COUNTERS_SEC0_pack_xys_per_tile = cfg_reg_array[3][16 +: 7];
    // PACK_COUNTERS_SEC0_pack_yz_transposed = cfg_reg_array[3][23 +: 1];
    pack_counters_u pack_counters;
    pack_counters.val = 0;
    // Number of reads per face
    // Used for resetting tile posistion generator for edge masks
    pack_counters.f.pack_reads_per_xy_plane = get_face_r_dim(pack_output_id); 
    for (uint32_t i = 0; i < 4; i++) {
        // disable auto last generation
        cfg[PACK_COUNTERS_SEC0_pack_per_xy_plane_ADDR32 + i] = pack_counters.val; 
    }

    pck_edge_offset_u pck_edge_offset;
    pck_edge_offset.val = 0;
    pck_edge_offset.f.mask = 0xffff;

    cfg[PCK_EDGE_OFFSET_SEC0_mask_ADDR32] = pck_edge_offset.val;
    // All packers use row set mapping 0, edge offset 0 mask
    cfg[TILE_ROW_SET_MAPPING_0_row_set_mapping_0_ADDR32] = 0x0; 

    regfile[p_gpr_pack::TILE_HEADER] = (uint32_t)get_cb_interface(pack_output_id).fifo_page_size;
    regfile[p_gpr_pack::TILE_HEADER + 1] = 0;
    regfile[p_gpr_pack::TILE_HEADER + 2] = 0;
    regfile[p_gpr_pack::TILE_HEADER + 3] = 0;
    sync_regfile_write(p_gpr_pack::TILE_HEADER + 3);

    // Config RELU
    relu_config_u hw_relu_config;
    // ACHTUNG: Cannot convert pointer to uint32, must adjust API
    //     (introduce macro to hide difference between silicon and simulator versions?)
#if 0
    uint32_t current_relu_val = reg_read((uint32_t)&cfg[STACC_RELU_ApplyRelu_ADDR32]);
#else
    uint32_t current_relu_val = cfg[STACC_RELU_ApplyRelu_ADDR32];
#endif
    hw_relu_config.val[0] = current_relu_val;

    hw_relu_config.r.STACC_RELU_ApplyRelu = relu_config & 0xffff;
    hw_relu_config.r.STACC_RELU_ReluThreshold = (relu_config >> 16) & 0xffff;

    cfg[STACC_RELU_ApplyRelu_ADDR32] = hw_relu_config.val[0];

    uint32_t face_r_dim = get_face_r_dim(pack_output_id);
    uint32_t face_dim = face_r_dim * FACE_C_DIM;
    bool narrow_tile = get_narrow_tile(pack_output_id);
    // Number of datums to pack per row
    // To untilize narrow tile (32x16) we just pack 2 faces back to back
    uint32_t pack_x_dim = untilize ? (narrow_tile ? face_dim : 16) : face_dim; 
    TT_SETADCXX(p_setadc::PAC, pack_x_dim - 1, 0x0);
}

void init_packer_dest_offset_registers_impl(
        DstTileFaceLayout FaceLayout, 
        bool untilize, 
        bool is_fp32_dest_acc_en) {
    uint32_t DEST_OFFSET_SHIFT = 0; // is_fp32_dest_acc_en ? 1 : 0;
    uint32_t DEST_HALF_OFFSET = DEST_REGISTER_HALF_SIZE >> DEST_OFFSET_SHIFT;

    if (untilize) {
        if (FaceLayout == DstTileFaceLayout::ColMajor) {
            // Packer0 :  0,32,  1,33 ...  7, 39
            // Packer1 :  8,40,  9,41 ... 15, 47
            // Packer2 : 16,48, 17,49 ... 23, 55
            // Packer3 : 23,56, 24,57 ... 31, 63
            regfile[p_gpr_pack::DEST_OFFSET_LO] = 0x0;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 1] = 0x0 + 0x8;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 2] = 0x0 + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 3] = 0x0 + 0x18;
            regfile[p_gpr_pack::DEST_OFFSET_HI] = DEST_HALF_OFFSET;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 1] = DEST_HALF_OFFSET + 0x8;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 2] = DEST_HALF_OFFSET + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 3] = DEST_HALF_OFFSET + 0x18;
        } else {
            // Packer0 :  0,16,  1,17 ...  7, 23
            // Packer1 :  8,24,  9,25 ... 15, 31
            // Packer2 : 32,48, 33,49 ... 39, 55
            // Packer3 : 40,56, 41,57 ... 47, 63
            regfile[p_gpr_pack::DEST_OFFSET_LO] = 0x0;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 1] = 0x0 + 0x8;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 2] = 0x0 + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 3] = 0x0 + 0x28;
            regfile[p_gpr_pack::DEST_OFFSET_HI] = DEST_HALF_OFFSET;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 1] = DEST_HALF_OFFSET + 0x8;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 2] = DEST_HALF_OFFSET + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 3] = DEST_HALF_OFFSET + 0x28;
        }
    } else {
        if (FaceLayout == DstTileFaceLayout::ColMajor) {
            regfile[p_gpr_pack::DEST_OFFSET_LO] = 0x0;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 1] = 0x0 + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 2] = 0x0 + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 3] = 0x0 + 0x30;
            regfile[p_gpr_pack::DEST_OFFSET_HI] = DEST_HALF_OFFSET;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 1] = DEST_HALF_OFFSET + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 2] = DEST_HALF_OFFSET + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 3] = DEST_HALF_OFFSET + 0x30;
        } else { 
            // Default to row major layout
            regfile[p_gpr_pack::DEST_OFFSET_LO] = 0x0;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 1] = 0x0 + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 2] = 0x0 + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_LO + 3] = 0x0 + 0x30;
            regfile[p_gpr_pack::DEST_OFFSET_HI] = DEST_HALF_OFFSET;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 1] = DEST_HALF_OFFSET + 0x10;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 2] = DEST_HALF_OFFSET + 0x20;
            regfile[p_gpr_pack::DEST_OFFSET_HI + 3] = DEST_HALF_OFFSET + 0x30;
        }
    }
    sync_regfile_write(p_gpr_pack::DEST_OFFSET_HI + 3);
}

void select_packer_dest_registers_impl(DstSync Dst) {
    // Flip packer dest register offset to 0 or DEST_REGISTER_HALF_SIZE
    // flip-flopping between two halfs
    constexpr uint32_t cfg_reg = DEST_TARGET_REG_CFG_PACK_SEC0_Offset_ADDR32;
    if (Dst == DstSync::SyncFull) {
        TTI_WRCFG(p_gpr_pack::DEST_OFFSET_LO, p_cfg::WRCFG_128b, cfg_reg);
    } else {
        TT_WRCFG(get_packer_dest_offset_index(), p_cfg::WRCFG_128b, cfg_reg);
    }
    TTI_DMANOP;
    TTI_DMANOP;
}

} // namespace packer
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

