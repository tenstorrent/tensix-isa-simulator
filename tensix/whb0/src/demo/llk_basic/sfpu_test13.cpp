
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
//    [  0] 20 -30 40 50 60 70 8110 110
//    [  8] -120 130 280 150 128 2 160 136
//    [ 16] 42.5 180 245.006 200 -210 220 280 150
//    [ 24] -35 240 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test13() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 26);
    SFPLOADI(6, 0, 0xc1a0);
    SFPLOADI(7, 0, 0xc1f0);
// v_if (dst_reg[0] == 0.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPSETCC(0x0, 5, 0, 6);
    SFPABS(0x0, 6, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
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
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 2, 0xfaaa);
    SFPLOADI(7, 4, 0x7bb);
// v_if (dst_reg[0] == 2.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4000);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPNOT(0x0, 6, 6, 0);
    SFPMOV(0x0, 6, 7, 0);
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
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4000);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(6, 8, 0xffff);
    SFPLOADI(6, 10, 0x555);
    SFPIADD(0x0, 7, 6, 10);
    SFPLOADI(6, 8, 0xffff);
    SFPLOADI(6, 10, 0x556);
    SFPIADD(0x0, 7, 6, 2);
    SFPLOADI(6, 0, 0x4220);
    SFPSTORE(6, 0, 3, 26);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4040);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffff845, 7, 6, 9);
    SFPIADD(0xfffff844, 7, 6, 1);
    SFPLOADI(6, 0, 0x4248);
    SFPSTORE(6, 0, 3, 26);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 4, 0x80);
    SFPLOADI(7, 4, 0x7bb);
// v_if (dst_reg[0] == 4.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4080);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLZ(0x0, 6, 6, 0);
    SFPMOV(0x0, 6, 7, 0);
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
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4080);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe8, 7, 6, 9);
    SFPIADD(0xffffffe7, 7, 6, 1);
    SFPLOADI(6, 0, 0x4270);
    SFPSTORE(6, 0, 3, 26);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x40a0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffff845, 7, 6, 9);
    SFPIADD(0xfffff844, 7, 6, 1);
    SFPLOADI(6, 0, 0x428c);
    SFPSTORE(6, 0, 3, 26);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x42b4);
    SFPLOADI(7, 0, 0x42dc);
// v_if (dst_reg[0] == 6.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x40c0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMUL(6, 6, 9, 6, 0);
    SFPLOADI(5, 0, 0x4120);
    SFPADD(6, 10, 5, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
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
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x42f0);
    SFPLOADI(7, 0, 0x4302);
// v_if (dst_reg[0] == 8.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4100);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPMOV(0x0, 6, 6, 1);
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
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x430c);
    SFPLOADI(7, 0, 0x4316);
// v_if (dst_reg[0] == 10.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4120);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPDIVP2(0x1, 6, 6, 1);
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
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x4320);
    SFPLOADI(7, 4, 0x80);
// v_if (dst_reg[0] == 12.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4140);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPEXEXP(0x0, 6, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
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
    SFPLOADI(6, 0, 0x3f80);
    SFPSETEXP(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x4320);
    SFPLOADI(7, 8, 0x8);
    SFPLOADI(7, 10, 0x0);
// v_if (dst_reg[0] == 14.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4160);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPEXMAN(0x0, 6, 6, 0);
    SFPMOV(0x0, 6, 7, 0);
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
    SFPLOADI(6, 0, 0x4300);
    SFPSETMAN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x432a);
    SFPLOADI(7, 0, 0x4334);
// v_if (dst_reg[0] == 16.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4180);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSETEXP(0x84, 6, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 16.0F || dst_reg[0] == 17.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4180);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4188);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(5, 0, 0x433e);
    SFPLOADI(7, 0, 0x4348);
// v_if (dst_reg[0] == 18.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(6, 0, 0x4190);
    SFPMAD(6, 11, 4, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 8, 0x75);
    SFPLOADI(6, 10, 0x19a);
    SFPSETMAN(0x0, 5, 6, 0);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 18.0F || dst_reg[0] == 19.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4190);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x4198);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x4352);
    SFPLOADI(7, 0, 0x435c);
// v_if (dst_reg[0] == 20.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41a0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPSETSGN(0x1, 6, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 20.0F || dst_reg[0] == 21.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41a0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41a8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(6, 0, 0x430c);
    SFPLOADI(7, 0, 0x4316);
// v_if (dst_reg[0] == 22.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x41b0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPDIVP2(0x1, 6, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 22.0F || dst_reg[0] == 23.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41b0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41b8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x4370);
// v_if (dst_reg[0] == 24.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 6);
    SFPLOADI(6, 0, 0xc20c);
    SFPMOV(0x0, 6, 7, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 24.0F || dst_reg[0] == 25.0F)
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c0);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPLOAD(5, 0, 3, 0);
    SFPLOADI(6, 0, 0x41c8);
    SFPMAD(6, 11, 5, 6, 0);
    SFPNOP();
    SFPSETCC(0x0, 6, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 26);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 26);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

