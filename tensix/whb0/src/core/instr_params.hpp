// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

// Hand-coded parameter encoding for various common instructions

struct p_setrwc {
    constexpr static uint32_t
        CLR_A        = 0x1,
        CLR_B        = 0x2,
        CLR_AB       = 0x3,
        CLR_NONE     = 0x0;

    constexpr static uint32_t
        SET_A        = 0x1,
        SET_B        = 0x2,
        SET_AB       = 0x3,
        SET_D        = 0x4,
        SET_AD       = 0x5,
        SET_BD       = 0x6,
        SET_ABD      = 0x7,
        SET_F        = 0x8,
        SET_A_F      = 0x9,
        SET_B_F      = 0xa,
        SET_AB_F     = 0xb,
        SET_D_F      = 0xc,
        SET_AD_F     = 0xd,
        SET_BD_F     = 0xe,
        SET_ABD_F    = 0xf;

    constexpr static uint32_t
        CR_A         = 0x1,
        CR_B         = 0x2,
        CR_AB        = 0x3,
        CR_D         = 0x4,
        CR_AD        = 0x5,
        CR_BD        = 0x6,
        CR_ABD       = 0x7,
        C_TO_CR_MODE = 0x8;
};

struct p_setibrwc {
    constexpr static uint32_t
        SET_BIAS = 0x0,
        INC_BIAS = 0x1;

    constexpr static uint32_t
        CR_NONE  = 0x0,
        CR_BIAS  = 0x1;
};

struct p_unpacr {
    constexpr static uint32_t
        RAREFYB_DISABLE       = 0x0,
        RAREFYB_ENABLE        = 0x1;

    constexpr static uint32_t
        TILE0_ADDRCNT_CONTEXT = 0,  // Address counter context for tile 0
        TILE1_ADDRCNT_CONTEXT = 0,  // Address counter context for tile 1
        TILE2_ADDRCNT_CONTEXT = 1,  // Address counter context for tile 2
        TILE3_ADDRCNT_CONTEXT = 1,  // Address counter context for tile 3
        TILE0_CFG_CONTEXT     = 0,  // Config context for tile 0
        TILE1_CFG_CONTEXT     = 0,  // Config context for tile 1
        TILE2_CFG_CONTEXT     = 0,  // Config context for tile 2
        TILE3_CFG_CONTEXT     = 0,  // Config context for tile 3
        AUTO_INC_CONTEXT      = 1;  // Auto increment config context (max value set through unpacker config command)
};

struct p_unpacr_nop {
    constexpr static uint32_t
        UNP_POP                = 0b000,
        UNP_NOP                = 0b010;

    constexpr static uint32_t
        UNP_ZEROSRC            = 0b001,
        UNP_NEGINFSRC          = 0b101;

    constexpr static uint32_t UNP_SET_DVALID = 0b111;

    constexpr static uint32_t 
        UNP_ZEROSRC_RESET_ALL_BANKS    = 0b1001,       // default is clear current bank
        UNP_ZEROSRC_STALL_RESET_WR_RDY = 0b10001,
        UNP_ZEROSRC_SET_DVALID         = 0b1000001;

    constexpr static uint32_t
        UNP0                  = 0x0,
        UNP1                  = 0x1;
};

struct p_srcb {
    constexpr static uint32_t
        FORWARD_PASS  = 0x0,
        BACKWARD_PASS = 0x1;
};

struct p_setadc {
    constexpr static uint32_t
        UNP0  = 0b001,
        UNP1  = 0b010,
        PAC   = 0b100;

    constexpr static uint32_t
        SET_X = 0,
        SET_Y = 1,
        SET_Z = 2,
        SET_W = 3;

    constexpr static uint32_t
        CH_0  = 0,
        CH_1  = 1;
};

struct p_pacr {
    constexpr static uint32_t
        P_ZERO_OUTPUT_DISABLED = 0x0,
        P_ZERO_OUTPUT_ENABLED  = 0x1;
};

struct p_ind {
    constexpr static uint32_t
        HIER_REGFILE = 0x0,
        HIER_L1      = 0x1;

    constexpr static uint32_t
        INC_NONE     = 0x0,
        INC_2B       = 0x1,
        INC_4B       = 0x2,
        INC_16B      = 0x3;

    constexpr static uint32_t
        LD_16B       = 0,
        LD_32bit     = 1,
        LD_16bit     = 2,
        LD_8bit      = 3;
};

struct p_mova2d {
    constexpr static uint32_t
        MATH_HALO_ROWS = 0x0,
        MOV_1_ROW      = 0x0,
        MOV_8_ROWS     = 0x2;
};

struct p_movd2a {
    constexpr static uint32_t
        MOV_1_ROW  = 0x0,
        MOV_4_ROWS = 0x2;
};

struct p_movb2d {
    constexpr static uint32_t
        SRC_ZERO_OFFSET          = 0x0,
        SRC_ROW16_OFFSET         = 0x10;

    constexpr static uint32_t
        MOV_1_ROW                = 0x0,
        MOV_1_ROW_D0_BRCST       = 0x1,
        MOV_8_ROW_BRCST          = 0x2,
        MOV_8_ROW_BRCST_D0_BRCST = 0x3,
        MOV_4_ROWS               = 0x4,
        MOV_4_ROWS_D0_BRCST      = 0x5;
};

struct p_movd2b {
    constexpr static uint32_t
        SRC_ZERO_OFFSET  = 0x0,
        SRC_ROW16_OFFSET = 0x10;

    constexpr static uint32_t
        MOV_1_ROW        = 0x0,
        MOV_4_ROWS       = 0x2;
};

struct p_movb2a {
    constexpr static uint32_t
        SRCA_ZERO_OFFSET  = 0x0,
        SRCB_ZERO_OFFSET  = 0x0,
        SRCB_ROW16_OFFSET = 0x10;

