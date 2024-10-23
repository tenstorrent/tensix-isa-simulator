
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
//    [  0] -0 35 -35 25 -25 38 -32 64
//    [  8] 128 4096 16 -128 1960.05 -1.625 30 -1
//    [ 16] 121 131 141 105 -105 -21 -22 -23
//    [ 24] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test12() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 24);
// v_if(dst_reg[0] == 1.0F)
    SFPLOAD(7, 0, 3, 0);
    SFPMAD(10, 11, 7, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x420c);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0xc20c);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x5);
    SFPIADD(0x23, 7, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffd8, 7, 6, 9);
    SFPIADD(0xffffffd7, 7, 6, 1);
    SFPLOADI(6, 0, 0x41c8);
    SFPSTORE(6, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x5);
    SFPIADD(0xffffffdd, 7, 6, 5);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0x1e, 7, 6, 9);
    SFPIADD(0x1d, 7, 6, 1);
    SFPLOADI(6, 0, 0xc1c8);
    SFPSTORE(6, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4040);
    SFPLOADI(6, 0, 0x420c);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4040);
    SFPLOADI(6, 0, 0xc20c);
    SFPADD(7, 10, 6, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
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
    SFPLOADI(7, 4, 0x4000);
    SFPMOV(0x0, 7, 6, 2);
    SFPSHFT(0xfffffff6, 0, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff0, 7, 6, 9);
    SFPIADD(0xffffffef, 7, 6, 1);
    SFPLOADI(6, 0, 0x4280);
    SFPSTORE(6, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x1);
    SFPMOV(0x0, 7, 6, 2);
    SFPSHFT(0xa, 0, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffc00, 7, 6, 9);
    SFPIADD(0xfffffbff, 7, 6, 1);
    SFPLOADI(6, 0, 0x4300);
    SFPSTORE(6, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4380);
    SFPDIVP2(0x4, 7, 7, 1);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4380);
    SFPDIVP2(0xfffffffc, 7, 7, 1);
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 11.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4300);
    SFPSETSGN(0xfff, 7, 7, 1);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(6, 0, 0x4480);
    SFPLOADI(7, 8, 0x75);
    SFPLOADI(7, 10, 0x19a);
    SFPSETMAN(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0xc4d0);
    SFPSETEXP(0x7f, 7, 6, 1);
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPPOPC(0x0, 0, 0, 0);
    SFPENCC(0x3, 0, 0, 10);
    SFPLOADI(7, 0, 0x41f0);
    SFPSTORE(7, 0, 3, 60);
    SFPSTORE(11, 0, 3, 62);
//  v_if(dst_reg[0] == 14.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOAD(7, 0, 3, 60);
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 15.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOAD(7, 0, 3, 62);
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 16.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4180);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x42f0);
    SFPSTORE(7, 0, 3, 24);
    SFPNOP();
    SFPLOAD(6, 0, 3, 24);
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0x3f80);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 17.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4302);
    SFPSTORE(7, 0, 3, 24);
    SFPNOP();
    SFPLOAD(6, 0, 3, 24);
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0x3f80);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 18.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x430c);
    SFPSTORE(7, 0, 3, 24);
    SFPNOP();
    SFPLOAD(6, 0, 3, 24);
    SFPMOV(0x0, 6, 7, 0);
    SFPLOADI(6, 0, 0x3f80);
    SFPADD(7, 10, 6, 7, 0);
    SFPNOP();
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if(dst_reg[0] == 19.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4040);
    SFPLOADI(6, 0, 0x420c);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x41a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x4040);
    SFPLOADI(6, 0, 0xc20c);
    SFPMUL(7, 6, 9, 6, 0);
    SFPNOP();
    SFPMOV(0x0, 6, 7, 0);
    SFPSTORE(7, 0, 3, 24);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 24);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

