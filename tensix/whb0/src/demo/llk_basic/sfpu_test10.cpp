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
//    [  0] -0 20 22 24 26 28 30 -7
//    [  8] 128 256 -30 -11 -12 -13 -14 -15
//    [ 16] -16 -17 -18 -19 -20 -21 -22 -23
//    [ 24] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test10() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 20);
// v_if(dst_reg[0] == 1.0F)
    SFPLOAD(7, 0, 3, 0);
    SFPMAD(10, 11, 7, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x15);
    SFPLOADI(6, 4, 0x6);
    SFPSHFT(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffac0, 7, 6, 9);
    SFPIADD(0xfffffabf, 7, 6, 1);
    SFPLOADI(6, 0, 0x41a0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x2aaa);
    SFPLOADI(6, 4, 0xfffffffc);
    SFPSHFT(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffd56, 7, 6, 9);
    SFPIADD(0xfffffd55, 7, 6, 1);
    SFPLOADI(6, 0, 0x41b0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 2, 0xaaaa);
    SFPLOADI(6, 4, 0xfffffffa);
    SFPSHFT(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffd56, 7, 6, 9);
    SFPIADD(0xfffffd55, 7, 6, 1);
    SFPLOADI(6, 0, 0x41c0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x5a);
    SFPLOADI(6, 4, 0x4);
    SFPSHFT(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffa60, 7, 6, 9);
    SFPIADD(0xfffffa5f, 7, 6, 1);
    SFPLOADI(6, 0, 0x41d0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x19);
    SFPIADD(0x5, 7, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
    SFPIADD(0x7, 7, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffdb, 7, 6, 9);
    SFPIADD(0xffffffda, 7, 6, 1);
    SFPLOADI(6, 0, 0x41e0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x1c);
    SFPLOADI(6, 4, 0x64);
    SFPIADD(0x0, 7, 6, 4);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffff80, 7, 6, 9);
    SFPIADD(0xffffff7f, 7, 6, 1);
    SFPLOADI(6, 0, 0x41f0);
    SFPSTORE(6, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPPOPC(0x0, 0, 0, 0);
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
    SFPLOAD(7, 0, 3, 0);
    SFPSETSGN(0x1, 7, 7, 1);
    SFPSTORE(7, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOAD(7, 0, 3, 0);
    SFPLOADI(6, 0, 0xc300);
    SFPSETSGN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0xc380);
    SFPSETSGN(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 20);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPADD(6, 10, 7, 7, 0);
    SFPNOP();
    SFPMOV(0x0, 7, 6, 0);
    SFPLOADI(7, 0, 0xc400);
    SFPSETSGN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 20);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 20);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

