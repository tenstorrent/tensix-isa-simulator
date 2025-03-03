// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include "demo/llk_basic/sfpu_test.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

//
//    SfpuTest
//

//
//    [  0] -250 160 -250 260 -270 280 -290 300
//    [  8] -310 320 -20 30 -40 50 -59 71
//    [ 16] -80 90 -90 100 -100 110 -110 120
//    [ 24] -120 130 -90 110 1.14939e+08 2.13444e+07 -30 -31
//

void SfpuTest::test14() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 28);
    SFPLOADI(6, 0, 0x437a);
    SFPLOADI(7, 0, 0x4382);
// v_if (dst_reg[0] != 0.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPSETCC(0x0, 5, 0, 2);
    SFPLOADI(5, 0, 0x4320);
    SFPMOV(0x0, 5, 7, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPMUL(9, 9, 9, 5, 0);
    SFPADD(5, 10, 9, 5, 0);
    SFPMOV(0x0, 6, 4, 1);
    SFPMOV(0x0, 4, 7, 0);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 0.0F || dst_reg[0] == 1.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(10, 11, 6, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x437a);
    SFPLOADI(7, 0, 0x4382);
// v_if (dst_reg[0] == 2.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4000);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLZ(0x0, 6, 5, 0);
    SFPSETCC(0x0, 5, 0, 2);
    SFPMUL(9, 9, 9, 5, 0);
    SFPADD(5, 10, 9, 5, 0);
    SFPMOV(0x0, 6, 4, 1);
    SFPMOV(0x0, 4, 7, 0);
    SFPMOV(0x0, 5, 6, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 2.0F || dst_reg[0] == 3.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4000);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4040);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x4387);
    SFPLOADI(7, 0, 0x438c);
// v_if (dst_reg[0] == 4.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4080);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPEXEXP(0x0, 6, 5, 0);
    SFPSETCC(0x0, 5, 0, 4);
    SFPMUL(9, 9, 9, 5, 0);
    SFPADD(5, 10, 9, 5, 0);
    SFPMOV(0x0, 6, 4, 1);
    SFPMOV(0x0, 4, 7, 0);
    SFPMOV(0x0, 5, 6, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 4.0F || dst_reg[0] == 5.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4080);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40a0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x4396);
    SFPLOADI(5, 4, 0x5);
// v_if (dst_reg[0] == 6.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(6, 0, 0x40c0);
    SFPMAD(6, 11, 4, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 0, 0x4391);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffffe, 5, 5, 9);
    SFPMUL(9, 9, 9, 5, 0);
    SFPADD(5, 10, 9, 5, 0);
    SFPMOV(0x0, 6, 4, 1);
    SFPMOV(0x0, 4, 7, 0);
    SFPMOV(0x0, 5, 6, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 6.0F || dst_reg[0] == 7.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40c0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40e0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0xa);
    SFPLOADI(7, 4, 0x14);
// v_if (dst_reg[0] == 8.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4100);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPIADD(0x1e, 6, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 8.0F || dst_reg[0] == 9.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4100);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4110);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4100);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffd8, 7, 6, 9);
    SFPIADD(0xffffffd7, 7, 6, 1);
    SFPLOADI(6, 0, 0xc39b);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4110);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffec, 7, 6, 9);
    SFPIADD(0xffffffeb, 7, 6, 1);
    SFPLOADI(6, 0, 0x43a0);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0xc1a0);
    SFPLOADI(7, 0, 0x41f0);
// v_if (dst_reg[0] == 10.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4120);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 10.0F || dst_reg[0] == 11.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4120);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4130);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0xc220);
    SFPLOADI(7, 0, 0x4248);
// v_if (dst_reg[0] == 12.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4140);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 100.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x42c8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 12.0F || dst_reg[0] == 13.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4140);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4150);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0xc270);
    SFPLOADI(7, 0, 0x428c);
// v_if (dst_reg[0] == 14.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4160);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 100.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x42c8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x3f80);
    SFPADD(6, 10, 5, 6, 0);
    SFPNOP();
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 14.0F || dst_reg[0] == 15.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4160);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4170);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(6, 0, 0x3f80);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0xa);
    SFPLOADI(7, 4, 0x14);
// v_if (dst_reg[0] == 16.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4180);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 6, 5, 2);
    SFPIADD(0x0, 7, 5, 6);
    SFPMOV(0x0, 5, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 100.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x42c8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 16.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4180);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff6, 7, 6, 9);
    SFPIADD(0xfffffff5, 7, 6, 1);
    SFPLOADI(6, 0, 0xc2a0);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 17.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4188);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffec, 7, 6, 9);
    SFPIADD(0xffffffeb, 7, 6, 1);
    SFPLOADI(6, 0, 0x42b4);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 4, 0xa);
    SFPLOADI(6, 4, 0x14);
// v_if (dst_reg[0] == 16.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4180);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 7, 5, 2);
    SFPIADD(0x0, 6, 5, 6);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 100.0f)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x42c8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 18.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4190);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff6, 7, 6, 9);
    SFPIADD(0xfffffff5, 7, 6, 1);
    SFPLOADI(6, 0, 0xc2b4);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 19.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4198);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff6, 7, 6, 9);
    SFPIADD(0xfffffff5, 7, 6, 1);
    SFPLOADI(6, 0, 0x42c8);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x41a0);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(11, 7, 6, 4, 0);
    SFPLOADI(5, 4, 0xa);
    SFPLOADI(6, 4, 0x14);
    SFPLOADI(7, 4, 0x1e);
