
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
//    [  0] 3 2 3 2 2 3 2 3
//    [  8] 1.12544e+07 1.12544e+07 1.32324 1.32031 48 64 3.27904e-43 96
//    [ 16] 112 128 4.54469e-41 4.54469e-41 -20 -21 -22 -23
//    [ 24] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test16() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 32);
    SFPLOADI(6, 0, 0x4000);
    SFPLOADI(7, 0, 0x4040);
// v_if (dst_reg[0] < 8.0F)
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4100);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 0);
    SFPSWAP(0x0, 7, 6, 0);
    SFPNOP();
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOAD(4, 0, 3, 0);
    SFPLOADI(5, 0, 0x4080);
    SFPMAD(5, 11, 4, 5, 0);
    SFPNOP();
    SFPSETCC(0x0, 5, 0, 4);
    SFPSWAP(0x0, 7, 6, 1);
    SFPNOP();
    SFPPOPC(0x0, 0, 0, 0);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPMOV(0x0, 15, 5, 2);
    SFPSHFT(0xffffffff, 0, 5, 1);
    SFPLOADI(4, 2, 0x1);
    SFPAND(0x0, 4, 5, 0);
    SFPSETCC(0x0, 5, 0, 6);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 8.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4100);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0xab);
    SFPLOADI(7, 10, 0xbaab);
    SFPCAST(7, 7, 1);
    SFPSTORE(7, 0, 3, 32);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 9.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4110);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0xab);
    SFPLOADI(7, 10, 0xbaab);
    SFPCAST(7, 7, 0);
    SFPSTORE(7, 0, 3, 32);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 10.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4120);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3fa9);
    SFPLOADI(7, 10, 0x628d);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 0);
    SFPSTORE(7, 0, 3, 32);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 11.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4130);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x3fa9);
    SFPLOADI(7, 10, 0x628d);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 32);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 12.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4140);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x41ba);
    SFPLOADI(7, 10, 0x6666);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 2);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe8, 7, 6, 9);
    SFPIADD(0xffffffe7, 7, 6, 1);
    SFPLOADI(6, 0, 0x4240);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 13.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4150);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 8, 0x41ba);
    SFPLOADI(7, 10, 0x6666);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 3);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffffe8, 7, 6, 9);
    SFPIADD(0xffffffe7, 7, 6, 1);
    SFPLOADI(6, 0, 0x4280);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 14.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4160);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x8);
    SFPLOADI(6, 2, 0xea00);
    SFP_STOCH_RND(1, 0x0, 7, 6, 7, 4);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xffffff15, 7, 6, 9);
    SFPIADD(0xffffff14, 7, 6, 1);
    SFPLOADI(6, 0, 0x42a0);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 15.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4170);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0xea0);
    SFP_STOCH_RND(1, 0x8, 0, 7, 7, 12);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff1, 7, 6, 9);
    SFPIADD(0xfffffff0, 7, 6, 1);
    SFPLOADI(6, 0, 0x42c0);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 16.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4180);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0x8);
    SFPLOADI(6, 4, 0xea0);
    SFP_STOCH_RND(1, 0x0, 7, 6, 7, 5);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff1, 7, 6, 9);
    SFPIADD(0xfffffff0, 7, 6, 1);
    SFPLOADI(6, 0, 0x42e0);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 17.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4188);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 4, 0xea0);
    SFP_STOCH_RND(1, 0x8, 0, 7, 7, 13);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPIADD(0xfffffff1, 7, 6, 9);
    SFPIADD(0xfffffff0, 7, 6, 1);
    SFPLOADI(6, 0, 0x4300);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 18.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4190);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x77eb);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 7);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(6, 4, 0x7eb1);
    SFPIADD(0x0, 7, 6, 10);
    SFPLOADI(6, 4, 0x7eb2);
    SFPIADD(0x0, 7, 6, 2);
    SFPLOADI(6, 0, 0x4302);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
// v_if (dst_reg[0] == 19.0F)
    SFPLOAD(6, 0, 3, 0);
    SFPLOADI(7, 0, 0x4198);
    SFPMAD(7, 11, 6, 7, 0);
    SFPNOP();
    SFPSETCC(0x0, 7, 0, 6);
    SFPLOADI(7, 1, 0x77eb);
    SFP_STOCH_RND(1, 0x0, 0, 7, 7, 6);
    SFPPUSHC(0x0, 0, 0, 0);
    SFPLOADI(6, 4, 0x7eb1);
    SFPIADD(0x0, 7, 6, 10);
    SFPLOADI(6, 4, 0x7eb2);
    SFPIADD(0x0, 7, 6, 2);
    SFPLOADI(6, 0, 0x4304);
    SFPSTORE(6, 0, 3, 32);
    SFPCOMPC(0x0, 0, 0, 0);
    SFPSTORE(7, 0, 3, 32);
    SFPPOPC(0x0, 0, 0, 0);
// v_endif
    SFPENCC(0x3, 0, 0, 10);
    SFPLOAD(7, 0, 3, 32);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

