
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
//    [  0] -0 -1 23 -2 1 20 20 -7
//    [  8] -8 -9 -10 -11 -12 -13 -14 -15
//    [ 16] -16 -17 -18 -19 -20 -21 -22 -23
//    [ 24] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test17() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 34);
// v_if (dst_reg[0] == 2.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4000);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0xffffffff);
    SFPLOADI(6, 4, 0xfffffffe);
    SFPSWAP(0x0, 6, 7, 1);
    SFPNOP();
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0x1, 7, 6, 9);
    SFPSETCC(0x0, 7, 0, 0);
    SFPLOADI(6, 0, 0x41b8);
    SFPSTORE(6, 0, 3, 34);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 34);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 3.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4040);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0xbf80);
    SFPLOADI(6, 0, 0xc000);
    SFPSWAP(0x0, 6, 7, 1);
    SFPNOP();
    SFPSTORE(7, 0, 3, 34);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 4.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4080);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 0, 0x3f80);
    SFPLOADI(6, 0, 0x4000);
    SFPSWAP(0x0, 6, 7, 1);
    SFPNOP();
    SFPSTORE(7, 0, 3, 34);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 5.0F || dst_reg[0] == 6.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40a0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x40c0);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 2);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLOADI(6, 0, 0xbf80);
    SFPLOADI(7, 0, 0x3f80);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x40a0);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 6);
    SFPLZ(0x0, 6, 6, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffffe, 6, 5, 9);
    SFPIADD(0xfffffffd, 6, 5, 1);
    SFPLOADI(5, 0, 0x41a0);
    SFPSTORE(5, 0, 3, 34);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(6, 0, 3, 34);
    SFPPOPC(0x0, 0, 0, 0);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPLZ(0x0, 7, 7, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffffe, 7, 6, 9);
    SFPIADD(0xfffffffd, 7, 6, 1);
    SFPLOADI(6, 0, 0x41a0);
    SFPSTORE(6, 0, 3, 34);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 34);
    SFPPOPC(0x0, 0, 0, 0);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 34);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

