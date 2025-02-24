// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef TRISC_MATH
#include "llk_math_common.h"
#define MATH(x) x
#define MAIN math_main()
#define TRISC_NS math
#else
#define MATH(x)
#endif

#ifdef TRISC_PACK
#include "llk_pack_common.h"
#include "llk_pack.h"
#define PACK(x) x
#define MAIN pack_main()
#define TRISC_NS pack
#else
#define PACK(x)
#endif

#ifdef TRISC_UNPACK
#include "llk_unpack_common.h"
#define UNPACK(x) x
#define MAIN unpack_main()
#define TRISC_NS unpack
#else
#define UNPACK(x)
#endif

#define BEGIN_NS_COMPUTE \
    namespace ronin { \
    namespace iss { \
    namespace whb0 { \
    namespace llk { \
    namespace compute { \
    namespace TRISC_NS { \
    using namespace llk::ref;

#define END_NS_COMPUTE } } } } } }

BEGIN_NS_COMPUTE

// moved from "hostdevcommon/kernel_structs.h"
enum DstMode {
    Full          = 0,
    Half          = 1,
    Tile          = 2,
    NUM_DST_MODES = 3,
};

END_NS_COMPUTE

