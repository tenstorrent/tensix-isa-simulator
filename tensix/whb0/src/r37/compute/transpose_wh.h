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
#include "llk_unpack_A.h"
#endif

BEGIN_NS_COMPUTE

inline void transpose_wh_init(uint32_t icb) {
    MATH((llk_math_eltwise_unary_datacopy_init<
        DataCopyType::A2D, BroadcastType::NONE>(true, true, icb)));

    MATH((llk_math_pack_sync_init<DstSync::SyncHalf>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(16)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));

    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_A_init<
        BroadcastType::NONE, true, EltwiseBinaryReuseDestType::NONE>(true, true)));
    UNPACK((llk_unpack_A_hw_configure_disaggregated<>(0, true)));
}

inline void transpose_wh_tile(uint32_t icb, uint32_t itile, uint32_t idst) {
    UNPACK((llk_unpack_A<BroadcastType::NONE, false>(icb, itile)));

    MATH((llk_math_eltwise_unary_datacopy<
        DataCopyType::A2D, BroadcastType::NONE, DstSync::SyncHalf>(idst)));
}

END_NS_COMPUTE

