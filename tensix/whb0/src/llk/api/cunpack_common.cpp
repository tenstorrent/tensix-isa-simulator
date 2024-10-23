
#include <cstdint>

#include "system/cfg_defines.h"

#include "core/tensix_types.hpp"

#include "ckernel_thread.h"
#include "ckernel_instr_params.h"
#include "ckernel_gpr_map.h"
#include "ckernel_mem_ptr.h"
#include "ckernel.h"
#include "cunpack_common.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace unpacker {

namespace {

uint32_t get_df_exp_width(DataFormat df) {
    // 0 = 5-bit, 1 = 8-bit
    return (uint32_t(df) >> 2) & 0x1; 
}

} // namespace

inline void unpacker_addr_counter_init() {
    // Set unpacker offsets to 0, except for unpacker 0, channel 1, X,
    // which is the tile X dimension
    TTI_SETADCXY(0b011, 0, 0, 0, 0, 0b1011);
    TTI_SETADCZW(0b011, 0, 0, 0, 0, 0b1111);
}

void wait_for_next_context(uint32_t num_contexts) {
    ThreadBase *thread = get_thread();
    wait([thread, num_contexts]() -> bool {
        return (thread->read_sem(semaphore::UNPACK_SYNC) < num_contexts);
    });
}

void switch_config_context() {
    unp_cfg_context = 1 - unp_cfg_context;
    if (unp_cfg_context == 0) {
        TTI_SETC16(UNPACK_MISC_CFG_CfgContextOffset_0_ADDR32, 0x0000);
    } else {
        TTI_SETC16(UNPACK_MISC_CFG_CfgContextOffset_0_ADDR32, 0x0101);
    }
}

void reset_config_context() {
    unp_cfg_context = 0;
    TTI_SETC16(UNPACK_MISC_CFG_CfgContextOffset_0_ADDR32, 0x0000);
}

