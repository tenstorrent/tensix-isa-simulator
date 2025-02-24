// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_eltwise_binary.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_ab.h"
#endif

BEGIN_NS_COMPUTE

inline void sub_tiles_bcast_cols(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWSUB, 
            BroadcastType::COL, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::COL>(icb0, icb1, itile0, itile1)));
}

inline void mul_tiles_bcast_cols(
        uint32_t icb0, uint32_t icb1, uint32_t itile0, uint32_t itile1, uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWMUL, 
            BroadcastType::COL, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::COL>(icb0, icb1, itile0, itile1)));
}

inline void mul_tiles_bcast_rows(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWMUL, 
            BroadcastType::ROW, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::ROW>(icb0, icb1, itile0, itile1)));
}

inline void add_tiles_bcast_rows(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWADD, 
            BroadcastType::ROW, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::ROW>(icb0, icb1, itile0, itile1)));
}

inline void add_tiles_bcast_cols(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWADD, 
            BroadcastType::COL, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::COL>(icb0, icb1, itile0, itile1)));
}

template<EltwiseBinaryType tBcastOp, BroadcastType tBcastDim>
void init_bcast(uint32_t icb0, uint32_t icb1, uint32_t ocb = 16) {
    if constexpr (tBcastOp == ELWMUL) {
        MATH((llk_math_eltwise_binary_init<tBcastOp, tBcastDim, MATH_FIDELITY>()));
    } else {
        MATH((llk_math_eltwise_binary_init<tBcastOp, tBcastDim>()));
    }

    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_init<tBcastDim>(icb0, icb1)));
    UNPACK((llk_unpack_AB_hw_configure_disaggregated(icb0, icb1)));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(ocb)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));

    MATH((llk_math_pack_sync_init<DstSync::SyncHalf>()));
}

template<EltwiseBinaryType tBcastOp, BroadcastType tBcastDim>
inline void any_tiles_bcast(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            tBcastOp, 
            tBcastDim, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<tBcastDim>(icb0, icb1, itile0, itile1)));
}

template<BroadcastType tBcastDim>
inline void add_tiles_bcast(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) { 
    any_tiles_bcast<EltwiseBinaryType::ELWADD, tBcastDim>(icb0, icb1, itile0, itile1, idst);
}

template<BroadcastType tBcastDim>
inline void sub_tiles_bcast(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) { 
    any_tiles_bcast<EltwiseBinaryType::ELWSUB, tBcastDim>(icb0, icb1, itile0, itile1, idst); 
}

template<BroadcastType tBcastDim>
inline void mul_tiles_bcast(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) { 
    any_tiles_bcast<EltwiseBinaryType::ELWMUL, tBcastDim>(icb0, icb1, itile0, itile1, idst); 
}

inline void add_bcast_rows_init_short() {
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::ROW>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::ROW>(0, 1)));
}

inline void add_bcast_cols_init_short() {
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::COL>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::COL>(0, 1)));
}

inline void mul_tiles_bcast_scalar_init_short() {
    // TODO(AP)
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWMUL, BroadcastType::SCALAR, MATH_FIDELITY>())); 
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::SCALAR>(0, 1)));
}

inline void mul_tiles_bcast_scalar(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWMUL, 
            BroadcastType::SCALAR, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
    UNPACK((llk_unpack_AB<BroadcastType::SCALAR>(icb0, icb1, itile0, itile1)));
}


inline void mul_bcast_cols_init_short() {
    // TODO(AP)
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWMUL, BroadcastType::COL, MATH_FIDELITY>())); 
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::COL>(0, 1)));
}

inline void mul_bcast_rows_init_short() {
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWMUL, BroadcastType::ROW, MATH_FIDELITY>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::ROW>(0, 1)));
}

inline void sub_bcast_cols_init_short() {
    // TODO(AP)
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWSUB, BroadcastType::COL, MATH_FIDELITY>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::COL>(0, 1)));
}

END_NS_COMPUTE

