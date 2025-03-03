// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

BEGIN_NS_COMPUTE

inline void acquire_dst(DstMode mode) {
    MATH((llk_math_wait_for_dest_available<SYNC>()));

    PACK((llk_packer_wait_for_math_done()));
}

inline void release_dst(DstMode mode) {
    MATH((llk_math_dest_section_done<SYNC>()));

    PACK((llk_pack_dest_section_done<SYNC>()));
}

END_NS_COMPUTE