void configure_unpack_AB(
         uint32_t unpA_operand_id,
         uint32_t unpB_operand_id,
         uint32_t unpA_face_r_dim/* = 16*/,
         uint32_t unpB_face_r_dim/* = 16*/,
         bool row_pool/* = false*/,
         bool transpose_xy_srca_en/* = false*/,
         bool is_fp32_dest_acc_en/* = false*/,
         bool srnd_fpu_en/* = false*/,
         uint32_t unpA_num_faces/* = 4*/,
         uint32_t unpB_num_faces/* = 4*/) {
    DataFormat unpA_src_format = get_unpack_src_format(unpA_operand_id);
    DataFormat unpA_dst_format = get_unpack_dst_format(unpA_operand_id);
    DataFormat unpB_src_format = get_unpack_src_format(unpB_operand_id);
    DataFormat unpB_dst_format = get_unpack_dst_format(unpB_operand_id);

    // Check that unpacker is done (all contexts freed up) before starting hw configuration
    wait_for_idle();

    // Reset address counters
    unpacker_addr_counter_init();

    // Get pointer to registers for current state ID
    MEM_PTR_T cfg = get_cfg_pointer();

    uint32_t unpA_ch1_x_stride = get_df_item_size(unpA_dst_format);
    uint32_t unpB_ch1_x_stride = get_df_item_size(unpB_dst_format);
    uint32_t unpA_ch1_z_stride = FACE_C_DIM * FACE_R_DIM * unpA_ch1_x_stride;
    uint32_t unpB_ch1_z_stride = FACE_C_DIM * FACE_R_DIM * unpB_ch1_x_stride;
    uint32_t exp_width = get_df_exp_width(unpA_dst_format);

    // Strides for incrementing ch1 address to srcA and srcB
    // Z and W (not used) stride for dest address (ch1)
    cfg[UNP0_ADDR_CTRL_ZW_REG_1_Zstride_ADDR32] = 
        (0 << UNP0_ADDR_CTRL_ZW_REG_1_Wstride_SHAMT) |
        (unpA_ch1_z_stride << UNP0_ADDR_CTRL_ZW_REG_1_Zstride_SHAMT);  
    // Z and W (not used) stride for dest address (ch1)
    cfg[UNP1_ADDR_CTRL_ZW_REG_1_Zstride_ADDR32] = 
        (0 << UNP1_ADDR_CTRL_ZW_REG_1_Wstride_SHAMT) |
        (unpB_ch1_z_stride << UNP1_ADDR_CTRL_ZW_REG_1_Zstride_SHAMT);

    // Math ALU_FORMAT_REG
    t6_mutex_acquire(mutex::REG_RMW);
    uint32_t alu_src_format = (0x0 << ALU_FORMAT_SPEC_REG_SrcA_val_SHAMT);

    constexpr uint32_t mask0 = (1 << (ALU_FORMAT_SPEC_REG_Dstacc_override_SHAMT + 1)) - 1;
    cfg_reg_rmw_tensix<
            ALU_FORMAT_SPEC_REG_SrcA_val_ADDR32, 
            ALU_FORMAT_SPEC_REG_SrcA_val_SHAMT, 
            mask0>(
        alu_src_format);

    alu_config_u alu_payload = {.val = 0};

    uint32_t fp32_dest_acc_en = is_fp32_dest_acc_en ? 1 : 0;

    alu_payload.f.ALU_FORMAT_SPEC_REG0_SrcA = (uint32_t)unpA_dst_format;
    alu_payload.f.ALU_FORMAT_SPEC_REG1_SrcB = 
        row_pool ? 
            ((uint32_t)DataFormat::Float16 | (exp_width << 2)) : 
            (uint32_t)unpB_dst_format;
    // FP32 accumulation and SFPU to read dest as FP32
    // NOTE: This assumes these config fields are adjacent and in same register!!
    static_assert(ALU_ACC_CTRL_Fp32_enabled_ADDR32 == ALU_FORMAT_SPEC_REG0_SrcA_ADDR32);
    static_assert(ALU_ACC_CTRL_Fp32_enabled_ADDR32 == ALU_ACC_CTRL_SFPU_Fp32_enabled_ADDR32);
    alu_payload.f.ALU_ACC_CTRL_Fp32_enabled = fp32_dest_acc_en;
    alu_payload.f.ALU_ACC_CTRL_SFPU_Fp32_enabled = fp32_dest_acc_en;
    alu_payload.f.ALU_ACC_CTRL_INT8_math_enabled = 
        (unpA_dst_format == DataFormat::Int8 || unpB_dst_format == DataFormat::Int8);

    constexpr uint32_t mask1 = 
        ALU_ACC_CTRL_INT8_math_enabled_MASK | 
        ALU_ACC_CTRL_SFPU_Fp32_enabled_MASK | 
        ALU_ACC_CTRL_Fp32_enabled_MASK | 
        ALU_FORMAT_SPEC_REG1_SrcB_MASK | 
        ALU_FORMAT_SPEC_REG0_SrcA_MASK;

    cfg_reg_rmw_tensix<ALU_FORMAT_SPEC_REG0_SrcA_ADDR32, 0, mask1>(alu_payload.val);

    cfg_reg_rmw_tensix<ALU_ROUNDING_MODE_Fpu_srnd_en_RMW>(srnd_fpu_en);

    t6_mutex_release(mutex::REG_RMW);

    // Set tile descriptor
    unpack_tile_descriptor_u tile_descriptor;
    for (uint32_t i = 0; i < TILE_DESC_SIZE; i++) {
       tile_descriptor.val[i] = 0;
    }
    tile_descriptor.f.in_data_format = (uint32_t)unpA_src_format;
    // Input tile is uncompressed
    tile_descriptor.f.uncompressed = 1; 
    // Not used for unpA as value is overriden by per context x_dim set below. Used for unpB
    tile_descriptor.f.x_dim = 0; 
    tile_descriptor.f.y_dim = 1;
    tile_descriptor.f.z_dim = unpA_num_faces;
    for (uint32_t i = 0; i < TILE_DESC_SIZE; i++) {
        cfg[THCON_SEC0_REG0_TileDescriptor_ADDR32 + i] = tile_descriptor.val[i];
    }
    tile_descriptor.f.in_data_format  = 
        row_pool ? (uint32_t)DataFormat::Float32 : (uint32_t)unpB_src_format;
    tile_descriptor.f.x_dim = unpB_face_r_dim * FACE_C_DIM;
    tile_descriptor.f.z_dim = unpB_num_faces;
    for (uint32_t i = 0; i < TILE_DESC_SIZE; i++) {
        cfg[THCON_SEC1_REG0_TileDescriptor_ADDR32 + i] = tile_descriptor.val[i];
    }

    // Set unpacker config
    unpack_config_u config;
    for (uint32_t i = 0; i < CONFIG_SIZE; i++) {
       config.val[i] = 0;
    }
    config.f.out_data_format = (uint32_t)unpA_dst_format;
    config.f.throttle_mode = 2;
    config.f.context_count = 0;
    config.f.haloize_mode = transpose_xy_srca_en ? 1 : 0;
    config.f.uncompress_cntx0_3 = 0xf;
    config.f.uncompress_cntx4_7 = 0xf;
    for (uint32_t i = 0; i < CONFIG_SIZE; i++) {
        cfg[THCON_SEC0_REG2_Out_data_format_ADDR32 + i] = config.val[i];
    }
    config.f.out_data_format = 
        row_pool ? 
            ((uint32_t)DataFormat::Float16 | (exp_width << 2)) : 
            (uint32_t)unpB_dst_format;
    config.f.haloize_mode = 0;

    for (uint32_t i = 0; i < CONFIG_SIZE; i++) {
        cfg[THCON_SEC1_REG2_Out_data_format_ADDR32 + i] = config.val[i];
    }

    uint32_t unpA_x_end = (unpA_face_r_dim == 0) ? 1 : (unpA_face_r_dim << 4) - 1;
    TTI_SETADCXX(p_setadc::UNP_A, unpA_x_end, 0x0);
    TTI_SETADCXX(p_setadc::UNP_B, (unpB_face_r_dim << 4) - 1, 0x0);

    // Program base address for all 2 sections
    // (each section address is loaded to corresponding context)
    // Load dummy data to unused location if face height is 0
    uint32_t Dest_cntx0_address = (unpA_face_r_dim == 0) ? 22 * 16 : 4 * 16;
    uint32_t Dest_cntx1_address = (unpA_face_r_dim == 0) ? 22 * 16 : 4 * 16;
    cfg[THCON_SEC0_REG5_Dest_cntx0_address_ADDR32] = 
        Dest_cntx0_address | (Dest_cntx1_address << 16);

    // Program unpacker0 per context x_dim (face size in l1)
    // Overrides value set by tile descriptor when thread override bit is set in unpack instruction
    uint32_t face_dim = unpA_face_r_dim * FACE_C_DIM;
    cfg[THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32] = face_dim | (face_dim << 16);

    constexpr uint32_t face_dim_16x16 = FACE_R_DIM * FACE_C_DIM;
    regfile[p_gpr_unpack::FACE_DIM_16x16] = (face_dim_16x16 / 1) | ((face_dim_16x16 / 1) << 16);
    regfile[p_gpr_unpack::FACE_DIM_8x16] = (face_dim_16x16 / 2) | ((face_dim_16x16 / 2) << 16);
    regfile[p_gpr_unpack::FACE_DIM_4x16] = (face_dim_16x16 / 4) | ((face_dim_16x16 / 4) << 16);
    regfile[p_gpr_unpack::FACE_DIM_2x16] = (face_dim_16x16 / 8) | ((face_dim_16x16 / 8) << 16);
    regfile[p_gpr_unpack::FACE_DIM_1x16] = (face_dim_16x16 / 16) | ((face_dim_16x16 / 16) << 16);
    sync_regfile_write(p_gpr_unpack::FACE_DIM_1x16);

    TTI_SETC16(SRCA_SET_Base_ADDR32, 0x4);

    // Enable address counter for unpacker ch1/dst address
    // final address is calculated as: Dest_cntx0/1_address + address_counter_ch1
    // used for face by face unpacking of entire tile into srcA
    cfg[UNP0_ADD_DEST_ADDR_CNTR_add_dest_addr_cntr_ADDR32] = 
        0x1 << UNP0_ADD_DEST_ADDR_CNTR_add_dest_addr_cntr_SHAMT;

    // Clear context ID
    reset_config_context();
}