    constexpr static uint32_t
        MOV_1_ROW         = 0x0,
        MOV_4_ROWS        = 0x2;
};

struct p_stall {
    // What to stall on
    constexpr static uint32_t
        NONE          = 0x0,
        THCON         = 0x1,
        UNPACK0       = 0x2,
        UNPACK1       = 0x4,
        UNPACK        = UNPACK0 | UNPACK1,
        PACK0         = 0x8,
        PACK1         = 0x10,
        PACK2         = 0x20,
        PACK3         = 0x40,
        PACK          = PACK0 | PACK1 | PACK2 | PACK3,
        MATH          = 0x80,
//        SEM_ZERO      = 0x20,
//        SEM_MAX       = 0x40,
        SRCA_CLR      = 0x100,
        SRCB_CLR      = 0x200,
        SRCA_VLD      = 0x400,
        SRCB_VLD      = 0x800,
        XMOV          = 0x1000,
        TRISC_CFG     = 0x2000,
        SFPU1         = 0x4000,
        WAIT_SFPU     = 0x4000,
        ALL_THREAD_RES = THCON | UNPACK | PACK | MATH | XMOV;

    // What to stall
    constexpr static uint32_t
        STALL_TDMA    = 0x1,
        STALL_SYNC    = 0x2,
        STALL_PACK    = 0x4,
        STALL_UNPACK  = 0x8,
//        STALL_XSEARCH = 0x10,
        STALL_XMOV    = 0x10,
        STALL_THCON   = 0x20,
        STALL_MATH    = 0x40,
        STALL_CFG     = 0x80,
        STALL_SFPU    = 0x100,
        STALL_THREAD  = 0x1ff;

    constexpr static uint32_t
        STALL_ON_ZERO = 0x1,
        STALL_ON_MAX  = 0x2;

    constexpr static uint32_t
        SEMAPHORE_0    = 0x1,
        SEMAPHORE_1    = 0x2,
        SEMAPHORE_2    = 0x4,
        SEMAPHORE_3    = 0x8,
        SEMAPHORE_4    = 0x10,
        SEMAPHORE_5    = 0x20,
        SEMAPHORE_6    = 0x40,
        SEMAPHORE_7    = 0x80,
        SEMAPHORE_BIAS = SEMAPHORE_4;
};

struct p_zeroacc {
    constexpr static uint32_t
        CLR_SPECIFIC     = 0b000,
        CLR_16           = 0b001,
        CLR_HALF         = 0b010,
        CLR_ALL          = 0b011,
        CLR_SPECIFIC_32B = 0b100,
        CLR_16_32B       = 0b101,
        CLR_HALF_32B     = 0b110,
        CLR_ALL_32B      = 0b111;
};

struct p_zerosrc {
    constexpr static uint32_t
        CLR_A  = 0x1,
        CLR_B  = 0x2,
        CLR_AB = 0x3;
};

struct p_shiftx {
    constexpr static uint32_t
        SHIFT_1      = 0x0,
        SHIFT_2      = 0x1,
        SHIFT_4      = 0x2,
        SHIFT_8      = 0x3;

    constexpr static uint32_t
        RESERVED0    = 0x0,
        RESERVED1    = 0x1,
        RIGHT_AWAY0  = 0x2,
        LEFT_TOWARD0 = 0x3;
};

struct p_cfg {
    constexpr static uint32_t
        WRCFG_128b = 0x1,
        WRCFG_32b  = 0x0;
};

struct p_alu {
    constexpr static uint32_t
        AND = 0x0,
        OR  = 0x1,
        XOR = 0x2;
};

struct p_gpool {
    constexpr static uint32_t
        DIM_1X16  = 0x0,
        DIM_16X16 = 0x1,
        INDEX_DIS = 0x0,
        INDEX_EN  = 0x1;
};

struct p_elwise {
    constexpr static uint32_t
        SRCB_NO_BCAST     = 0x0,
        DEST_ACCUM_EN     = 0x1,
        DEST_ACCUM_DIS    = 0x0,
        SRCB_BCAST_COL    = 0x1,
        SRCB_BCAST_ROW    = 0x2,
        SRCB_BCAST_ALL    = 0x3;
};

struct p_sfpu {
    // SFPU registers
    constexpr static uint32_t
        LREG0 = 0,
        LREG1 = 1,
        LREG2 = 2,
        LREG3 = 3,
        LREG4 = 4,
        LREG5 = 5,
        LREG6 = 6,
        LREG7 = 7;

    // HW provided constants
    constexpr static uint32_t
        LCONST_0_8373 = 8,
        LCONST_0 = 9,
        LCONST_1 = 10;

    // Programmable constants
    constexpr static uint32_t
        LREG11 = 11,
        LREG12 = 12,
        LREG13 = 13,
        LREG14 = 14;

    constexpr static uint32_t
        LTILEID = 15;

    constexpr static uint32_t
        kCONST_1_FP16B = 0x3F80,
        kCONST_1_FP16A = 0x3C00,
        kCONST_0 = 0x0000,
        kCONST_Exp_8Bit = 0,
        kCONST_Exp_5Bit = 1;
};

struct p_exp {
    constexpr static uint32_t
        FRAC_BITS = 3,
        C23_73 = 0x4340, // Based on FRAC_BITS
        // ADJ_EXP = -0x4300 + 0x003F
        //  0x4300 : 0100 0011 0000 0000
        //  0x003F : 0000 0000 0011 1111
        // -0x4300 : 1011 1101 0000 0000
        // ADJ_EXP : 1011 1101 0011 1111 (-0x4300 + 0x003F = 0xBD3F)
        ADJ_EXP = 0xBD3F;
}; 

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

