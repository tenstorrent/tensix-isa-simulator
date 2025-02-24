// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "ckernel_core.h"

#include "llk_defs.h"
#include "llk_unpack_A.h"
#include "llk_unpack_AB.h"
#include "llk_unpack_AB_matmul.h"
#include "llk_unpack_reduce.h"
#include "llk_unpack_tilize.h"
#include "llk_unpack_untilize.h"

namespace syntax {

using namespace ckernel;

//
//    Dummy code just to test compilation of templates
//

void test_llk_unpack_a() {
    llk_unpack_A_mop_config(false, 0);
    llk_unpack_A_hw_configure_disaggregated(0);
    llk_unpack_A_init();
    llk_unpack_A(0, 0);
}

void test_llk_unpack_ab() {
    llk_unpack_AB_mop_config();
    llk_unpack_AB_hw_configure_disaggregated(0, 1);
    llk_unpack_AB_init(0, 1);
    llk_unpack_AB(0, 1, 0, 0);
}

void test_llk_unpack_ab_matmul() {
    llk_unpack_AB_matmul_mop_config(
        false,  // transpose
        8,      // ct_dim
        8,      // rt_dim
        16,     // kt_dim
        false); // partial_face
    llk_unpack_AB_matmul_hw_configure_disaggregated(0, 1);
    llk_unpack_AB_matmul_init(0, 1);
    llk_unpack_AB_matmul(0, 1, 0, 0);
}

void test_llk_unpack_reduce() {
    llk_unpack_reduce_mop_config<PoolType::SUM, ReduceDim::REDUCE_ROW>();
    llk_unpack_reduce_hw_configure_disaggregated<PoolType::SUM, ReduceDim::REDUCE_ROW>(0, 1.0f);
    llk_unpack_reduce_init<PoolType::SUM, ReduceDim::REDUCE_ROW>();
    llk_unpack_reduce<PoolType::SUM, ReduceDim::REDUCE_ROW>(0, 0);
}

void test_llk_unpack_tilize() {
    llk_unpack_tilize_mop_config(16);
    llk_unpack_tilize_hw_configure_disaggregated(0, 8);
    llk_unpack_tilize_init(0, 8);
    llk_unpack_tilize_block(0, 8);
    llk_unpack_tilize_uninit();
}

void test_llk_unpack_untilize() {
    llk_unpack_untilize_mop_config();
    llk_unpack_untilize_hw_configure_disaggregated(0);
    llk_unpack_untilize_init(0);
    llk_unpack_untilize(0, 8);
    llk_unpack_untilize_uninit(0);
}

} // namespace syntax

