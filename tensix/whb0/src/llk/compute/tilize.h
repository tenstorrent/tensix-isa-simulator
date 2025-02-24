// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_eltwise_unary_datacopy.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_tilize.h"
#endif

BEGIN_NS_COMPUTE

inline void tilize_init(uint32_t icb, uint32_t block, uint32_t ocb = 16) {
    MATH((llk_math_eltwise_unary_datacopy_init<
        DataCopyType::A2D, BroadcastType::NONE>(0, 0, icb)));

    MATH((llk_math_pack_sync_init<DstSync::SyncHalf>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(ocb)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));

    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_tilize_hw_configure_disaggregated<>(icb, block)));
    UNPACK((llk_unpack_tilize_init(icb, block)));
}

inline void tilize_init_short(uint32_t icb, uint32_t block) {
    MATH((llk_math_eltwise_unary_datacopy_init<
        DataCopyType::A2D, BroadcastType::NONE>(0, 0, icb)));
    UNPACK((llk_unpack_tilize_init(icb, block)));
}

inline void tilize_block(uint32_t icb, uint32_t block, uint32_t ocb) {
    UNPACK((llk_unpack_tilize_block(icb, block)));

    for (uint32_t t = 0; t < block; t++) {
        // Acquire dst
        MATH((llk_math_wait_for_dest_available<SYNC>()));
        PACK((llk_packer_wait_for_math_done()));

        // Datacopy
        MATH((llk_math_eltwise_unary_datacopy<
            DataCopyType::A2D, BroadcastType::NONE, DstSync::SyncHalf>(0)));
        PACK((llk_pack<false, SYNC, false >(0, ocb)));

        // Release dest
        MATH((llk_math_dest_section_done<SYNC>()));
        PACK((llk_pack_dest_section_done<SYNC>()));
    }
}

inline void tilize_uninit() {
    UNPACK((llk_unpack_tilize_uninit()));
}

END_NS_COMPUTE

