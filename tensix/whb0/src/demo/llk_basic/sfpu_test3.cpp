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
//    [  0] 10 20 30 1.005 1.005 1.00194 1.005 1.00438
//    [  8] 1.00438 1.005 1.00194 -nan 1.005 0.00497437 1.00488 25
//    [ 16] 28 0.8373 1.99219 1.99609 130560 130592 65408 130816
//    [ 24] 0.000121832 6.09159e-05 20 20 20 20 20 20
//

void SfpuTest::test3() {
    SFPLOAD(7, 0, 3, 0);
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4120);
    SFPSTORE(7, 0, 3, 6);
// v_else
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 2.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41f0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 3.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3f80);
    SFPLOADI(7, 10, 0xa3d7);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 4.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3f80);
    SFPLOADI(7, 10, 0xa3d7);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 5.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x3f80);
    SFPLOADI(6, 8, 0x3f80);
    SFPLOADI(6, 10, 0x0);
    SFPOR(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 6.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3f80);
    SFPLOADI(7, 10, 0xa3d7);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 7.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40e0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 2, 0x8f80);
    SFPLOADI(6, 8, 0x3f80);
    SFPLOADI(6, 10, 0x0);
    SFPOR(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 8.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 2, 0x8f80);
    SFPLOADI(6, 8, 0x3f80);
    SFPLOADI(6, 10, 0x0);
    SFPOR(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 9.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3f80);
    SFPLOADI(7, 10, 0xa3d7);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 10.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 2, 0x3f80);
    SFPLOADI(6, 8, 0x3f80);
    SFPLOADI(6, 10, 0x0);
    SFPOR(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 11.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0xffff8f80);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 12.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3f80);
    SFPLOADI(7, 10, 0xa3d7);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 13.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3ba3);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 14.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x3c05);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 15.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41c8);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 16.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4180);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x41e0);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 17.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPSTORE(8, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 18.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3fff);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 19.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x3ffc);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 20.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x47ff);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 21.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x47ff);
    SFPLOADI(7, 10, 0x1000);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 22.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x7bfc);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 23.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41b8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x47ff);
    SFPLOADI(7, 10, 0x8000);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 24.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x7fc);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 25.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41c8);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x387f);
    SFPLOADI(7, 10, 0x8000);
    SFPSTORE(7, 0, 3, 6);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 6);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

