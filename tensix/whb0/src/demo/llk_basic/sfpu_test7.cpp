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
//    [  0] 64 30 32 33 34 32 64 35
//    [  8] 1024 -1.625 1960.05 1960.05 -128 -128 -14 -128
//    [ 16] -128 -256 -18 -256 -20 -256 -22 -256
//    [ 24] -24 -25 -1024 -27 -1024 1712 176 1872
//

void SfpuTest::test7() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 14);
// v_if(dst_reg[0] == 1.0F)
    SFPLOAD(7, 0, 3, 0);
    SFPMAD(10, 11, 7, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x42f8);
    SFPLOADI(7, 10, 0x199a);
    SFPEXMAN(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(6, 8, 0xf8);
    SFPLOADI(6, 10, 0x199a);
    SFPIADD(0x0, 7, 6, 10);
    SFPLOADI(6, 8, 0xf8);
    SFPLOADI(6, 10, 0x199b);
    SFPIADD(0x0, 7, 6, 2);
    SFPLOADI(6, 0, 0x41f0);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x42f8);
    SFPLOADI(7, 10, 0x199a);
    SFPEXMAN(0x0, 7, 7, 1);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(6, 8, 0x78);
    SFPLOADI(6, 10, 0x199a);
    SFPIADD(0x0, 7, 6, 10);
    SFPLOADI(6, 8, 0x78);
    SFPLOADI(6, 10, 0x199b);
    SFPIADD(0x0, 7, 6, 2);
    SFPLOADI(6, 0, 0x4200);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4b80);
    SFPEXEXP(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe8, 7, 6, 9);
    SFPIADD(0xffffffe7, 7, 6, 1);
    SFPLOADI(6, 0, 0x4204);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4b80);
    SFPEXEXP(0x0, 7, 7, 1);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffff69, 7, 6, 9);
    SFPIADD(0xffffff68, 7, 6, 1);
    SFPLOADI(6, 0, 0x4208);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3f00);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4100);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 0);
    SFPLOADI(6, 0, 0x4400);
    SFPMOV(0x0, 6, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPEXEXP(0x0, 7, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(6, 0, 0x4200);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(6, 0, 0x4280);
    SFPSTORE(6, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40e0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff7, 7, 6, 9);
    SFPIADD(0xfffffff6, 7, 6, 1);
    SFPLOADI(6, 0, 0x420c);
    SFPSTORE(6, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3f80);
    SFPSETEXP(0x89, 7, 7, 1);
    SFPSTORE(7, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(6, 4, 0x7f);
    SFPLOADI(7, 0, 0xc4d0);
    SFPSETEXP(0x0, 7, 6, 0);
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 14);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 10.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(6, 0, 0x4480);
    SFPLOADI(7, 8, 0x75);
    SFPLOADI(7, 10, 0x19a);
    SFPSETMAN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 14);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(6, 0, 0x4480);
    SFPLOADI(7, 8, 0x75);
    SFPLOADI(7, 10, 0x19a);
    SFPSETMAN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 14);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(6, 0, 3, 0);
// v_if ((v >= 12.0f && v < 14.0f) || (v >= 15.0f && v < 17.0f))
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0xc300);
    SFPSTORE(7, 0, 3, 14);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(((v >= 17.0f && v < 18.0f) || (v >= 19.0f && v < 20.0f)) || ...)
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41a8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41b0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0xc380);
    SFPSTORE(7, 0, 3, 14);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (v >= 25.0f && v < 29.0f)
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41e8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0xc480);
    SFPSTORE(7, 0, 3, 14);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (v >= 29.0f && v < 32.0f)
    SFPLOADI(7, 0, 0x41e8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4200);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPMOV(0x0, 15, 6, 2);
    SFPSHFT(0xffffffff, 0, 6, 1);
    SFPLOADI(7, 0, 0x4180);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(5, 0, 0x4200);
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPLOADI(5, 0, 0x4280);
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(5, 0, 0x4300);
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPLOADI(5, 0, 0x4380);
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe3, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(5, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 5, 0, 6);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(5, 0, 0x4400);
    SFPADD(7, 10, 5, 5, 0);
    SFPNOP();
    SFPMOV(0x0, 5, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe2, 6, 5, 9);
    SFPSETCC(0x0, 5, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe3, 6, 6, 9);
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(6, 0, 0x4480);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 14);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 14);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

