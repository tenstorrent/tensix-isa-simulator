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
//    [480] 8 8 8 8 8 8 8 8
//    [488] 16 16 16 16 16 16 16 16
//    [496] 24 24 24 24 24 24 24 24
//    [504] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test15() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 30);
// block 1
    SFPIADD(0x100, 15, 0, 5);
    SFPIADD(0x200, 15, 2, 5);
    SFPIADD(0x300, 15, 3, 5);
    SFPIADD(0x400, 15, 6, 5);
    SFPMOV(0x0, 2, 1, 2);
    SFPMOV(0x0, 3, 2, 2);
    SFPMOV(0x0, 6, 3, 2);
    SFPTRANSP(0x0, 0, 0, 0);
    SFPMOV(0x0, 3, 6, 2);
    SFPMOV(0x0, 2, 3, 2);
    SFPMOV(0x0, 1, 2, 2);
    SFPMOV(0x0, 15, 5, 2);
    SFPSHFT(0xfffffffc, 0, 5, 1);
    SFPMOV(0x0, 5, 7, 2);
    SFPSHFT(0x8, 0, 7, 1);
    SFPMOV(0x0, 7, 5, 0);
    SFPIADD(0x100, 5, 7, 5);
    SFPMOV(0x0, 7, 5, 0);
    SFPLOADI(4, 4, 0xf);
    SFPMOV(0x0, 15, 7, 2);
    SFPAND(0x0, 4, 7, 0);
    SFPOR(0x0, 5, 7, 0);
    SFPMOV(0x0, 0, 1, 2);
    SFPIADD(0x0, 7, 1, 6);
    SFPMOV(0x0, 1, 7, 0);
    SFPMOV(0x0, 15, 1, 2);
    SFPAND(0x0, 4, 1, 0);
    SFPIADD(0x10, 1, 1, 5);
    SFPOR(0x0, 5, 1, 0);
    SFPIADD(0x0, 1, 2, 6);
    SFPMOV(0x0, 2, 1, 0);
    SFPMOV(0x0, 15, 2, 2);
    SFPAND(0x0, 4, 2, 0);
    SFPIADD(0x20, 2, 2, 5);
    SFPOR(0x0, 5, 2, 0);
    SFPIADD(0x0, 2, 3, 6);
    SFPMOV(0x0, 3, 2, 0);
    SFPAND(0x0, 15, 4, 0);
    SFPIADD(0x30, 4, 4, 5);
    SFPMOV(0x0, 5, 3, 2);
    SFPOR(0x0, 4, 3, 0);
    SFPIADD(0x0, 3, 6, 6);
    SFPMOV(0x0, 6, 3, 0);
    SFPLOADI(6, 4, 0x0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPSETCC(0x0, 1, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 6, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 4, 0x0);
    SFPSETCC(0x0, 2, 0, 6);
    SFPSETCC(0x0, 3, 0, 6);
    SFPLOADI(5, 4, 0x1);
    SFPMOV(0x0, 5, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(0, 4, 0x0);
    SFPIADD(0xffffffff, 6, 6, 5);
    SFPSETCC(0x0, 6, 0, 6);
    SFPIADD(0xffffffff, 7, 7, 5);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPTRANSP(0x0, 0, 0, 0);
    SFPLOADI(5, 4, 0x0);
    SFPIADD(0xffffffff, 0, 7, 5);
    SFPSETCC(0x0, 7, 0, 6);
    SFPIADD(0xffffffff, 1, 7, 5);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 5, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0x0);
    SFPIADD(0xffffffff, 2, 7, 5);
    SFPSETCC(0x0, 7, 0, 6);
    SFPIADD(0xffffffff, 3, 7, 5);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 6, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 4, 0x0);
    SFPIADD(0xffffffff, 5, 5, 5);
    SFPSETCC(0x0, 5, 0, 6);
    SFPIADD(0xffffffff, 6, 6, 5);
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 4, 0x1);
    SFPMOV(0x0, 6, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4100);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffff, 7, 6, 9);
    SFPIADD(0xfffffffe, 7, 6, 1);
    SFPLOADI(6, 0, 0x4100);
    SFPSTORE(6, 0, 3, 30);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 30);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// end block 1
// block 2
    SFPSHFT2(0x0, 15, 1, 3);
    SFPNOP();
    SFPIADD(0xfffffffe, 15, 7, 5);
    SFPLOADI(6, 4, 0xf);
    SFPAND(0x0, 15, 6, 0);
    SFPSETCC(0x0, 6, 0, 6);
    SFPIADD(0x10, 7, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPIADD(0x0, 1, 7, 6);
    SFPMOV(0x0, 7, 1, 0);
    SFPLOADI(0, 4, 0x1);
    SFPLOADI(2, 4, 0x1);
    SFPLOADI(3, 4, 0x1);
    SFPTRANSP(0x0, 0, 0, 0);
    SFPLOADI(5, 4, 0x0);
    SFPSETCC(0x0, 1, 0, 6);
    SFPSETCC(0x0, 0, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 5, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0x0);
    SFPSETCC(0x0, 2, 0, 6);
    SFPSETCC(0x0, 3, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 6, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 4, 0x0);
    SFPIADD(0xffffffff, 5, 5, 5);
    SFPSETCC(0x0, 5, 0, 6);
    SFPIADD(0xffffffff, 6, 6, 5);
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 4, 0x1);
    SFPMOV(0x0, 6, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4100);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 4);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4180);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffff, 7, 6, 9);
    SFPIADD(0xfffffffe, 7, 6, 1);
    SFPLOADI(6, 0, 0x4180);
    SFPSTORE(6, 0, 3, 30);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 30);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// end block 2
// block 3
    SFPSHFT2(0x0, 9, 9, 3);
    SFPNOP();
    SFPSHFT2(0x0, 15, 2, 4);
    SFPNOP();
    SFPIADD(0xfffffffe, 15, 7, 5);
    SFPLOADI(6, 4, 0xf);
    SFPAND(0x0, 15, 6, 0);
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 4, 0x0);
    SFPMOV(0x0, 6, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPIADD(0x0, 2, 7, 6);
    SFPMOV(0x0, 7, 2, 0);
    SFPLOADI(0, 4, 0x1);
    SFPLOADI(1, 4, 0x1);
    SFPLOADI(3, 4, 0x1);
    SFPTRANSP(0x0, 0, 0, 0);
    SFPLOADI(5, 4, 0x0);
    SFPSETCC(0x0, 0, 0, 6);
    SFPSETCC(0x0, 2, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 5, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0x0);
    SFPSETCC(0x0, 1, 0, 6);
    SFPSETCC(0x0, 3, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 6, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 4, 0x0);
    SFPIADD(0xffffffff, 5, 5, 5);
    SFPSETCC(0x0, 5, 0, 6);
    SFPIADD(0xffffffff, 6, 6, 5);
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 4, 0x1);
    SFPMOV(0x0, 6, 7, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4180);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 4);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffff, 7, 6, 9);
    SFPIADD(0xfffffffe, 7, 6, 1);
    SFPLOADI(6, 0, 0x41c0);
    SFPSTORE(6, 0, 3, 30);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 30);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// end block 3
    SFPLOAD(7, 0, 3, 30);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

