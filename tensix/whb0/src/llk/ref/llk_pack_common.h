#pragma once

#include <cstdint>

#include "ckernel_core.h"
#include "ckernel_addrmod.h"
#include "ckernel_instr_params.h"
#include "ckernel_gpr_map.h"
#include "ckernel_defs.h"
#include "ckernel.h"
#include "cpack_common.h"

#include "llk_defs.h"
#include "llk_param_structs.h"

BEGIN_NS_LLK

using namespace ckernel;
using namespace ckernel::packer;

// wait until math is done and has produced something to pack
inline void llk_packer_wait_for_math_done() {
    TTI_SEMWAIT(
        p_stall::STALL_TDMA, 
        semaphore::t6_sem(semaphore::MATH_PACK), 
        p_stall::STALL_ON_ZERO);
}

// Tell math that it can write again
template <uint32_t WaitRes = p_stall::NONE>
inline void llk_packer_set_math_semaphore() {
    // Indicate that packer is done and header is written into L1
    t6_semaphore_get<WaitRes>(semaphore::MATH_PACK);
}

// Wait for all writes to complete in L1 (header + data)
// Tell math it can write again
// Clear dest
template <DstSync Dst, bool is_fp32_dest_acc_en = false>
inline void llk_pack_dest_section_done() {
    constexpr bool clear_dest = (Dst != DstSync::SyncTile16);

    if constexpr (clear_dest){
        // wait for pack to finish
        TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::PACK);  

        if constexpr (Dst == DstSync::SyncFull) {
            constexpr uint32_t CLEAR_MODE = 
                is_fp32_dest_acc_en ? p_zeroacc::CLR_ALL_32B : p_zeroacc::CLR_ALL;
            TT_ZEROACC(CLEAR_MODE, ADDR_MOD_1, 0);
        } else {
            static_assert(Dst == DstSync::SyncHalf || Dst == DstSync::SyncTile2);
            constexpr uint32_t CLEAR_MODE = 
                is_fp32_dest_acc_en ? p_zeroacc::CLR_HALF_32B : p_zeroacc::CLR_HALF;
            TT_ZEROACC(CLEAR_MODE, ADDR_MOD_1, dest_offset_id % 2);
        }
    }

    // Note: we should have already stalled math in non-tile dest modes due to clearing
    constexpr uint32_t WaitRes = (Dst == DstSync::SyncTile16) ? p_stall::PACK : p_stall::NONE;

    // Tell math that it can write again
    llk_packer_set_math_semaphore<WaitRes>();

    constexpr bool flip_dest = (Dst == DstSync::SyncHalf || Dst == DstSync::SyncTile2);

    if constexpr (flip_dest){
        flip_packer_dest_offset_id();
        select_packer_dest_registers<Dst>();
    }
}

template <
    DstSync Dst, 
    DstTileFaceLayout FaceLayout = RowMajor, 
    bool untilize = false, 
    bool is_fp32_dest_acc_en = false>
inline void llk_pack_dest_init() {
    tensix_sync();
    reset_dest_offset_id();
    init_packer_dest_offset_registers<FaceLayout, untilize, is_fp32_dest_acc_en>();
    select_packer_dest_registers<Dst>();
    packer_addr_counter_init();
    pack_sync_tile_dst_ptr = 0;
}

