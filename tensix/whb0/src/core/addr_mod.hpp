// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

struct AddrMod {
    // CLR, CR, INCR (4 bits)
    struct Src {
        uint32_t incr;
        uint32_t clr;
        uint32_t cr;
    };
    // CLR, CR, INCR (8 bits)
    struct Dest {
        uint32_t incr;
        uint32_t clr;
        uint32_t cr;
        uint32_t c_to_cr;
    };
    // CLR, INCR (2 bits)
    struct Fidelity {
        uint32_t incr;
        uint32_t clr;
    };
    // CLR, INCR (4 bits)
    struct Bias {
        uint32_t incr;
        uint32_t clr;
    };

    Src srca;
    Src srcb;
    Dest dest;
    Fidelity fidelity;
    Bias bias;

    AddrMod() {
        srca.incr = 0;
        srca.clr = 0;
        srca.cr = 0;
        srcb.incr = 0;
        srcb.clr = 0;
        srcb.cr = 0;
        dest.incr = 0;
        dest.clr = 0;
        dest.cr = 0;
        dest.c_to_cr = 0;
        fidelity.incr = 0;
        fidelity.clr = 0;
        bias.incr = 0;
        bias.clr = 0;
    }
};

struct AddrModPack {
    // CLR, CR, INCR (4 bits)
    struct AddrModVals {
        uint32_t incr;
        uint32_t clr;
        uint32_t cr;
    };
    struct AddrModReduced {
        uint32_t incr = 0;
        uint32_t clr = 0;
    };

    AddrModVals y_src;
    AddrModVals y_dst;
    AddrModReduced z_src;
    AddrModReduced z_dst; 

    AddrModPack() {
        y_src.incr = 0;
        y_src.clr = 0;
        y_src.cr = 0;
        y_dst.incr = 0;
        y_dst.clr = 0;
        y_dst.cr = 0;
        z_src.incr = 0;
        z_src.clr = 0;
        z_dst.incr = 0;
        z_dst.clr = 0;
    }
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