// v_if (condition == 0.0F)
    SFPSETCC(0x0, 4, 0, 6);
    SFPMOV(0x0, 6, 4, 2);
    SFPIADD(0x0, 5, 4, 6);
    SFPMOV(0x0, 4, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (vConst0p8373 == dst_reg[0])
    SFPLOAD(4, 0, 3, 0);
    SFPMAD(4, 11, 8, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 6);
    SFPSTORE(5, 0, 3, 28);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 20.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41a0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xa, 7, 6, 9);
    SFPIADD(0x9, 7, 6, 1);
    SFPLOADI(6, 0, 0xc2c8);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 21.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41a8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 7, 6, 9);
    SFPIADD(0xffffffe1, 7, 6, 1);
    SFPLOADI(6, 0, 0x42dc);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x41b0);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(11, 7, 6, 4, 0);
    SFPLOADI(7, 4, 0xa);
    SFPLOADI(5, 4, 0x14);
    SFPLOADI(6, 4, 0x1e);
// v_if (condition == 0.0F)
    SFPSETCC(0x0, 4, 0, 6);
    SFPIADD(0x0, 7, 5, 6);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (vConst0p8373 == dst_reg[0])
    SFPLOAD(5, 0, 3, 0);
    SFPMAD(5, 11, 8, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(7, 0, 3, 28);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 22.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41b0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff6, 7, 6, 9);
    SFPIADD(0xfffffff5, 7, 6, 1);
    SFPLOADI(6, 0, 0xc2dc);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 23.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41b8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff6, 7, 6, 9);
    SFPIADD(0xfffffff5, 7, 6, 1);
    SFPLOADI(6, 0, 0x42f0);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x41c0);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(11, 7, 6, 4, 0);
    SFPLOADI(5, 4, 0xa);
    SFPLOADI(7, 4, 0x14);
    SFPLOADI(6, 4, 0x1e);
// v_if (condition == 0.0F)
    SFPSETCC(0x0, 4, 0, 6);
    SFPMOV(0x0, 5, 4, 2);
    SFPMOV(0x0, 7, 5, 2);
    SFPIADD(0x0, 4, 5, 6);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (vConst0p8373 == dst_reg[0])
    SFPLOAD(5, 0, 3, 0);
    SFPMAD(5, 11, 8, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(6, 0, 3, 28);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 24.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffec, 7, 6, 9);
    SFPIADD(0xffffffeb, 7, 6, 1);
    SFPLOADI(6, 0, 0xc2f0);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 25.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffec, 7, 6, 9);
    SFPIADD(0xffffffeb, 7, 6, 1);
    SFPLOADI(6, 0, 0x4302);
    SFPSTORE(6, 0, 3, 28);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(5, 0, 3, 0);
// v_if (dr == 26.0F || dr == 27.0F)
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(6, 0, 0xc2b4);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dr == 26.0F)
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x42c8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dr == 27.0F)
    SFPLOADI(4, 0, 0x41d8);
    SFPMAD(4, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 6);
    SFPLOADI(4, 0, 0x42dc);
    SFPMOV(0x0, 4, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dr == 27.0F)
    SFPLOADI(4, 0, 0x41d8);
    SFPMAD(4, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 6);
    SFPMOV(0x0, 7, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dr == 26.0F || dr == 27.0F)
    SFPLOADI(4, 0, 0x41d0);
    SFPMAD(4, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 2);
    SFPLOADI(4, 0, 0x41d8);
    SFPMAD(4, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(6, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dr == 500.0F)
    SFPLOADI(6, 0, 0x43fa);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x4348);
    SFPLOADI(6, 0, 0x3f80);
// [0] v_if (dst_reg[0] == 28.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x4000);
    SFPMOV(0x0, 5, 6, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41f0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPLOADI(5, 0, 0x4120);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPMUL(7, 6, 9, 5, 0);
    SFPNOP();
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
// [1] v_if (dst_reg[0] == 28.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x4080);
    SFPMOV(0x0, 5, 6, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPLOADI(5, 0, 0x41a0);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPMUL(7, 6, 9, 5, 0);
    SFPNOP();
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
// [2] v_if (dst_reg[0] == 28.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x4100);
    SFPMOV(0x0, 5, 6, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPLOADI(5, 0, 0x4120);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPMUL(7, 6, 9, 5, 0);
    SFPNOP();
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
// [3] v_if (dst_reg[0] == 28.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x4080);
    SFPMUL(6, 5, 9, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 6, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41d8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPLOADI(5, 0, 0x41a0);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPMUL(7, 6, 9, 5, 0);
    SFPNOP();
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
// [4] v_if (dst_reg[0] == 28.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0x4080);
    SFPMUL(6, 5, 9, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 6, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41d0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPLOADI(5, 0, 0x4120);
    SFPMOV(0x0, 5, 6, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
// v_if (dst_reg[0] == 28.0F || dst_reg[0] == 29.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41e0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41e8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 28);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 28);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