template <DstSync Dst, DstTileFaceLayout FaceLayout, bool untilize = false>
inline void llk_init_packer_dest_offset_registers(uint32_t pack_output = 0) {
    // wait for pack to finish
    TTI_STALLWAIT(p_stall::STALL_TDMA|p_stall::STALL_THCON, p_stall::PACK);  
    constexpr uint32_t
        DEST_OFFSET_LO = p_gpr_pack::DEST_OFFSET_LO,
        DEST_OFFSET_HI = p_gpr_pack::DEST_OFFSET_HI;
    if constexpr (untilize) {
       if constexpr (FaceLayout == ColMajor) {
          // Packer0 :  0, 32,  1, 33 ...  7, 39
          // Packer1 :  8, 40,  9, 41 ... 15, 47
          // Packer2 : 16, 48, 17, 49 ... 23, 55
          // Packer3 : 23, 56, 24, 57 ... 31, 63
          TT_SETDMAREG(0, 0x000 + 0x00, 0, LO_16(DEST_OFFSET_LO + 0));
          TT_SETDMAREG(0, 0x000 + 0x08, 0, LO_16(DEST_OFFSET_LO + 1));
          TT_SETDMAREG(0, 0x000 + 0x10, 0, LO_16(DEST_OFFSET_LO + 2));
          TT_SETDMAREG(0, 0x000 + 0x18, 0, LO_16(DEST_OFFSET_LO + 3));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(DEST_OFFSET_HI + 0));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x08, 0, LO_16(DEST_OFFSET_HI + 1));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(DEST_OFFSET_HI + 2));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x18, 0, LO_16(DEST_OFFSET_HI + 3));
       } else {
          // Packer0 :  0, 16,  1, 17 ...  7, 23
          // Packer1 :  8, 24,  9, 25 ... 15, 31
          // Packer2 : 32, 48, 33, 49 ... 39, 55
          // Packer3 : 40, 56, 41, 57 ... 47, 63
          TT_SETDMAREG(0, 0x000 + 0x00, 0, LO_16(DEST_OFFSET_LO + 0));
          TT_SETDMAREG(0, 0x000 + 0x08, 0, LO_16(DEST_OFFSET_LO + 1));
          TT_SETDMAREG(0, 0x000 + 0x20, 0, LO_16(DEST_OFFSET_LO + 2));
          TT_SETDMAREG(0, 0x000 + 0x28, 0, LO_16(DEST_OFFSET_LO + 3));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(DEST_OFFSET_HI + 0));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x08, 0, LO_16(DEST_OFFSET_HI + 1));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(DEST_OFFSET_HI + 2));
          TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x28, 0, LO_16(DEST_OFFSET_HI + 3));
       }
    } else {
       if constexpr (FaceLayout == ColMajor) {
           TT_SETDMAREG(0, 0x00, 0, LO_16(DEST_OFFSET_LO + 0));
           TT_SETDMAREG(0, 0x20, 0, LO_16(DEST_OFFSET_LO + 1));
           TT_SETDMAREG(0, 0x10, 0, LO_16(DEST_OFFSET_LO + 2));
           TT_SETDMAREG(0, 0x30, 0, LO_16(DEST_OFFSET_LO + 3));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(DEST_OFFSET_HI + 0));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(DEST_OFFSET_HI + 1));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(DEST_OFFSET_HI + 2));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x30, 0, LO_16(DEST_OFFSET_HI + 3));
       } else {
           // Default to row major layout
           TT_SETDMAREG(0, 0x00, 0, LO_16(DEST_OFFSET_LO + 0));
           TT_SETDMAREG(0, 0x10, 0, LO_16(DEST_OFFSET_LO + 1));
           TT_SETDMAREG(0, 0x20, 0, LO_16(DEST_OFFSET_LO + 2));
           TT_SETDMAREG(0, 0x30, 0, LO_16(DEST_OFFSET_LO + 3));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x00, 0, LO_16(DEST_OFFSET_HI + 0));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x10, 0, LO_16(DEST_OFFSET_HI + 1));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x20, 0, LO_16(DEST_OFFSET_HI + 2));
           TT_SETDMAREG(0, DEST_REGISTER_HALF_SIZE + 0x30, 0, LO_16(DEST_OFFSET_HI + 3));
       }
    }
    select_packer_dest_registers<Dst>();
}

inline void llk_setup_outputs() {
    // originally in "llk_io_pack.h"
    // nothing to do in similator case
    // input CBs are set via simulator framework
}

END_NS_LLK

