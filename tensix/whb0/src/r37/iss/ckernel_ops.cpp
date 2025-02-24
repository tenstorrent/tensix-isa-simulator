// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "core/thread.hpp"

#include "ckernel_ops.h"

namespace ckernel {

using ronin::iss::whb0::core::Thread;

//
//    Public functions
//

bool is_valid(uint32_t val, uint32_t wid) {
    uint32_t mask = (1 << wid) - 1;
    return ((val & mask) == val); 
}

void run_thread_op(Thread *thread, const Op &op) {
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

void decode_op(uint32_t code, Op &op) {
    uint32_t *ap = op.args;
    for (int i = 0; i < 16; i++) {
       ap[i] = 0;
    }
    if (code == 0) {
        op.type = OpType(0);
        return;
    }
    switch (code >> 24) {
    case 0x58:
        op.type = OpType::ADDDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 12) & 0x7ff;
        ap[2] = (code >> 6) & 0x3f;
        ap[3] = (code >> 0) & 0x3f;
        break;
    case 0x53:
        op.type = OpType::ADDRCRXY;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x56:
        op.type = OpType::ADDRCRZW;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x25:
        op.type = OpType::APOOL3S1;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 15) & 0x7f;
        ap[2] = (code >> 14) & 0x1;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x32:
        op.type = OpType::APOOL3S2;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 15) & 0x7f;
        ap[2] = (code >> 14) & 0x1;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x64:
        op.type = OpType::ATCAS;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 18) & 0x1f;
        ap[2] = (code >> 14) & 0xf;
        ap[3] = (code >> 12) & 0x3;
        ap[4] = (code >> 6) & 0x3f;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0xa0:
        op.type = OpType::ATGETM;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0x61:
        op.type = OpType::ATINCGET;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 14) & 0x1ff;
        ap[2] = (code >> 12) & 0x3;
        ap[3] = (code >> 6) & 0x3f;
        ap[4] = (code >> 0) & 0x3f;
        break;
    case 0x62:
        op.type = OpType::ATINCGETPTR;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 22) & 0x1;
        ap[2] = (code >> 18) & 0xf;
        ap[3] = (code >> 14) & 0xf;
        ap[4] = (code >> 12) & 0x3;
        ap[5] = (code >> 6) & 0x3f;
        ap[6] = (code >> 0) & 0x3f;
        break;
    case 0xa1:
        op.type = OpType::ATRELM;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0x63:
        op.type = OpType::ATSWAP;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 14) & 0x1ff;
        ap[2] = (code >> 6) & 0xff;
        ap[3] = (code >> 0) & 0x3f;
        break;
    case 0x5b:
        op.type = OpType::BITWOPDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 18) & 0x1f;
        ap[2] = (code >> 12) & 0x3f;
        ap[3] = (code >> 6) & 0x3f;
        ap[4] = (code >> 0) & 0x3f;
        break;
    case 0x36:
        op.type = OpType::CLEARDVALID;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 0) & 0x3fffff;
        break;
    case 0x21:
        op.type = OpType::CLREXPHIST;
        break;
    case 0x5d:
        op.type = OpType::CMPDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 18) & 0x1f;
        ap[2] = (code >> 12) & 0x3f;
        ap[3] = (code >> 6) & 0x3f;
        ap[4] = (code >> 0) & 0x3f;
        break;
    case 0x22:
        op.type = OpType::CONV3S1;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 17) & 0x1f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 0) & 0x7fff;
        break;
    case 0x23:
        op.type = OpType::CONV3S2;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 17) & 0x1f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 0) & 0x7fff;
        break;
    case 0x60:
        op.type = OpType::DMANOP;
        break;
    case 0x29:
        op.type = OpType::DOTPV;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 21) & 0x1;
        ap[2] = (code >> 19) & 0x3;
        ap[3] = (code >> 15) & 0xf;
        ap[4] = (code >> 0) & 0x7fff;
        break;
    case 0x28:
        op.type = OpType::ELWADD;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 21) & 0x1;
        ap[2] = (code >> 19) & 0x3;
        ap[3] = (code >> 15) & 0xf;
        ap[4] = (code >> 0) & 0x7fff;
        break;
    case 0x27:
        op.type = OpType::ELWMUL;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 21) & 0x1;
        ap[2] = (code >> 19) & 0x3;
        ap[3] = (code >> 15) & 0xf;
        ap[4] = (code >> 0) & 0x7fff;
        break;
    case 0x30:
        op.type = OpType::ELWSUB;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 21) & 0x1;
        ap[2] = (code >> 19) & 0x3;
        ap[3] = (code >> 15) & 0xf;
        ap[4] = (code >> 0) & 0x7fff;
        break;
    case 0x46:
        op.type = OpType::FLUSHDMA;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0x34:
        op.type = OpType::GAPOOL;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 19) & 0x7;
        ap[2] = (code >> 15) & 0xf;
        ap[3] = (code >> 14) & 0x1;
        ap[4] = (code >> 0) & 0x3fff;
        break;
    case 0x35:
        op.type = OpType::GATESRCRST;
        ap[0] = (code >> 1) & 0x7fffff;
        ap[1] = (code >> 0) & 0x1;
        break;
    case 0x33:
        op.type = OpType::GMPOOL;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 19) & 0x7;
        ap[2] = (code >> 15) & 0xf;
        ap[3] = (code >> 14) & 0x1;
        ap[4] = (code >> 0) & 0x3fff;
        break;
    case 0x52:
        op.type = OpType::INCADCXY;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        break;
    case 0x55:
        op.type = OpType::INCADCZW;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        break;
    case 0x38:
        op.type = OpType::INCRWC;
        ap[0] = (code >> 18) & 0x3f;
        ap[1] = (code >> 14) & 0xf;
        ap[2] = (code >> 10) & 0xf;
        ap[3] = (code >> 6) & 0xf;
        break;
    case 0x49:
        op.type = OpType::LOADIND;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 14) & 0xff;
        ap[2] = (code >> 12) & 0x3;
        ap[3] = (code >> 6) & 0x3f;
        ap[4] = (code >> 0) & 0x3f;
        break;
    case 0x68:
        op.type = OpType::LOADREG;
        ap[0] = (code >> 18) & 0x3f;
        ap[1] = (code >> 0) & 0x3ffff;
        break;
    case 0x3a:
        op.type = OpType::MFCONV3S1;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 17) & 0x1f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 0) & 0x7fff;
        break;
    case 0x1:
        op.type = OpType::MOP;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 16) & 0x7f;
        ap[2] = (code >> 0) & 0xffff;
        break;
    case 0x3:
        op.type = OpType::MOP_CFG;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0x12:
        op.type = OpType::MOVA2D;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 17) & 0x3f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 12) & 0x7;
        ap[4] = (code >> 0) & 0xfff;
        break;
    case 0xb:
        op.type = OpType::MOVB2A;
        ap[0] = (code >> 17) & 0x7f;
        ap[1] = (code >> 15) & 0x3;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 0) & 0xfff;
        break;
    case 0x13:
        op.type = OpType::MOVB2D;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 17) & 0x3f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 12) & 0x7;
        ap[4] = (code >> 0) & 0xfff;
        break;
    case 0x8:
        op.type = OpType::MOVD2A;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 17) & 0x3f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 12) & 0x7;
        ap[4] = (code >> 0) & 0xfff;
        break;
    case 0xa:
        op.type = OpType::MOVD2B;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 17) & 0x3f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 12) & 0x7;
        ap[4] = (code >> 0) & 0xfff;
        break;
    case 0x9:
        op.type = OpType::MOVDBGA2D;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 17) & 0x3f;
        ap[2] = (code >> 15) & 0x3;
        ap[3] = (code >> 12) & 0x7;
        ap[4] = (code >> 0) & 0xfff;
        break;
    case 0x24:
        op.type = OpType::MPOOL3S1;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 15) & 0x7f;
        ap[2] = (code >> 14) & 0x1;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x31:
        op.type = OpType::MPOOL3S2;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 15) & 0x7f;
        ap[2] = (code >> 14) & 0x1;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x5a:
        op.type = OpType::MULDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 12) & 0x7ff;
        ap[2] = (code >> 6) & 0x3f;
        ap[3] = (code >> 0) & 0x3f;
        break;
    case 0x26:
        op.type = OpType::MVMUL;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 19) & 0x7;
        ap[2] = (code >> 15) & 0xf;
        ap[3] = (code >> 0) & 0x7fff;
        break;
    case 0x2:
        op.type = OpType::NOP;
        break;
    case 0x41:
        op.type = OpType::PACR;
        ap[0] = (code >> 15) & 0x1ff;
        ap[1] = (code >> 12) & 0x7;
        ap[2] = (code >> 8) & 0xf;
        ap[3] = (code >> 7) & 0x1;
        ap[4] = (code >> 4) & 0x7;
        ap[5] = (code >> 1) & 0x7;
        ap[6] = (code >> 0) & 0x1;
        break;
    case 0x4a:
        op.type = OpType::PACR_SETREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 22) & 0x1;
        ap[2] = (code >> 12) & 0x3ff;
        ap[3] = (code >> 8) & 0xf;
        ap[4] = (code >> 2) & 0x3f;
        ap[5] = (code >> 1) & 0x1;
        ap[6] = (code >> 0) & 0x1;
        break;
    case 0x15:
        op.type = OpType::RAREB;
        break;
    case 0xb1:
        op.type = OpType::RDCFG;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 0) & 0xffff;
        break;
    case 0x48:
        op.type = OpType::REG2FLOP;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 20) & 0x3;
        ap[2] = (code >> 18) & 0x3;
        ap[3] = (code >> 16) & 0x3;
        ap[4] = (code >> 6) & 0x3ff;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x4:
        op.type = OpType::REPLAY;
        ap[0] = (code >> 14) & 0x3ff;
        ap[1] = (code >> 4) & 0x3ff;
        ap[2] = (code >> 1) & 0x7;
        ap[3] = (code >> 0) & 0x1;
        break;
    case 0xb3:
        op.type = OpType::RMWCIB0;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 8) & 0xff;
        ap[2] = (code >> 0) & 0xff;
        break;
    case 0xb4:
        op.type = OpType::RMWCIB1;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 8) & 0xff;
        ap[2] = (code >> 0) & 0xff;
        break;
    case 0xb5:
        op.type = OpType::RMWCIB2;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 8) & 0xff;
        ap[2] = (code >> 0) & 0xff;
        break;
    case 0xb6:
        op.type = OpType::RMWCIB3;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 8) & 0xff;
        ap[2] = (code >> 0) & 0xff;
        break;
    case 0x44:
        op.type = OpType::RSTDMA;
        break;
    case 0xa5:
        op.type = OpType::SEMGET;
        ap[0] = (code >> 2) & 0x3fffff;
        break;
    case 0xa3:
        op.type = OpType::SEMINIT;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 2) & 0x3fff;
        break;
    case 0xa4:
        op.type = OpType::SEMPOST;
        ap[0] = (code >> 2) & 0x3fffff;
        break;
    case 0xa6:
        op.type = OpType::SEMWAIT;
        ap[0] = (code >> 15) & 0x1ff;
        ap[1] = (code >> 2) & 0x1fff;
        ap[2] = (code >> 0) & 0x3;
        break;
    case 0x50:
        op.type = OpType::SETADC;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 20) & 0x1;
        ap[2] = (code >> 18) & 0x3;
        ap[3] = (code >> 0) & 0x3ffff;
        break;
    case 0x5e:
        op.type = OpType::SETADCXX;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 10) & 0x7ff;
        ap[2] = (code >> 0) & 0x3ff;
        break;
    case 0x51:
        op.type = OpType::SETADCXY;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x54:
        op.type = OpType::SETADCZW;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 15) & 0x3f;
        ap[2] = (code >> 12) & 0x7;
        ap[3] = (code >> 9) & 0x7;
        ap[4] = (code >> 6) & 0x7;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x1e:
        op.type = OpType::SETASHRMH;
        ap[0] = (code >> 1) & 0x7fffff;
        ap[1] = (code >> 0) & 0x1;
        break;
    case 0x1a:
        op.type = OpType::SETASHRMH0;
        ap[0] = (code >> 1) & 0x7fffff;
        ap[1] = (code >> 0) & 0x1;
        break;
    case 0x1b:
        op.type = OpType::SETASHRMH1;
        ap[0] = (code >> 1) & 0x7fffff;
        ap[1] = (code >> 0) & 0x1;
        break;
    case 0x1c:
        op.type = OpType::SETASHRMV;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0xb2:
        op.type = OpType::SETC16;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 0) & 0xffff;
        break;
    case 0x45:
        op.type = OpType::SETDMAREG;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 8) & 0x3fff;
        ap[2] = (code >> 7) & 0x1;
        ap[3] = (code >> 0) & 0x7f;
        break;
    case 0x57:
        op.type = OpType::SETDVALID;
        ap[0] = (code >> 0) & 0xffffff;
        break;
    case 0x39:
        op.type = OpType::SETIBRWC;
        ap[0] = (code >> 18) & 0x3f;
        ap[1] = (code >> 6) & 0xfff;
        ap[2] = (code >> 0) & 0x3f;
        break;
    case 0x1d:
        op.type = OpType::SETPKEDGOF;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x37:
        op.type = OpType::SETRWC;
        ap[0] = (code >> 22) & 0x3;
        ap[1] = (code >> 18) & 0xf;
        ap[2] = (code >> 14) & 0xf;
        ap[3] = (code >> 10) & 0xf;
        ap[4] = (code >> 6) & 0xf;
        ap[5] = (code >> 0) & 0x3f;
        break;
    case 0x7d:
        op.type = OpType::SFPABS;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x85:
        op.type = OpType::SFPADD;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 12) & 0xf;
        ap[2] = (code >> 8) & 0xf;
        ap[3] = (code >> 4) & 0xf;
        ap[4] = (code >> 0) & 0xf;
        break;
    case 0x75:
        op.type = OpType::SFPADDI;
        ap[0] = (code >> 8) & 0xffff;
        ap[1] = (code >> 4) & 0xf;
        ap[2] = (code >> 0) & 0xf;
        break;
    case 0x7e:
        op.type = OpType::SFPAND;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x90:
        op.type = OpType::SFPCAST;
        ap[0] = (code >> 8) & 0xffff;
        ap[1] = (code >> 4) & 0xf;
        ap[2] = (code >> 0) & 0xf;
        break;
    case 0x8b:
        op.type = OpType::SFPCOMPC;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x91:
        op.type = OpType::SFPCONFIG;
        ap[0] = (code >> 8) & 0xffff;
        ap[1] = (code >> 4) & 0xf;
        ap[2] = (code >> 0) & 0xf;
        break;
    case 0x76:
        op.type = OpType::SFPDIVP2;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x8a:
        op.type = OpType::SFPENCC;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x77:
        op.type = OpType::SFPEXEXP;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x78:
        op.type = OpType::SFPEXMAN;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x79:
        op.type = OpType::SFPIADD;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x70:
        op.type = OpType::SFPLOAD;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 14) & 0x3;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x71:
        op.type = OpType::SFPLOADI;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 0) & 0xffff;
        break;
    case 0x93:
        op.type = OpType::SFPLOADMACRO;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 14) & 0x3;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x73:
        op.type = OpType::SFPLUT;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 0) & 0xffff;
        break;
    case 0x95:
        op.type = OpType::SFPLUTFP32;
        ap[0] = (code >> 4) & 0xfffff;
        ap[1] = (code >> 0) & 0xf;
        break;
    case 0x81:
        op.type = OpType::SFPLZ;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x84:
        op.type = OpType::SFPMAD;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 12) & 0xf;
        ap[2] = (code >> 8) & 0xf;
        ap[3] = (code >> 4) & 0xf;
        ap[4] = (code >> 0) & 0xf;
        break;
    case 0x7c:
        op.type = OpType::SFPMOV;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x86:
        op.type = OpType::SFPMUL;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 12) & 0xf;
        ap[2] = (code >> 8) & 0xf;
        ap[3] = (code >> 4) & 0xf;
        ap[4] = (code >> 0) & 0xf;
        break;
    case 0x74:
        op.type = OpType::SFPMULI;
        ap[0] = (code >> 8) & 0xffff;
        ap[1] = (code >> 4) & 0xf;
        ap[2] = (code >> 0) & 0xf;
        break;
    case 0x8f:
        op.type = OpType::SFPNOP;
        break;
    case 0x80:
        op.type = OpType::SFPNOT;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x7f:
        op.type = OpType::SFPOR;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x88:
        op.type = OpType::SFPPOPC;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x87:
        op.type = OpType::SFPPUSHC;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x7b:
        op.type = OpType::SFPSETCC;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x82:
        op.type = OpType::SFPSETEXP;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x83:
        op.type = OpType::SFPSETMAN;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x89:
        op.type = OpType::SFPSETSGN;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x7a:
        op.type = OpType::SFPSHFT;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x94:
        op.type = OpType::SFPSHFT2;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x72:
        op.type = OpType::SFPSTORE;
        ap[0] = (code >> 20) & 0xf;
        ap[1] = (code >> 16) & 0xf;
        ap[2] = (code >> 14) & 0x3;
        ap[3] = (code >> 0) & 0x3fff;
        break;
    case 0x92:
        op.type = OpType::SFPSWAP;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x8c:
        op.type = OpType::SFPTRANSP;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x8d:
        op.type = OpType::SFPXOR;
        ap[0] = (code >> 12) & 0xfff;
        ap[1] = (code >> 8) & 0xf;
        ap[2] = (code >> 4) & 0xf;
        ap[3] = (code >> 0) & 0xf;
        break;
    case 0x8e:
        op.type = OpType::SFP_STOCH_RND;
        ap[0] = (code >> 21) & 0x7;
        ap[1] = (code >> 16) & 0x1f;
        ap[2] = (code >> 12) & 0xf;
        ap[3] = (code >> 8) & 0xf;
        ap[4] = (code >> 4) & 0xf;
        ap[5] = (code >> 0) & 0xf;
        break;
    case 0x5c:
        op.type = OpType::SHIFTDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 18) & 0x1f;
        ap[2] = (code >> 12) & 0x3f;
        ap[3] = (code >> 6) & 0x3f;
        ap[4] = (code >> 0) & 0x3f;
        break;
    case 0x17:
        op.type = OpType::SHIFTXA;
        ap[0] = (code >> 2) & 0x3fffff;
        ap[1] = (code >> 0) & 0x3;
        break;
    case 0x18:
        op.type = OpType::SHIFTXB;
        ap[0] = (code >> 15) & 0x1ff;
        ap[1] = (code >> 10) & 0x1f;
        ap[2] = (code >> 0) & 0x3ff;
        break;
    case 0xa2:
        op.type = OpType::STALLWAIT;
        ap[0] = (code >> 15) & 0x1ff;
        ap[1] = (code >> 0) & 0x7fff;
        break;
    case 0x66:
        op.type = OpType::STOREIND;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 22) & 0x1;
        ap[2] = (code >> 21) & 0x1;
        ap[3] = (code >> 14) & 0x7f;
        ap[4] = (code >> 12) & 0x3;
        ap[5] = (code >> 6) & 0x3f;
        ap[6] = (code >> 0) & 0x3f;
        break;
    case 0x67:
        op.type = OpType::STOREREG;
        ap[0] = (code >> 18) & 0x3f;
        ap[1] = (code >> 0) & 0x3ffff;
        break;
    case 0x59:
        op.type = OpType::SUBDMAREG;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 12) & 0x7ff;
        ap[2] = (code >> 6) & 0x3f;
        ap[3] = (code >> 0) & 0x3f;
        break;
    case 0x4b:
        op.type = OpType::TBUFCMD;
        break;
    case 0x14:
        op.type = OpType::TRNSPSRCA;
        break;
    case 0x16:
        op.type = OpType::TRNSPSRCB;
        break;
    case 0x42:
        op.type = OpType::UNPACR;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 15) & 0xff;
        ap[2] = (code >> 13) & 0x3;
        ap[3] = (code >> 10) & 0x7;
        ap[4] = (code >> 8) & 0x3;
        ap[5] = (code >> 7) & 0x1;
        ap[6] = (code >> 6) & 0x1;
        ap[7] = (code >> 5) & 0x1;
        ap[8] = (code >> 4) & 0x1;
        ap[9] = (code >> 3) & 0x1;
        ap[10] = (code >> 2) & 0x1;
        ap[11] = (code >> 1) & 0x1;
        ap[12] = (code >> 0) & 0x1;
        break;
    case 0x43:
        op.type = OpType::UNPACR_NOP;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 0) & 0x7fffff;
        break;
    case 0xb0:
        op.type = OpType::WRCFG;
        ap[0] = (code >> 16) & 0xff;
        ap[1] = (code >> 15) & 0x1;
        ap[2] = (code >> 0) & 0x7fff;
        break;
    case 0x40:
        op.type = OpType::XMOV;
        ap[0] = (code >> 23) & 0x1;
        ap[1] = (code >> 0) & 0x7fffff;
        break;
    case 0x10:
        op.type = OpType::ZEROACC;
        ap[0] = (code >> 19) & 0x1f;
        ap[1] = (code >> 15) & 0xf;
        ap[2] = (code >> 0) & 0x7fff;
        break;
    case 0x11:
        op.type = OpType::ZEROSRC;
        ap[0] = (code >> 4) & 0xfffff;
        ap[1] = (code >> 3) & 0x1;
        ap[2] = (code >> 2) & 0x1;
        ap[3] = (code >> 0) & 0x3;
        break;
    default:
        assert(false);
        break;
    }
}

} // namespace ckernel

