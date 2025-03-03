// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/pack.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace basic {

//
//    Pack
//

void Pack::pack_reduce_hw_configure_disaggregated(
        bool untilize, 
        PoolType type, 
        ReduceDim dim, 
        bool is_fp32_dest_acc_en, 
        ReluType relu_type, 
        uint32_t relu_threshold,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
#if 0 // TODO: Revise this
    configure_pack(
        is_fp32_dest_acc_en, 
        out_src_format,
        out_dst_format,
        relu_type, 
        relu_threshold);
#endif
    configure_pack(
        is_fp32_dest_acc_en, 
        untilize,
        out_src_format,
        out_dst_format,
        relu_type, 
        relu_threshold);

    set_pack_edge_offset_mask(0, 0x0); 
    if (dim == ReduceDim::REDUCE_ROW) {
        set_pack_edge_offset_mask(1, 0x0001); 
        if (untilize) {
            set_pack_edge_row_set_select(0, 1);
            set_pack_edge_row_set_select(1, 1);
            set_pack_edge_row_set_select(2, 1);
            set_pack_edge_row_set_select(3, 1);
            set_tile_row_set_mapping(1, 0x11111111);
        } else {
            set_pack_edge_row_set_select(0, 1);
            set_pack_edge_row_set_select(2, 1);
            set_tile_row_set_mapping(1, 0x55555555);
        }
    } else if (dim == ReduceDim::REDUCE_SCALAR) {
        set_pack_edge_row_set_select(0, 1);
        set_pack_edge_offset_mask(1, 0x0001); 
        set_tile_row_set_mapping(1, 0x00000001);
    } else {
        set_pack_edge_row_set_select(0, 1);
        set_pack_edge_row_set_select(1, 1);
        set_pack_edge_offset_mask(1, 0xffff); 
        if (untilize) {
            set_tile_row_set_mapping(1, 0x00000005);
        } else {
            set_tile_row_set_mapping(1, 0x00000001);
        }
    }
}

void Pack::pack_reduce_config_v2(
        ReduceDim dim, 
        bool at_kernel_start, 
        bool revert, 
        bool is_fp32_dest_acc_en,
        DataFormat out_src_format,
        DataFormat out_dst_format) {
    // This is provisional version
    // TODO (Wait until definitive version in original code)

    if (at_kernel_start) {
#if 0 // TODO: Revise this
        configure_pack(
            is_fp32_dest_acc_en, 
            out_src_format,
            out_dst_format,
            ReluType::NO_RELU, 
            0);
#endif
        configure_pack(
            is_fp32_dest_acc_en, 
            false,
            out_src_format,
            out_dst_format,
            ReluType::NO_RELU, 
            0);
    }

    if (revert) {
        if (!at_kernel_start) {
            // same as in 'configure_pack'
            set_pack_edge_offset_mask(0, 0xffff); 
            set_pack_edge_row_set_select(0, 0);
            set_pack_edge_row_set_select(1, 0);
            set_pack_edge_row_set_select(2, 0);
            set_pack_edge_row_set_select(3, 0);
            set_tile_row_set_mapping(0, 0x0);
        }
    } else {
        // same as in 'pack_reduce_hw_configure_disaggregated': unify?
        set_pack_edge_offset_mask(0, 0x0); 
        if (dim == ReduceDim::REDUCE_ROW) {
            set_pack_edge_offset_mask(1, 0x0001); 
            set_pack_edge_row_set_select(0, 1);
            set_pack_edge_row_set_select(2, 1);
            set_tile_row_set_mapping(1, 0x55555555);
        } else if (dim == ReduceDim::REDUCE_SCALAR) {
            set_pack_edge_row_set_select(0, 1);
            set_pack_edge_offset_mask(1, 0x0001); 
            set_tile_row_set_mapping(1, 0x00000001);
        } else {
            set_pack_edge_row_set_select(0, 1);
            set_pack_edge_row_set_select(1, 1);
            set_pack_edge_offset_mask(1, 0xffff); 
            set_tile_row_set_mapping(1, 0x00000001);
        }
    }
}

} // namespace basic
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

