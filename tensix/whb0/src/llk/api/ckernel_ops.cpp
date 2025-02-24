// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/thread.hpp"

#include "ckernel_ops.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

//
//    Public functions
//

bool is_valid(uint32_t val, uint32_t wid) {
    uint32_t mask = (1 << wid) - 1;
    return ((val & mask) == val); 
}

void run_thread_op(core::Thread *thread, const Op &op) {
    const uint32_t *ap = op.args;
    switch(op.type) {
    case OpType::ADDDMAREG:
        thread->ADDDMAREG(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::ADDRCRXY:
        thread->ADDRCRXY(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::ADDRCRZW:
        thread->ADDRCRZW(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::APOOL3S1:
        thread->APOOL3S1(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::APOOL3S2:
        thread->APOOL3S2(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::ATCAS:
        thread->ATCAS(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::ATGETM:
        thread->ATGETM(ap[0]);
        break;
    case OpType::ATINCGET:
        thread->ATINCGET(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::ATINCGETPTR:
        thread->ATINCGETPTR(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5], ap[6]);
        break;
    case OpType::ATRELM:
        thread->ATRELM(ap[0]);
        break;
    case OpType::ATSWAP:
        thread->ATSWAP(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::BITWOPDMAREG:
        thread->BITWOPDMAREG(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::CLEARDVALID:
        thread->CLEARDVALID(ap[0], ap[1]);
        break;
    case OpType::CLREXPHIST:
        thread->CLREXPHIST();
        break;
    case OpType::CMPDMAREG:
        thread->CMPDMAREG(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::CONV3S1:
        thread->CONV3S1(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::CONV3S2:
        thread->CONV3S2(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::DMANOP:
        thread->DMANOP();
        break;
    case OpType::DOTPV:
        thread->DOTPV(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::ELWADD:
        thread->ELWADD(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::ELWMUL:
        thread->ELWMUL(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::ELWSUB:
        thread->ELWSUB(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::FLUSHDMA:
        thread->FLUSHDMA(ap[0]);
        break;
    case OpType::GAPOOL:
        thread->GAPOOL(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::GATESRCRST:
        thread->GATESRCRST(ap[0], ap[1]);
        break;
    case OpType::GMPOOL:
        thread->GMPOOL(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::INCADCXY:
        thread->INCADCXY(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::INCADCZW:
        thread->INCADCZW(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::INCRWC:
        thread->INCRWC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::LOADIND:
        thread->LOADIND(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::LOADREG:
        thread->LOADREG(ap[0], ap[1]);
        break;
    case OpType::MFCONV3S1:
        thread->MFCONV3S1(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::MOP:
        thread->MOP(ap[0], ap[1], ap[2]);
        break;
    case OpType::MOP_CFG:
        thread->MOP_CFG(ap[0]);
        break;
    case OpType::MOVA2D:
        thread->MOVA2D(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::MOVB2A:
        thread->MOVB2A(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::MOVB2D:
        thread->MOVB2D(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::MOVD2A:
        thread->MOVD2A(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::MOVD2B:
        thread->MOVD2B(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::MOVDBGA2D:
        thread->MOVDBGA2D(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::MPOOL3S1:
        thread->MPOOL3S1(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::MPOOL3S2:
        thread->MPOOL3S2(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::MULDMAREG:
        thread->MULDMAREG(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::MVMUL:
        thread->MVMUL(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::NOP:
        thread->NOP();
        break;
    case OpType::PACR:
        thread->PACR(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5], ap[6]);
        break;
    case OpType::PACR_SETREG:
        thread->PACR_SETREG(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5], ap[6]);
        break;
    case OpType::RAREB:
        thread->RAREB();
        break;
    case OpType::RDCFG:
        thread->RDCFG(ap[0], ap[1]);
        break;
    case OpType::REG2FLOP:
        thread->REG2FLOP(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::REPLAY:
        thread->REPLAY(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::RMWCIB0:
        thread->RMWCIB0(ap[0], ap[1], ap[2]);
        break;
    case OpType::RMWCIB1:
        thread->RMWCIB1(ap[0], ap[1], ap[2]);
        break;
    case OpType::RMWCIB2:
        thread->RMWCIB2(ap[0], ap[1], ap[2]);
        break;
    case OpType::RMWCIB3:
        thread->RMWCIB3(ap[0], ap[1], ap[2]);
        break;
    case OpType::RSTDMA:
        thread->RSTDMA();
        break;
    case OpType::SEMGET:
        thread->SEMGET(ap[0]);
        break;
    case OpType::SEMINIT:
        thread->SEMINIT(ap[0], ap[1], ap[2]);
        break;
    case OpType::SEMPOST:
        thread->SEMPOST(ap[0]);
        break;
    case OpType::SEMWAIT:
        thread->SEMWAIT(ap[0], ap[1], ap[2]);
        break;
    case OpType::SETADC:
        thread->SETADC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SETADCXX:
        thread->SETADCXX(ap[0], ap[1], ap[2]);
        break;
    case OpType::SETADCXY:
        thread->SETADCXY(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::SETADCZW:
        thread->SETADCZW(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::SETASHRMH:
        thread->SETASHRMH(ap[0], ap[1]);
        break;
    case OpType::SETASHRMH0:
        thread->SETASHRMH0(ap[0], ap[1]);
        break;
    case OpType::SETASHRMH1:
        thread->SETASHRMH1(ap[0], ap[1]);
        break;
    case OpType::SETASHRMV:
        thread->SETASHRMV(ap[0]);
        break;
    case OpType::SETC16:
        thread->SETC16(ap[0], ap[1]);
        break;
    case OpType::SETDMAREG:
        thread->SETDMAREG(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SETDVALID:
        thread->SETDVALID(ap[0]);
        break;
    case OpType::SETIBRWC:
        thread->SETIBRWC(ap[0], ap[1], ap[2]);
        break;
    case OpType::SETPKEDGOF:
        thread->SETPKEDGOF(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SETRWC:
        thread->SETRWC(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::SFPABS:
        thread->SFPABS(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPADD:
        thread->SFPADD(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::SFPADDI:
        thread->SFPADDI(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPAND:
        thread->SFPAND(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPCAST:
        thread->SFPCAST(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPCOMPC:
        thread->SFPCOMPC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPCONFIG:
        thread->SFPCONFIG(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPDIVP2:
        thread->SFPDIVP2(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPENCC:
        thread->SFPENCC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPEXEXP:
        thread->SFPEXEXP(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPEXMAN:
        thread->SFPEXMAN(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPIADD:
        thread->SFPIADD(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPLOAD:
        thread->SFPLOAD(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPLOADI:
        thread->SFPLOADI(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPLOADMACRO:
        thread->SFPLOADMACRO(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPLUT:
        thread->SFPLUT(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPLUTFP32:
        thread->SFPLUTFP32(ap[0], ap[1]);
        break;
    case OpType::SFPLZ:
        thread->SFPLZ(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPMAD:
        thread->SFPMAD(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::SFPMOV:
        thread->SFPMOV(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPMUL:
        thread->SFPMUL(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::SFPMULI:
        thread->SFPMULI(ap[0], ap[1], ap[2]);
        break;
    case OpType::SFPNOP:
        thread->SFPNOP();
        break;
    case OpType::SFPNOT:
        thread->SFPNOT(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPOR:
        thread->SFPOR(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPPOPC:
        thread->SFPPOPC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPPUSHC:
        thread->SFPPUSHC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSETCC:
        thread->SFPSETCC(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSETEXP:
        thread->SFPSETEXP(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSETMAN:
        thread->SFPSETMAN(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSETSGN:
        thread->SFPSETSGN(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSHFT:
        thread->SFPSHFT(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSHFT2:
        thread->SFPSHFT2(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSTORE:
        thread->SFPSTORE(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPSWAP:
        thread->SFPSWAP(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPTRANSP:
        thread->SFPTRANSP(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFPXOR:
        thread->SFPXOR(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::SFP_STOCH_RND:
        thread->SFP_STOCH_RND(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
        break;
    case OpType::SHIFTDMAREG:
        thread->SHIFTDMAREG(ap[0], ap[1], ap[2], ap[3], ap[4]);
        break;
    case OpType::SHIFTXA:
        thread->SHIFTXA(ap[0], ap[1]);
        break;
    case OpType::SHIFTXB:
        thread->SHIFTXB(ap[0], ap[1], ap[2]);
        break;
    case OpType::STALLWAIT:
        thread->STALLWAIT(ap[0], ap[1]);
        break;
    case OpType::STOREIND:
        thread->STOREIND(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5], ap[6]);
        break;
    case OpType::STOREREG:
        thread->STOREREG(ap[0], ap[1]);
        break;
    case OpType::SUBDMAREG:
        thread->SUBDMAREG(ap[0], ap[1], ap[2], ap[3]);
        break;
    case OpType::TBUFCMD:
        thread->TBUFCMD();
        break;
    case OpType::TRNSPSRCA:
        thread->TRNSPSRCA();
        break;
    case OpType::TRNSPSRCB:
        thread->TRNSPSRCB();
        break;
    case OpType::UNPACR:
        thread->UNPACR(ap[0], ap[1], ap[2], ap[3], ap[4], ap[5], ap[6], ap[7], ap[8], ap[9], ap[10], ap[11], ap[12]);
        break;
    case OpType::UNPACR_NOP:
        thread->UNPACR_NOP(ap[0], ap[1]);
        break;
    case OpType::WRCFG:
        thread->WRCFG(ap[0], ap[1], ap[2]);
        break;
    case OpType::XMOV:
        thread->XMOV(ap[0], ap[1]);
        break;
    case OpType::ZEROACC:
        thread->ZEROACC(ap[0], ap[1], ap[2]);
        break;
    case OpType::ZEROSRC:
        thread->ZEROSRC(ap[0], ap[1], ap[2], ap[3]);
        break;
    default:
        assert(false);
        break;
    }
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

