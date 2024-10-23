#pragma once

#include <cstdint>

#include "system/cfg_defines.h"

#include "fw_debug.h"
#include "ckernel_ops.h"
#include "ckernel_thread.h"
#include "ckernel_asm.h"
#include "ckernel_instr_params.h"
#include "ckernel_defs.h"
#include "ckernel.h"

#ifndef SFPU_OP_PARAM
#define SFPU_OP_PARAM 0
#endif

#ifndef FUSE_SQRT_RECIP
#define FUSE_SQRT_RECIP 0
#endif

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {
namespace math {

namespace ckernel = llk::api;

constexpr uint32_t DstTileSize[3] = {
    64,     // 32x32 tile shape
    32,     // 32x16, 16x32 tile shape
    16      // 16x16 tile shape
};
constexpr uint32_t DstTileSizeLog2[3] = {
    6,     // 32x32 tile shape
    5,     // 32x16, 16x32 tile shape
    4      // 16x16 tile shape
};

// split replay buffer usage between fpu/sfpu
// fist 16 for sfpu, next 16 for fpu
constexpr uint32_t replay_buf_offset = 16; 

inline void reset_counters(uint32_t setrwc) {
    TTI_SETRWC(p_setrwc::CLR_NONE, 0, 0, 0, 0, setrwc);
}

inline void incr_counters(uint32_t incr_a, uint32_t incr_b, uint32_t incr_d, uint32_t incr_cr) {
    FWASSERT("Value exceeds RWC_A width of 4 bits", incr_a < 16);
    FWASSERT("Value exceeds RWC_B width of 4 bits", incr_b < 16);
    FWASSERT("Value exceeds RWC_D width of 4 bits", incr_d < 16);
    FWASSERT("Value exceeds RWC_CR width of 4 bits", incr_cr < 16);
    TT_INCRWC(incr_cr, incr_d, incr_b, incr_a);
}

inline void move_d2a_fixed_face(uint32_t addrmod) {
    // MOVD2A for a whole face assumes unpacker will set a dummy data_valid, so we want to wait on that
    TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::SRCA_VLD); 
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  0, addrmod, p_movd2a::MOV_4_ROWS,  0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  4, addrmod, p_movd2a::MOV_4_ROWS,  4);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  8, addrmod, p_movd2a::MOV_4_ROWS,  8);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 12, addrmod, p_movd2a::MOV_4_ROWS, 12);
}

inline void move_d2b_fixed_face(uint32_t addrmod) {
    // MOVD2B for a whole face assumes unpacker will set a dummy data_valid, so we want to wait on that
    TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::SRCB_VLD); 
    TTI_MOVD2B(0, p_movd2b::SRC_ZERO_OFFSET +  0, addrmod, p_movd2b::MOV_4_ROWS,  0);
    TTI_MOVD2B(0, p_movd2b::SRC_ZERO_OFFSET +  4, addrmod, p_movd2b::MOV_4_ROWS,  4);
    TTI_MOVD2B(0, p_movd2b::SRC_ZERO_OFFSET +  8, addrmod, p_movd2b::MOV_4_ROWS,  8);
    TTI_MOVD2B(0, p_movd2b::SRC_ZERO_OFFSET + 12, addrmod, p_movd2b::MOV_4_ROWS, 12);
}

inline void move_d2a_row_broadcast_fixed_face(uint32_t addrmod) {
    // // Seems to make things 200 clocks slower. Really shouldn't though.
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  0, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  1, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  2, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  3, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  4, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  5, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  6, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  7, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  8, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS +  9, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 10, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 11, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 12, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 13, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 14, addrmod, p_movd2a::MOV_1_ROW, 0);
    TTI_MOVD2A(0, p_mova2d::MATH_HALO_ROWS + 15, addrmod, p_movd2a::MOV_1_ROW, 0);
}

inline void move_a2d_fixed_face(uint32_t addrmod) {
    TTI_MOVA2D(0, p_mova2d::MATH_HALO_ROWS, addrmod, p_mova2d::MOV_8_ROWS, 0);
    TTI_MOVA2D(0, p_mova2d::MATH_HALO_ROWS, addrmod, p_mova2d::MOV_8_ROWS, 0);
}

template <uint32_t SrcReg>
inline void wait_bank_valid() {
    if constexpr (SrcReg == Srcs::SrcA) {
        TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::SRCA_VLD);
    } else {
        TTI_STALLWAIT(p_stall::STALL_MATH, p_stall::SRCB_VLD);
    }
}

