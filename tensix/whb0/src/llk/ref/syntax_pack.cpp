// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "ckernel_core.h"

#include "llk_defs.h"
#include "llk_pack_common.h"
#include "llk_pack.h"

BEGIN_NS_LLK

//
//    Dummy code just to test compilation of templates
//

void test_llk_pack_common() {
    llk_packer_wait_for_math_done();
    llk_pack_dest_section_done<DstSync::SyncFull>();
    llk_pack_dest_init<DstSync::SyncFull>();
    llk_init_packer_dest_offset_registers<DstSync::SyncFull, DstTileFaceLayout::ColMajor>();
}

void test_llk_pack() {
    llk_pack_mop_config(16);
    llk_pack_hw_configure_disaggregated(16);
    llk_pack_reduce_hw_configure_disaggregated<false, PoolType::SUM, ReduceDim::REDUCE_ROW>(16);
    llk_pack_init(16);
    llk_pack(0, 16);
    llk_pack_reduce_config_v2<ReduceDim::REDUCE_ROW>(16);
}

END_NS_LLK

