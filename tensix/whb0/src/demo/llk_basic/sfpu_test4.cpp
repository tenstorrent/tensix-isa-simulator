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
//    [  0] 64 64 66 65 65 67 68 69
//    [  8] 70 71 72 -11 120 120 120 -15
//    [ 16] 120 120 180 160 180 160 180 200
//    [ 24] 220 240 270 260 270 290 340 320
//

void SfpuTest::test4() {
    SFPLOAD(5, 0, 3, 0);
    SFPSTORE(5, 0, 3, 8);
// v_if(v < 2.0F)
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x4280);
    SFPSTORE(7, 0, 3, 8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(v < 6.0F)
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4282);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4284);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4286);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(v >= 6.0F)
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4288);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x428a);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x428c);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x428e);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4290);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(v >= 11.0F)
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x42f0);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(v >= 18.0F && v < 23.0F)
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x41a8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4320);
    SFPSTORE(7, 0, 3, 8);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4334);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(!(v != 23.0F))
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4348);
    SFPSTORE(7, 0, 3, 8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(!(v >= 25.0F) && !(v < 24.0F))
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x41c0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x435c);
    SFPSTORE(7, 0, 3, 8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(!((v < 25.0F) || (v >= 26.0F)))
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x4370);
    SFPSTORE(7, 0, 3, 8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if((v >= 26.0F) && (v < 29.0F))
    SFPLOADI(7, 0, 0x41d0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41e8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(7, 0, 0x4382);
    SFPSTORE(7, 0, 3, 8);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41d8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPLOADI(7, 0, 0x41e0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x4387);
    SFPSTORE(7, 0, 3, 8);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if ((v >= 28.0F) && (v == 29.0F || v == 30.0F || v == 31.0F))
    SFPLOADI(7, 0, 0x41e0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41e8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x41f0);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOADI(7, 0, 0x41f8);
    SFPMAD(7, 11, 5, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 4, 0x0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(6, 0, 0x41f0);
    SFPLOADI(7, 0, 0x438c);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPMAD(6, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 0);
    SFPLOADI(4, 0, 0x4120);
    SFPADD(7, 10, 4, 4, 0);
    SFPNOP();
    SFPMOV(0x0, 4, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPMAD(6, 11, 5, 4, 0);
    SFPNOP();
    SFPSETCC(0x0, 4, 0, 6);
    SFPLOADI(4, 0, 0x41a0);
    SFPADD(7, 10, 4, 4, 0);
    SFPNOP();
    SFPMOV(0x0, 4, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 4);
    SFPLOADI(6, 0, 0x4220);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 8);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 8);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