template <uint32_t SrcReg>
inline void clear_bank_valid() {
    if constexpr (SrcReg == Srcs::SrcA) {
        TTI_SETRWC(p_setrwc::CLR_A, 0, 0, 0, 0, p_setrwc::SET_A);
    } else {
        TTI_SETRWC(p_setrwc::CLR_B, 0, 0, 0, 0, p_setrwc::SET_B);
    }
}

inline void update_dest_offset_id() {
    // ping-pong between 0 and 1
    dest_offset_id = 1 - dest_offset_id;
}

inline uint32_t get_dest_buffer_base() {
    return (dest_offset_id != 0) ? DEST_REGISTER_HALF_SIZE : 0x0;
}

inline void wait_math_semaphores() {
    // wait while math semaphore is on max, no room to write math results
    TTI_SEMWAIT(
        p_stall::STALL_MATH|p_stall::STALL_SFPU, 
        semaphore::t6_sem(semaphore::MATH_PACK), 
        p_stall::STALL_ON_MAX);
}

inline void set_math_semaphores() {
    // Tell packer that it has something to pack
    t6_semaphore_post<p_stall::MATH|p_stall::WAIT_SFPU>(semaphore::MATH_PACK);
}

template <DstTileLayout layout, DstTileShape tile_shape>
inline void set_dst_write_addr(uint32_t tile_index) {
    if constexpr (layout == DstTileLayout::Default) {
        uint32_t dst_index = tile_index << DstTileSizeLog2[tile_shape];
        dst_index = dst_index + get_dest_buffer_base();
        TT_SETC16(DEST_TARGET_REG_CFG_MATH_Offset_ADDR32, dst_index);
    } else {
        // FIXME MT: add this mapping for other layout
    }
}

// Programming a dst write addr offset that gets added to base
//
inline void clear_dst_reg_addr() {
    TTI_SETRWC(p_setrwc::CLR_NONE, 0, 0, 0, 0, p_setrwc::SET_D);
}

inline void set_addr_mod_base() {
    // set addr mod base (use addr mods 4..7)
    TTI_SETC16(ADDR_MOD_SET_Base_ADDR32, 1);
}

inline void clear_addr_mod_base() {
    // clear addr mod base (use addr mods 0..3)
    TTI_SETC16(ADDR_MOD_SET_Base_ADDR32, 0);
}

inline void math_dest_wait() {
    TTI_SEMWAIT(
        p_stall::STALL_MATH|p_stall::STALL_SFPU, 
        semaphore::t6_sem(semaphore::MATH_PACK), 
        p_stall::STALL_ON_MAX);
}

inline void dest_section_flip() {
    update_dest_offset_id();
    uint32_t base_addr = get_dest_buffer_base();
    TTI_STALLWAIT(p_stall::STALL_CFG, p_stall::MATH|p_stall::SFPU1);
    TT_SETC16(DEST_TARGET_REG_CFG_MATH_Offset_ADDR32, base_addr);
}

template <DstStart Dst>
inline void set_dest_section_base() {
    uint32_t base_addr;
    if constexpr (Dst == DstStart::StartZero) {
        base_addr = 0;
    } else {
        base_addr = DEST_REGISTER_HALF_SIZE;
    }
    TT_SETC16(DEST_TARGET_REG_CFG_MATH_Offset_ADDR32, base_addr);
}

inline uint32_t get_operand_id(uint32_t operand) {
    const int INTERMEDIATE_BASE_ID = 24;
    const int OPERAND_BASE_ID = 0;
    return (operand >= INTERMEDIATE_BASE_ID) ? operand - 8 : operand - OPERAND_BASE_ID;
}

// FIXME: Added this --> Should be generated by compile trisc?
constexpr uint32_t math_tile_dims[32][2] = {
    {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32},
    {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32},
    {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32},
    {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32}
};
constexpr uint32_t math_tile_num_faces[32] = {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
constexpr uint32_t math_partial_face[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

inline uint32_t get_math_tile_dim(uint32_t operand_id, uint32_t tile_dim) {
    return math_tile_dims[operand_id][tile_dim];
}

inline constexpr uint32_t get_num_faces(uint32_t operand_id) {
    return math_tile_num_faces[operand_id];
}

inline constexpr uint32_t get_partial_face(uint32_t operand_id) {
    return math_partial_face[operand_id];
}

} // namespace math
} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

