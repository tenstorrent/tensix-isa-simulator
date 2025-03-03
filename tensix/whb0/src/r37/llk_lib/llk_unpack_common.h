// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h" // [-ISS]

#include "ckernel.h"
#include "ckernel_defs.h"
#include "debug/fw_debug.h"
#include "cunpack_common.h"
#include "llk_param_structs.h"
#include "llk_io_unpack.h"

#ifdef PERF_DUMP
#include "ckernel_perf_api.h"
#endif

using namespace ckernel;
using namespace ckernel::unpacker;

// ACHTUNG: Missing inline in original code (apparent bug)
inline void llk_zero_operand(std::uint32_t operand) {
    std::uint32_t input = get_operand_id(operand);

    TT_SETDMAREG(0, 0, 0, LO_16(p_gpr_unpack::OPERAND_OFFSET_ADDR));
    TT_SETDMAREG(0, 0, 0, HI_16(p_gpr_unpack::OPERAND_OFFSET_ADDR));

    std::uint32_t fifo_base_addr = cb_interface[input].fifo_limit - cb_interface[input].fifo_size;
    TT_SETDMAREG(0, LOWER_HALFWORD(fifo_base_addr), 0, LO_16(p_gpr_unpack::p_gpr_unpack::OPERAND_BASE_ADDR));
    TT_SETDMAREG(0, UPPER_HALFWORD(fifo_base_addr), 0, HI_16(p_gpr_unpack::p_gpr_unpack::OPERAND_BASE_ADDR));

    for (std::uint32_t i = 0; i < cb_interface[input].fifo_size; i++) {
        TTI_STOREIND(
            1,
            0,
            p_ind::LD_16B,
            LO_16(p_gpr_unpack::OPERAND_OFFSET_ADDR),
            p_ind::INC_16B,
            p_gpr_unpack::ZERO_0,
            p_gpr_unpack::OPERAND_BASE_ADDR);
    }
}

inline void llk_unpack_debug_dump(std::uint8_t *data, std::uint32_t byte_size) {
    debug_dump(data, byte_size);
}

inline void llk_unpack_debug_dump_seek(std::uint8_t offset) {
    debug_dump_seek(offset);
}

inline void llk_unpack_reconfig_data_format_srca_impl(std::uint32_t srca_operand_id)
{
    cfg_reg_rmw_tensix<THCON_SEC0_REG0_TileDescriptor_ADDR32, 0, 0x0f>(unpack_src_format[srca_operand_id]);
    cfg_reg_rmw_tensix<THCON_SEC0_REG2_Out_data_format_RMW>(unpack_dst_format[srca_operand_id]);
    TT_SETDMAREG(0, LOWER_HALFWORD(cb_interface[srca_operand_id].fifo_page_size), 0, LO_16(p_gpr_unpack::TILE_SIZE_A)); // update gpr which holds tile size A
}

inline void llk_unpack_reconfig_data_format_srcb_impl(std::uint32_t srcb_operand_id)
{
    cfg_reg_rmw_tensix<THCON_SEC1_REG0_TileDescriptor_ADDR32, 0, 0x0f>(unpack_src_format[srcb_operand_id]);
    cfg_reg_rmw_tensix<THCON_SEC1_REG2_Out_data_format_RMW>(unpack_dst_format[srcb_operand_id]);
    TT_SETDMAREG(0, LOWER_HALFWORD(cb_interface[srcb_operand_id].fifo_page_size), 0, LO_16(p_gpr_unpack::TILE_SIZE_B)); // update gpr which holds tile size B
}

inline void llk_unpack_reconfig_data_format_srca(const std::uint32_t srca_new_operand) {
    llk_unpack_reconfig_data_format_srca_impl(get_operand_id(srca_new_operand));
}

inline void llk_unpack_reconfig_data_format_srcb(const std::uint32_t srcb_new_operand) {
    llk_unpack_reconfig_data_format_srcb_impl(get_operand_id(srcb_new_operand));
}

inline void llk_unpack_reconfig_data_format_srca(const std::uint32_t srca_old_operand, const std::uint32_t srca_new_operand) {
    std::uint32_t old_srca_operand_id = get_operand_id(srca_old_operand);
    std::uint32_t new_srca_operand_id = get_operand_id(srca_new_operand);

    if((unpack_src_format[old_srca_operand_id] != unpack_src_format[new_srca_operand_id])) {
        llk_unpack_reconfig_data_format_srca_impl(new_srca_operand_id);
    }
}

inline void llk_unpack_reconfig_data_format_srcb(const std::uint32_t srcb_old_operand, const std::uint32_t srcb_new_operand) {
    std::uint32_t old_srcb_operand_id = get_operand_id(srcb_old_operand);
    std::uint32_t new_srcb_operand_id = get_operand_id(srcb_new_operand);

    if((unpack_src_format[old_srcb_operand_id] != unpack_src_format[new_srcb_operand_id])) {
        llk_unpack_reconfig_data_format_srcb_impl(new_srcb_operand_id);
    }
}

inline void llk_unpack_reconfig_data_format(const std::uint32_t srca_new_operand, const std::uint32_t srcb_new_operand) {
    llk_unpack_reconfig_data_format_srca(srca_new_operand);
    llk_unpack_reconfig_data_format_srcb(srcb_new_operand);
}

inline void llk_unpack_reconfig_data_format(const std::uint32_t srca_old_operand, const std::uint32_t srca_new_operand, const std::uint32_t srcb_old_operand, const std::uint32_t srcb_new_operand) {
    llk_unpack_reconfig_data_format_srca(srca_old_operand, srca_new_operand);
    llk_unpack_reconfig_data_format_srcb(srcb_old_operand, srcb_new_operand);
}

inline void llk_unpack_dbg_feature_disable(){
    reg_write(RISCV_DEBUG_REG_DBG_FEATURE_DISABLE, 1<<11); // Set debug feature disable bit 11
                                                           // workaround for bug https://yyz-gitlab.local.tenstorrent.com/tenstorrent/budabackend/-/issues/1372
}
