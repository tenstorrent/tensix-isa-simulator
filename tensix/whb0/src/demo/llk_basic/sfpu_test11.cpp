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
//    [  0] -0 0.125 -0.125 0.125 -0.125 0.375 -0.5 0.25
//    [  8] -0.25 0.25 -0.25 0.75 -1 -3.5 10 -20.5
//    [ 16] 3.5 -3.5 10 -25 3.5 -3.5 8 -14.5
//    [ 24] -23 36 55 3.5 -23 46 67 3.5
//

void SfpuTest::test11() {
// ---- test11
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 22);
    SFPLOADI(6, 2, 0xff30);
    SFPLOADI(7, 4, 0x3020);
// v_if(dst_reg[0] == 1.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPMAD(10, 11, 5, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 0, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4000);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 4, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4040);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 0, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4080);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 4, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x40a0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 0, 0x3f80);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 4, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x40c0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(3, 0, 0x4080);
    SFPLOADI(2, 2, 0xa010);
    SFPMOV(0x0, 6, 0, 2);
    SFPMOV(0x0, 7, 1, 2);
    SFPLUT(5, 0, 0);
    SFPMOV(0x0, 1, 7, 2);
    SFPMOV(0x0, 0, 6, 2);
    SFPMOV(0x0, 5, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(5, 4, 0x0);
    SFPMOV(0x0, 5, 6, 0);
    SFPLOADI(6, 4, 0x0);
    SFPMOV(0x0, 6, 7, 0);
// TTI_SFPLOADI(0, SFPLOADI_MOD0_USHORT, 0xFF20);
// TTI_SFPLOADI(1, SFPLOADI_MOD0_USHORT, 0x2010);
// Manually inserted:
    SFPLOADI(0, 2, 0xFF20);
    SFPLOADI(1, 2, 0x2010);
// v_if(dst_reg[0] == 7.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 0, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 4, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 0, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 8, 0xbe99);
    SFPLOADI(3, 10, 0x999a);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 4, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x3f80);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 4, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x4080);
    SFPLOADI(2, 2, 0x9000);
    SFPLUT(7, 0, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(0, 8, 0x4000);
    SFPLOADI(0, 10, 0x4200);
    SFPLOADI(1, 8, 0x4400);
    SFPLOADI(1, 10, 0x4500);
    SFPLOADI(2, 8, 0x4600);
    SFPLOADI(2, 10, 0x4700);
// v_if(dst_reg[0] == 13.0f)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLOADI(7, 2, 0x7);
    SFPLUTFP32(7, 14);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x3fa0);
    SFPLOADI(7, 2, 0x7);
    SFPLUTFP32(7, 14);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xc010);
    SFPLOADI(7, 2, 0x7);
    SFPLUTFP32(7, 14);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4180);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLOADI(7, 2, 0x7);
    SFPLUTFP32(7, 10);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(0, 0, 0x4000);
    SFPLOADI(1, 0, 0x4080);
    SFPLOADI(2, 0, 0x40c0);
    SFPLOADI(4, 0, 0x4040);
    SFPLOADI(5, 0, 0x40a0);
    SFPLOADI(6, 0, 0x40e0);
// v_if(dst_reg[0] == 17.0f)
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLUTFP32(7, 4);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x3fa0);
    SFPLUTFP32(7, 4);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xc040);
    SFPLUTFP32(7, 4);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLUTFP32(7, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(0, 8, 0x4400);
    SFPLOADI(0, 10, 0x4000);
    SFPLOADI(1, 8, 0x4800);
    SFPLOADI(1, 10, 0x4600);
    SFPLOADI(2, 8, 0x4a00);
    SFPLOADI(2, 10, 0x4900);
    SFPLOADI(4, 8, 0x4500);
    SFPLOADI(4, 10, 0x4200);
    SFPLOADI(5, 8, 0x4880);
    SFPLOADI(5, 10, 0x4700);
    SFPLOADI(6, 8, 0x4a80);
    SFPLOADI(6, 10, 0x4980);
// v_if(dst_reg[0] == 21.0f)
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x3f40);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbfa0);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbfe0);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x4020);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x4060);
    SFPLUTFP32(7, 6);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLUTFP32(7, 2);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(0, 8, 0x4400);
    SFPLOADI(0, 10, 0x4000);
    SFPLOADI(1, 8, 0x4800);
    SFPLOADI(1, 10, 0x4600);
    SFPLOADI(2, 8, 0x4a00);
    SFPLOADI(2, 10, 0x4900);
    SFPLOADI(4, 8, 0x4500);
    SFPLOADI(4, 10, 0x4200);
    SFPLOADI(5, 8, 0x4880);
    SFPLOADI(5, 10, 0x4700);
    SFPLOADI(6, 8, 0x4a80);
    SFPLOADI(6, 10, 0x4980);
// v_if(dst_reg[0] == 28.0f)
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41e0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbfe0);
    SFPLUTFP32(7, 7);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41e8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x4060);
    SFPLUTFP32(7, 7);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41f0);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0x4090);
    SFPLUTFP32(7, 7);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(3, 0, 3, 0);
    SFPLOADI(7, 0, 0x41f8);
    SFPMAD(7, 11, 3, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(3, 0, 0xbe80);
    SFPLUTFP32(7, 3);
    SFPNOP();
    SFPMOV(0x0, 7, 3, 0);
    SFPSTORE(3, 0, 3, 22);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 22);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

