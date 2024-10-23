#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_reduce.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_ab.h"
#include "llk_unpack_reduce.h"
#endif

BEGIN_NS_COMPUTE

#if 0 // TODO: Revise this (eliminated macros REDUCE_OP, REDUCE_DIM)
template<bool at_start>
inline void reduce_init(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t icb, 
        uint32_t icb_scaler, 
        uint32_t ocb = 16) {
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_init<>(icb, icb_scaler)));
    UNPACK((llk_unpack_AB_hw_configure_disaggregated(icb, icb_scaler)));

    MATH((llk_math_reduce_init<REDUCE_OP, REDUCE_DIM, MATH_FIDELITY>()));
    MATH((llk_math_pack_sync_init<SYNC>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_reduce_config_v2<REDUCE_DIM, at_start>(ocb)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SYNC, DstTileFaceLayout::RowMajor, false>()));
}

inline void reduce_init_short(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t icb, 
        uint32_t icb_scaler, 
        uint32_t ocb = 16) {
    UNPACK((llk_unpack_AB_init<>(icb, icb_scaler)));
    MATH((llk_math_reduce_init<REDUCE_OP, REDUCE_DIM, MATH_FIDELITY>()));
    PACK((llk_pack_reduce_config_v2<REDUCE_DIM, false>(ocb)));
}

template<bool at_start>
inline void reduce_init_delta(PoolType reduce_op, ReduceDim dim, uint32_t ocb = 16) {
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<>(0, 1)));
    MATH((llk_math_reduce_init<REDUCE_OP, REDUCE_DIM, MATH_FIDELITY>()));
    PACK((llk_pack_reduce_config_v2<REDUCE_DIM, at_start>(ocb)));
}

inline void reduce_revert_delta(uint32_t ocb = 16) {
    PACK((llk_pack_reduce_config_v2<REDUCE_DIM, false, true>(ocb)));
}

inline void reduce_tile(
        PoolType reduce_op, 
        ReduceDim dim, 
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_reduce<REDUCE_OP, REDUCE_DIM, MATH_FIDELITY>(idst)));
    UNPACK((llk_unpack_AB(icb0, icb1, itile0, itile1)));
}
#endif

template<PoolType reduce_op, ReduceDim reduce_dim, bool at_start>
inline void reduce_init(
        uint32_t icb, 
        uint32_t icb_scaler, 
        uint32_t ocb = 16) {
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_init<>(icb, icb_scaler)));
    UNPACK((llk_unpack_AB_hw_configure_disaggregated(icb, icb_scaler)));

    MATH((llk_math_reduce_init<reduce_op, reduce_dim, MATH_FIDELITY>()));
    MATH((llk_math_pack_sync_init<SYNC>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_reduce_config_v2<reduce_dim, at_start>(ocb)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SYNC, DstTileFaceLayout::RowMajor, false>()));
}

template<PoolType reduce_op, ReduceDim reduce_dim>
inline void reduce_init_short(
        uint32_t icb, 
        uint32_t icb_scaler, 
        uint32_t ocb = 16) {
    UNPACK((llk_unpack_AB_init<>(icb, icb_scaler)));
    MATH((llk_math_reduce_init<reduce_op, reduce_dim, MATH_FIDELITY>()));
    PACK((llk_pack_reduce_config_v2<reduce_dim, false>(ocb)));
}

template<PoolType reduce_op, ReduceDim reduce_dim, bool at_start>
inline void reduce_init_delta(uint32_t ocb = 16) {
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<>(0, 1)));
    MATH((llk_math_reduce_init<reduce_op, reduce_dim, MATH_FIDELITY>()));
    PACK((llk_pack_reduce_config_v2<reduce_dim, at_start>(ocb)));
}

template<ReduceDim reduce_dim>
inline void reduce_revert_delta(uint32_t ocb = 16) {
    PACK((llk_pack_reduce_config_v2<reduce_dim, false, true>(ocb)));
}

template<PoolType reduce_op, ReduceDim reduce_dim>
inline void reduce_tile(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    MATH((llk_math_reduce<reduce_op, reduce_dim, MATH_FIDELITY>(idst)));
    UNPACK((llk_unpack_AB(icb0, icb1, itile0, itile1)));
}

END_NS_COMPUTE

