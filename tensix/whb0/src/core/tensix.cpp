// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <limits>

#include "core/tensix_types.hpp"
#include "core/addr_mod.hpp"
#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Tensix
//

Tensix::Tensix(Memory *memory):
        m_memory(memory) {
    reset();
}

Tensix::~Tensix() { }

void Tensix::reset() {
    m_thread_id = 0;

    float *srca = m_srca[0];
    for (uint32_t i = 0; i < SRCA_ROWS * ROW_SIZE; i++) {
        srca[i] = 0.0f;
    }
    float *srcb = m_srcb[0];
    for (uint32_t i = 0; i < SRCB_ROWS * ROW_SIZE; i++) {
        srcb[i] = 0.0f;
    }
    float *dest = m_dest[0];
    for (uint32_t i = 0; i < DEST_ROWS * ROW_SIZE; i++) {
        dest[i] = 0.0f;
    }
    m_srca_dvalid = false;
    m_srcb_dvalid = false;

    m_srca_rwc = 0;
    m_srca_rwc_cr = 0;
    m_srcb_rwc = 0;
    m_srcb_rwc_cr = 0;
    m_dest_rwc = 0;
    m_dest_rwc_cr = 0;
    m_fidelity = 0;
    m_bias = 0;

    U32 *lreg = m_lreg[0];
    for (uint32_t i = 0; i < LREG_COUNT * LROW_SIZE; i++) {
        lreg[i].f = 0.0f;
    }
    U32 *lreg8 = m_lreg[8];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        lreg8[i].f = 0.837300003f; // 0x3f56594b
    }
    U32 *lreg10 = m_lreg[10];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        lreg10[i].f = 1.0f;
    }
    // ACHTUNG: Setting LREG11 is SFPI-specific (should this be done elsewhere?)
    U32 *lreg11 = m_lreg[11];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        lreg11[i].f = -1.0f;
    }
    U32 *lreg15 = m_lreg[15];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        lreg15[i].i = 2 * i;
    }
    m_cc = ~uint32_t(0);
    m_cc_en = ~uint32_t(0);
    for (uint32_t i = 0; i < CC_STACK_SIZE; i++) {
        m_cc_stack[i] = 0;
        m_cc_en_stack[i] = 0;
    }
    m_cc_sp = 0;

    uint32_t *dma_reg = m_dma_reg[0];
    for (uint32_t i = 0; i < 3 * DMA_REG_COUNT; i++) {
        dma_reg[i] = 0;
    }

    uint32_t *thd_reg = m_thd_reg[0];
    for (uint32_t i = 0; i < 3 * THD_REG_COUNT; i++) {
        thd_reg[i] = 0;
    }
    uint32_t *cfg_reg = m_cfg_reg[0];
    for (uint32_t i = 0; i < 2 * CFG_REG_COUNT; i++) {
        cfg_reg[i] = 0;
    }
    for (uint32_t i = 0; i < 3; i++) {
        m_cfg_state_id[i] = 0;
    }

    for (uint32_t i = 0; i < 8; i++) {
        m_mutex[i] = MUTEX_NONE;
    }
    for (uint32_t i = 0; i < 8; i++) {
        m_sem[i] = 0;
        m_sem_max[i] = 0;
    }

    m_dest_offset = 0;
    m_haloize_mode = 0;
    m_tileize_mode = 0;
    m_shift_amount = 0;

    m_tile_x_dim = 0;
    m_ovrd_ch0_x_stride = 0;
    m_ovrd_ch0_y_stride = 0;
    m_ovrd_ch0_z_stride = 0;
    m_ovrd_ch0_w_stride = 0;
    m_ovrd_ch1_y_stride = 0;
    m_ovrd_ch1_z_stride = 0;
    m_ovrd_ch1_w_stride = 0;

    for (int i = 0; i < 3; i++) {
        AddrCtrl &adc = m_adc[i];
        adc.tile_in_data_format = DataFormat(0);
        adc.tile_x_dim = 0;
        adc.tile_y_dim = 0;
        adc.tile_z_dim = 0;
        adc.tile_w_dim = 0;
        adc.ch0_x_stride = 0;
        adc.ch0_y_stride = 0;
        adc.ch0_z_stride = 0;
        adc.ch0_w_stride = 0;
        adc.ch1_y_stride = 0;
        adc.ch1_z_stride = 0;
        adc.ch1_w_stride = 0;
        for (int k = 0; k < 8; k++) {
            CfgCntx &cntx = adc.cntx[k];
            cntx.base_addr = 0;
            cntx.offset_addr = 0;
        }
        adc.unp_cntx_offset = 0;
        adc.ch0_x = 0;
        adc.ch0_y = 0;
        adc.ch0_z = 0;
        adc.ch0_w = 0;
        adc.ch1_x = 0;
        adc.ch1_y = 0;
        adc.ch1_z = 0;
        adc.ch1_w = 0;
        adc.x_end = 0;
        adc.ch0_x_cr = 0;
        adc.ch0_y_cr = 0;
        adc.ch0_z_cr = 0;
        adc.ch0_w_cr = 0;
        adc.ch1_x_cr = 0;
        adc.ch1_y_cr = 0;
        adc.ch1_z_cr = 0;
        adc.ch1_w_cr = 0;
        adc.update_strides = false;
    }

    m_pack_fp32_dest = false;
    m_pack_src_format = DataFormat(0); 
    m_pack_dst_format = DataFormat(0);
    m_pack_x_dim = 0; 
    m_pack_y_dim = 0; 
    m_pack_z_dim = 0;
    m_pack_ch0_y_stride = 0;
    m_pack_ch0_z_stride = 0;
    m_pack_ch0_w_stride = 0;
    m_pack_ch1_x_stride = 0;
    m_pack_ch1_y_stride = 0;
    m_pack_ch1_z_stride = 0;
    m_pack_ch1_w_stride = 0;
    m_pack_l1_offset = 0;
    m_pack_edge_offset_mask[0] = 0; 
    m_pack_edge_offset_mask[1] = 0; 
    m_pack_edge_offset_mask[2] = 0; 
    m_pack_edge_offset_mask[3] = 0; 
    m_pack_edge_row_set_select[0] = 0;
    m_pack_edge_row_set_select[1] = 0;
    m_pack_edge_row_set_select[2] = 0;
    m_pack_edge_row_set_select[3] = 0;
    m_tile_row_set_mapping[0] = 0;
    m_tile_row_set_mapping[1] = 0;
    m_tile_row_set_mapping[2] = 0;
    m_tile_row_set_mapping[3] = 0;
    m_relu_type = 0;
    m_relu_threshold = 0;
    m_pack_dest_offset[0] = 0;
    m_pack_dest_offset[1] = 0;
    m_pack_dest_offset[2] = 0;
    m_pack_dest_offset[3] = 0;
    m_pack_dest_addr = 0;
    m_pack_l1_acc = false;
    m_pack_update_strides = true;

    m_sem_math_pack = 0;

    m_neginf = -std::numeric_limits<float>::infinity();
}

