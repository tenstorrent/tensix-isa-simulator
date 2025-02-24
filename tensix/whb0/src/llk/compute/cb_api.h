// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef TRISC_PACK
#include "llk_io_pack.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_io_unpack.h"
#endif

BEGIN_NS_COMPUTE

inline void cb_wait_front(uint32_t cbid, uint32_t ntiles) {
    UNPACK((llk_wait_tiles(cbid, ntiles)));
}

inline void cb_pop_front(uint32_t cbid, uint32_t ntiles) {
    UNPACK((llk_pop_tiles(cbid, ntiles)));
}

inline void cb_reserve_back(uint32_t cbid, uint32_t ntiles) {
    PACK((llk_wait_for_free_tiles<false, false, false>(cbid, ntiles)));
}

inline void cb_push_back(uint32_t cbid, uint32_t ntiles) {
    PACK((llk_push_tiles<false, false>(cbid, ntiles)));
}

END_NS_COMPUTE

