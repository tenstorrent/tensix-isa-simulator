#pragma once

#include <cstdint>

#include "common.h"

#ifdef TRISC_MATH
#include "llk_math_eltwise_binary.h"
#endif
#ifdef TRISC_UNPACK
#include "llk_unpack_AB.h"
#endif

BEGIN_NS_COMPUTE

inline void binary_op_init_common(uint32_t icb0, uint32_t icb1) {
    UNPACK((llk_setup_operands()));
    UNPACK((llk_unpack_AB_init<BroadcastType::NONE>(icb0, icb1)));
    UNPACK((llk_unpack_AB_hw_configure_disaggregated(icb0, icb1)));

    MATH((llk_math_pack_sync_init<SYNC>()));

    PACK((llk_pack_init()));
    PACK((llk_pack_hw_configure_disaggregated<false>(16)));
    PACK((llk_setup_outputs()));
    PACK((llk_pack_dest_init<SYNC, DstTileFaceLayout::RowMajor, false>()));
}

inline void mul_tiles_init_f() { 
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWMUL, BroadcastType::NONE, MATH_FIDELITY>())); 
}

inline void mul_tiles_init() {
    MATH((llk_math_eltwise_binary_init<
        EltwiseBinaryType::ELWMUL, BroadcastType::NONE, MATH_FIDELITY>()));
    PACK((llk_init_packer_dest_offset_registers<
        DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::NONE>(0, 1)));
}

inline void add_tiles_init_nof() { 
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::NONE>())); 
}

inline void add_tiles_init() {
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWADD, BroadcastType::NONE>()));
    PACK((llk_init_packer_dest_offset_registers<
        DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::NONE>(0, 1)));
}

inline void sub_tiles_init_nof() { 
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWSUB, BroadcastType::NONE>())); 
}

inline void sub_tiles_init() {
    MATH((llk_math_eltwise_binary_init<EltwiseBinaryType::ELWSUB, BroadcastType::NONE>()));
    PACK((llk_init_packer_dest_offset_registers<
        DstSync::SyncHalf, DstTileFaceLayout::RowMajor, false>()));
    // FIXME: API Update needed in compute kernel?
    UNPACK((llk_unpack_AB_init<BroadcastType::NONE>(0, 1)));
}

inline void mul_tiles(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    UNPACK((llk_unpack_AB(icb0, icb1, itile0, itile1)));
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWMUL, 
            BroadcastType::NONE, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
}

inline void add_tiles(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    UNPACK((llk_unpack_AB(icb0, icb1, itile0, itile1)));
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWADD, 
            BroadcastType::NONE, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
}

inline void sub_tiles(
        uint32_t icb0, 
        uint32_t icb1, 
        uint32_t itile0, 
        uint32_t itile1, 
        uint32_t idst) {
    UNPACK((llk_unpack_AB(icb0, icb1, itile0, itile1)));
    MATH((llk_math_eltwise_binary<
            EltwiseBinaryType::ELWSUB, 
            BroadcastType::NONE, 
            DstSync::SyncHalf, 
            MATH_FIDELITY, 
            EltwiseBinaryReuseDestType::NONE>(
        icb0, icb1, idst)));
}

#if 0 // TODO: Revise this (eliminated macros ELTWISE_OP, ELTWISE_OP_CODE)
template<bool full_init = false>
inline void binary_op_specific_init(int op_code) {
    // TODO(AP): better naming
#ifdef ELTWISE_OP
    if constexpr (full_init) {
        // TODO(AP): pass an enum probably
        if constexpr (ELTWISE_OP_CODE == 0) {
            add_tiles_init();
        } else if constexpr (ELTWISE_OP_CODE == 1) {
            sub_tiles_init();
        } else if constexpr (ELTWISE_OP_CODE == 2) {
            mul_tiles_init();
        }
    } else {
        // TODO(AP): pass an enum probably
        if constexpr (ELTWISE_OP_CODE == 0) {
            add_tiles_init_nof();
        } else if constexpr (ELTWISE_OP_CODE == 1) {
            sub_tiles_init_nof();
        } else if constexpr (ELTWISE_OP_CODE == 2) {
            mul_tiles_init_f();
        }
    }
#endif
}
#endif

template<EltwiseBinaryType op_code, bool full_init = false>
inline void binary_op_specific_init() {
    if constexpr (full_init) {
        // TODO(AP): pass an enum probably
        if constexpr (op_code == EltwiseBinaryType::ELWADD) {
            add_tiles_init();
        } else if constexpr (op_code == EltwiseBinaryType::ELWSUB) {
            sub_tiles_init();
        } else if constexpr (op_code == EltwiseBinaryType::ELWMUL) {
            mul_tiles_init();
        }
    } else {
        if constexpr (op_code == EltwiseBinaryType::ELWADD) {
            add_tiles_init_nof();
        } else if constexpr (op_code == EltwiseBinaryType::ELWSUB) {
            sub_tiles_init_nof();
        } else if constexpr (op_code == EltwiseBinaryType::ELWMUL) {
            mul_tiles_init_f();
        }
    }
}

END_NS_COMPUTE

