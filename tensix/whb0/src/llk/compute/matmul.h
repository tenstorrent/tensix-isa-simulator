// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_matmul.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_ab_matmul.h"
#endif

BEGIN_NS_COMPUTE

inline void mm_init(
        uint32_t in0_cb_id = 0, 
        uint32_t in1_cb_id = 1, 
        uint32_t out_cb_id = 16, 
        uint32_t transpose = 0) {
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_matmul_init(in0_cb_id, in1_cb_id)));
    UNPACK((llk_unpack_AB_matmul_hw_configure_disaggregated(in0_cb_id, in1_cb_id)));

    MATH((llk_math_matmul_init<MATH_FIDELITY>(in0_cb_id, in1_cb_id)));
    MATH((llk_math_pack_sync_init<SYNC>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(out_cb_id)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SYNC, DstTileFaceLayout::RowMajor, false>()));
    // TODO(AP): ZM-only kernel
    PACK((llk_init_packer_dest_offset_registers<SyncHalf, DstTileFaceLayout::RowMajor, false>()));
}

inline void mm_init_once() {
    // nothing to do
}

inline void matmul_tiles(
        uint32_t c_in0, 
        uint32_t c_in1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst, 
        bool transpose) {
    UNPACK((llk_unpack_AB_matmul(c_in0, c_in1, itile0, itile1)));
    MATH((llk_math_matmul<MATH_FIDELITY>(idst, transpose)));
}

#if 0 // TODO: Revise this
inline void mm_init_short_with_dt(uint32_t cbid, uint32_t transpose = 0) {
    UNPACK((llk_unpack_AB_matmul_init(cbid, 1)));
    UNPACK((llk_unpack_reconfig_data_format(cbid, 1, 0, 0)));
    MATH((llk_math_matmul_init<MATH_FIDELITY>(cbid, 1)));
}
#endif

inline void mm_init_short(uint32_t transpose = 0) {
    MATH((llk_math_matmul_init<MATH_FIDELITY>(0, 1, 0)));
    UNPACK((llk_unpack_AB_matmul_init(0, 1)));
}

END_NS_COMPUTE

