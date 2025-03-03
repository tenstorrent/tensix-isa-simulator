// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include "core/thread.hpp"

#include "demo/llk_basic/sfpu_test.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using core::Thread;

//
//    SfpuTest
//

SfpuTest::SfpuTest(Thread *thread):
        SfpuWrap(thread) { }

SfpuTest::~SfpuTest() { }

void SfpuTest::test(int n) {
    configure_programmable_constants();
    switch (n) {
    case 1:
        test1();
        break;
    case 2:
        test2();
        break;
    case 3:
        test3();
        break;
    case 4:
        test4();
        break;
    case 5:
        test5();
        break;
    case 6:
        test6();
        break;
    case 7:
        test7();
        break;
    case 8:
        test8();
        break;
    case 9:
        test9();
        break;
    case 10:
        test10();
        break;
    case 11:
        test11();
        break;
    case 12:
        test12();
        break;
    case 13:
        test13();
        break;
    case 14:
        test14();
        break;
    case 15:
        test15();
        break;
    case 16:
        test16();
        break;
    case 17:
        test17();
        break;
    }
}

void SfpuTest::configure_programmable_constants() {
    // lreg[11] = -1
    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0xBF80);
    SFPCONFIG(0, 11, 0);

    // lreg[12] = ln(2)
    SFPLOADI(0, 0xA, 0x6000);
    SFPLOADI(0, 0x8, 0x3f31);
    SFPCONFIG(0, 12, 0);

    // lreg[13] = 1 / ln(2)
    SFPLOADI(0, 0xA, 0xAA3B);
    SFPLOADI(0, 0x8, 0x3FB8);
    SFPCONFIG(0, 13, 0);

    // lreg[14] = -0.5
    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0xBF00);
    SFPCONFIG(0, 14, 0);

    SFPLOADI(0, 0xA, 0x0000);
    SFPLOADI(0, 0x8, 0x0000);
}

//
//    [  0] 1.3 1.3 1.3 1.3 1.3 1.3 1.3 1.3
//    [  8] 1.3 1.3 1.3 1.3 1.3 1.3 1.3 1.3
//    [ 16] 1.3 1.3 1.3 1.3 1.3 1.3 1.3 1.3
//    [ 24] 1.3 1.3 1.3 1.3 1.3 1.3 1.3 1.3
//

void SfpuTest::test1() {
    SFPLOADI(7, 8, 0x3fa6);
    SFPLOADI(7, 10, 0x6666);
    SFPSTORE(7, 0, 3, 0);
}

//
//    [  0] -0 -1 -2 -3 -4 -5 -6 -7
//    [  8] -8 -9 -10 -11 -12 -13 -14 -15
//    [ 16] -16 -17 -18 -19 -20 -21 -22 -23
//    [ 24] -24 -25 -26 -27 -28 -29 -30 -31
//

void SfpuTest::test2() {
    SFPLOAD(7, 0, 3, 0);
    SFPMOV(0x0, 7, 7, 1);
    SFPSTORE(7, 0, 3, 4);
    SFPLOAD(7, 0, 3, 4);
    SFPSTORE(7, 0, 3, 0);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