// diagnostics methods

float *Tensix::dest_row_data(uint32_t row_index) {
    assert(row_index < DEST_ROWS);
    return m_dest[row_index];
}

void Tensix::diag_dump_dest(uint32_t row_index, uint32_t row_count) {
    printf("# Dest[%d : %d]\n", int(row_index), int(row_count));
    float *ptr = dest_row_data(row_index);
    for (uint32_t i = 0; i < 2 * row_count; i++) {
        printf("[%3d]", int(i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %g", ptr[i * 8 + k]);
        }
        printf("\n");
    }
}

void Tensix::diag_dump_reg(uint32_t reg, uint32_t start, uint32_t count) {
    static const char *reg_names[] = { "SrcA", "SrcB", "Dest" };
    if (reg > 2) {
        return;
    }
    count /= 8;
    printf("# %s[%d : %d]\n", reg_names[reg], int(start), int(count * 8));
    float *ptr = (reg == 0) ? m_srca[0] : (reg == 1) ? m_srcb[0] : m_dest[0];
    ptr += start;
    for (uint32_t i = 0; i < count; i++) {
        printf("[%3d]", int(start + i * 8));
        for (uint32_t k = 0; k < 8; k++) {
            printf(" %g", ptr[i * 8 + k]);
        }
        printf("\n");
    }
}

void Tensix::get_lreg_int(uint32_t lreg, uint32_t *data) {
    U32 *p = m_lreg[lreg];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        data[i] = p[i].i;
    }
}

void Tensix::get_lreg_float(uint32_t lreg, float *data) {
    U32 *p = m_lreg[lreg];
    for (uint32_t i = 0; i < LROW_SIZE; i++) {
        data[i] = p[i].f;
    }

}

//
//    NOP: Do nothing and consume an instruction slot and cycle
//

void Tensix::NOP() {
    // nothing to do
}

//
//    Private helpers
//

uint32_t Tensix::get_df_item_size(DataFormat df) {
    DataFormat base_df = DataFormat(uint32_t(df) & 0x3);
    return (base_df == DataFormat::Float32) ? 4 : 
        (base_df == DataFormat::Float16) ? 2 : 1;
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

