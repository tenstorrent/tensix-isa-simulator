#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_eltwise_unary_datacopy.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_a.h"
#endif

BEGIN_NS_COMPUTE

#if 0 // TODO: Revise this
inline void copy_tile_to_dst_init_short_with_dt(uint32_t cbid) {
    UNPACK((llk_unpack_A_init<BroadcastType::NONE, false, EltwiseBinaryReuseDestType::NONE>()));
    UNPACK((llk_unpack_reconfig_data_format(1, cbid, 0, 0)));
    MATH((llk_math_eltwise_unary_datacopy_init<
        DataCopyType::A2D, BroadcastType::NONE>(0, 0, cbid)));
}
#endif

inline void copy_tile_to_dst_init_short() {
    UNPACK((llk_unpack_A_init<BroadcastType::NONE, false, EltwiseBinaryReuseDestType::NONE>()));
    MATH((llk_math_eltwise_unary_datacopy_init<DataCopyType::A2D, BroadcastType::NONE>()));
}

inline void copy_tile_init() {
    copy_tile_to_dst_init_short();
    PACK((llk_init_packer_dest_offset_registers<
        DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));
}

inline void copy_tile(uint32_t icb, uint32_t itile, uint32_t idst) {
    MATH((llk_math_eltwise_unary_datacopy<
        DataCopyType::A2D, BroadcastType::NONE, DstSync::SyncHalf>(idst)));
    UNPACK((llk_unpack_A(icb, itile)));
}

END_NS_COMPUTE

