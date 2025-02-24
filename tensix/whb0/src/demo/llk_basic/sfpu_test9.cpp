// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
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
//    [  0] -0 600 800 50 60 256 16 38
//    [  8] 55 30 60 40 60 -6 -6 -15
//    [ 16] 32 -17 32 -19 32 -21 32 -23
//    [ 24] 64 -25 64 -27 64 -29 -30 -31
//

void SfpuTest::test9() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 18);
// v_if(dst_reg[0] == 1.0F)
    SFPLOAD(7, 0, 3, 0);
    SFPMAD(10, 11, 7, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41a0);
    SFPLOADI(6, 0, 0x41f0);
    SFPMUL(7, 6, 9, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41a0);
    SFPLOADI(6, 0, 0x4220);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41a0);
    SFPLOADI(6, 0, 0x41f0);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41a0);
    SFPLOADI(6, 0, 0x4220);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4180);
    SFPDIVP2(0x4, 7, 7, 1);
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4380);
    SFPDIVP2(0xfffffffc, 7, 7, 1);
    SFPSTORE(7, 0, 3, 18);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 7.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x0);
    SFPLZ(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe0, 7, 6, 9);
    SFPIADD(0xffffffdf, 7, 6, 1);
    SFPLOADI(6, 0, 0x4218);
    SFPSTORE(6, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0xffff);
    SFPLOADI(7, 10, 0xffff);
    SFPLZ(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 4);
    SFPIADD(0xffffffff, 7, 6, 1);
    SFPLOADI(6, 0, 0x425c);
    SFPSTORE(6, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 2, 0xffff);
    SFPLZ(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff0, 7, 6, 9);
    SFPIADD(0xffffffef, 7, 6, 1);
    SFPLOADI(6, 0, 0x41f0);
    SFPSTORE(6, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(11, 7, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40e0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 4);
    SFPLZ(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe0, 7, 7, 5);
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x4270);
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4220);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 13.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3f80);
    SFPLOADI(6, 0, 0x4000);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0xc040);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0x4080);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0xc080);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 18);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3f80);
    SFPLOADI(6, 1, 0x4000);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 1, 0xffffc200);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 1, 0x4400);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 1, 0xffffc400);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 18);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] >= 15.0F && dst_reg[0] < 19.0)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4180);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4200);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] >= 19.0F && dst_reg[0] < 23.0)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41b0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4200);
    SFPSTORE(7, 0, 3, 18);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if ((dst_reg[0] == 23.0 || dst_reg[0] == 24.0 || ...)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x4280);
    SFPSTORE(7, 0, 3, 18);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 18);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

