// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
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

inline void unary_op_init_common(uint32_t icb) {
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_A_init<BroadcastType::NONE, false, EltwiseBinaryReuseDestType::NONE>()));
    UNPACK((llk_unpack_A_hw_configure_disaggregated<>(icb)));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(16)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SYNC, DstTileFaceLayout::RowMajor, false>()));

    MATH((llk_math_eltwise_unary_datacopy_init<A2D, BroadcastType::NONE>(0, 0, icb)));
    MATH((llk_math_pack_sync_init<SYNC>()));
}

inline void init_sfpu(uint32_t icb) {
    unary_op_init_common(icb);
}

END_NS_COMPUTE