void config_face_dim_impl(bool INSERT_FENCE, uint32_t UNP_SEL, uint32_t face_r_dim) {
    uint32_t flop_index = THCON_SEC0_REG5_Tile_x_dim_cntx0_ADDR32 - THCON_CFGREG_BASE_ADDR32;
    switch (face_r_dim) {
    case 1:
        TTI_SETADCXX(UNP_SEL, 1 * FACE_C_DIM - 1, 0x0);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::FACE_DIM_1x16);
        break;
    case 2:
        TTI_SETADCXX(UNP_SEL, 2 * FACE_C_DIM - 1, 0x0);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::FACE_DIM_2x16);
        break;
    case 4:
        TTI_SETADCXX(UNP_SEL, 4 * FACE_C_DIM - 1, 0x0);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::FACE_DIM_4x16);
        break;
    case 8:
        TTI_SETADCXX(UNP_SEL, 8 * FACE_C_DIM - 1, 0x0);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::FACE_DIM_8x16);
        break;
    default:
        TTI_SETADCXX(UNP_SEL, FACE_R_DIM * FACE_C_DIM - 1, 0x0);
        TTI_REG2FLOP(1, 0, 0, 0, flop_index, p_gpr_unpack::FACE_DIM_16x16);
        break;
    }

    if (INSERT_FENCE) {
        // Insert fence if reg2flop is followed by an unpack
        TTI_DMANOP; 
    }
}

} // namespace unpacker
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

