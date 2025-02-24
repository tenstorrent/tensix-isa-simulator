// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "core/thread.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

//
//    OpType
//

enum class OpType {
    ADDDMAREG,
    ADDRCRXY,
    ADDRCRZW,
    APOOL3S1,
    APOOL3S2,
    ATCAS,
    ATGETM,
    ATINCGET,
    ATINCGETPTR,
    ATRELM,
    ATSWAP,
    BITWOPDMAREG,
    CLEARDVALID,
    CLREXPHIST,
    CMPDMAREG,
    CONV3S1,
    CONV3S2,
    DMANOP,
    DOTPV,
    ELWADD,
    ELWMUL,
    ELWSUB,
    FLUSHDMA,
    GAPOOL,
    GATESRCRST,
    GMPOOL,
    INCADCXY,
    INCADCZW,
    INCRWC,
    LOADIND,
    LOADREG,
    MFCONV3S1,
    MOP,
    MOP_CFG,
    MOVA2D,
    MOVB2A,
    MOVB2D,
    MOVD2A,
    MOVD2B,
    MOVDBGA2D,
    MPOOL3S1,
    MPOOL3S2,
    MULDMAREG,
    MVMUL,
    NOP,
    PACR,
    PACR_SETREG,
    RAREB,
    RDCFG,
    REG2FLOP,
    REPLAY,
    RMWCIB0,
    RMWCIB1,
    RMWCIB2,
    RMWCIB3,
    RSTDMA,
    SEMGET,
    SEMINIT,
    SEMPOST,
    SEMWAIT,
    SETADC,
    SETADCXX,
    SETADCXY,
    SETADCZW,
    SETASHRMH,
    SETASHRMH0,
    SETASHRMH1,
    SETASHRMV,
    SETC16,
    SETDMAREG,
    SETDVALID,
    SETIBRWC,
    SETPKEDGOF,
    SETRWC,
    SFPABS,
    SFPADD,
    SFPADDI,
    SFPAND,
    SFPCAST,
    SFPCOMPC,
    SFPCONFIG,
    SFPDIVP2,
    SFPENCC,
    SFPEXEXP,
    SFPEXMAN,
    SFPIADD,
    SFPLOAD,
    SFPLOADI,
    SFPLOADMACRO,
    SFPLUT,
    SFPLUTFP32,
    SFPLZ,
    SFPMAD,
    SFPMOV,
    SFPMUL,
    SFPMULI,
    SFPNOP,
    SFPNOT,
    SFPOR,
    SFPPOPC,
    SFPPUSHC,
    SFPSETCC,
    SFPSETEXP,
    SFPSETMAN,
    SFPSETSGN,
    SFPSHFT,
    SFPSHFT2,
    SFPSTORE,
    SFPSWAP,
    SFPTRANSP,
    SFPXOR,
    SFP_STOCH_RND,
    SHIFTDMAREG,
    SHIFTXA,
    SHIFTXB,
    STALLWAIT,
    STOREIND,
    STOREREG,
    SUBDMAREG,
    TBUFCMD,
    TRNSPSRCA,
    TRNSPSRCB,
    UNPACR,
    UNPACR_NOP,
    WRCFG,
    XMOV,
    ZEROACC,
    ZEROSRC
};

//
//    Op
//

struct Op {
    OpType type;
    uint32_t args[16];
};

//
//    Public functions
//

inline constexpr Op make_op(
        OpType type,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0,
        uint32_t arg8 = 0,
        uint32_t arg9 = 0,
        uint32_t arg10 = 0,
        uint32_t arg11 = 0,
        uint32_t arg12 = 0,
        uint32_t arg13 = 0,
        uint32_t arg14 = 0,
        uint32_t arg15 = 0) {
    return Op {
        type,
        {
            arg0,
            arg1,
            arg2,
            arg3,
            arg4,
            arg5,
            arg6,
            arg7,
            arg8,
            arg9,
            arg10,
            arg11,
            arg12,
            arg13,
            arg14,
            arg15
        }
    };
}

bool is_valid(uint32_t val, uint32_t wid);
void run_thread_op(core::Thread *thread, const Op &op);

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

