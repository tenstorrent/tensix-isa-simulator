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
//    [  0] 0.8373 0 -1 0.5 1.5 -1.5 1 0
//    [  8] 0 0 0 0.1627 240 33 280.5 35.5
//    [ 16] 319.5 36.5 355 375.5 394.5 414.5 445 461
//    [ 24] 21 495 515 567 532 9 9.5 11.5
//

void SfpuTest::test5() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 10);
    SFPLOADI(0, 0, 0x3f00);
    SFPCONFIG(0x0, 12, 0);
    SFPLOADI(0, 0, 0x3fc0);
    SFPCONFIG(0x0, 13, 0);
    SFPLOADI(0, 8, 0xbfc0);
    SFPLOADI(0, 10, 0x0);
    SFPCONFIG(0x0, 14, 0);
// dst_reg[0] == 0.0F
    SFPLOAD(7, 0, 3, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 8, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(7, 0, 3, 0);
    SFPMAD(10, 11, 7, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 9, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 11, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 12, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 13, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 14, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(9, 9, 9, 7, 0);
    SFPADD(7, 10, 10, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// dst_reg[0] == 7.0F
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPSTORE(9, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPSTORE(9, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPSTORE(9, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPSTORE(9, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPMUL(8, 11, 9, 7, 0);
    SFPADD(7, 10, 10, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a0);
// dst_reg[0] == 12.0F
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4140);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4150);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPADD(6, 10, 7, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4160);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 5, 0);
    SFPLOADI(4, 0, 0x3f00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4170);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPADD(6, 10, 7, 5, 0);
    SFPLOADI(4, 0, 0x3f00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4180);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 5, 0);
    SFPLOADI(4, 0, 0xbf00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4188);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPADD(6, 10, 7, 5, 0);
    SFPLOADI(4, 0, 0xbf00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4190);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPMUL(6, 7, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// dst_reg[0] == 19.0F
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4198);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPMUL(6, 7, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPLOADI(4, 0, 0x3f00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41a0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPMUL(6, 7, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPLOADI(4, 0, 0xbf00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41a8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPMUL(6, 7, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPLOADI(4, 0, 0xbf00);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41b0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(5, 0, 0xc0a0);
    SFPMUL(6, 7, 9, 4, 0);
    SFPMAD(11, 5, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41b8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 5, 0);
    SFPADD(5, 10, 10, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41c0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(10, 7, 9, 5, 0);
    SFPADD(5, 10, 10, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// dst_reg[0] == 25.0F
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41c8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPLOAD(5, 0, 3, 0);
    SFPMUL(5, 7, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41d0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLOADI(4, 0, 0xc0a0);
    SFPLOAD(5, 0, 3, 0);
    SFPMUL(7, 5, 9, 5, 0);
    SFPNOP();
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41d8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 5, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPADD(5, 10, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 7, 9, 4, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPMAD(11, 5, 4, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// dst_reg[0] == 29.0F
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41e8);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMAD(11, 7, 6, 5, 0);
    SFPNOP();
    SFPSTORE(5, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41f0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMAD(11, 7, 6, 6, 0);
    SFPLOADI(5, 0, 0xbf00);
    SFPADD(6, 10, 5, 6, 0);
    SFPNOP();
    SFPSTORE(6, 0, 3, 10);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41f8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOAD(6, 0, 3, 0);
    SFPMAD(11, 7, 6, 7, 0);
    SFPLOADI(6, 0, 0x3f00);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 10);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 10);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

