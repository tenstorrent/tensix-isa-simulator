// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>

#include "core/addr_mod.hpp"

#include "ckernel_thread.h"
#include "ckernel_addrmod.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

//
//    addr_mod_t
//

void addr_mod_t::set(uint32_t mod_index) const {
    core::AddrMod a;
    a.srca.incr = srca.incr;
    a.srca.clr = srca.clr;
    a.srca.cr = srca.cr;
    a.srcb.incr = srcb.incr;
    a.srcb.clr = srcb.clr;
    a.srcb.cr = srcb.cr;
    a.dest.incr = dest.incr;
    a.dest.clr = dest.clr;
    a.dest.cr = dest.cr;
    a.dest.c_to_cr = dest.c_to_cr;
    a.fidelity.incr = fidelity.incr;
    a.fidelity.clr = fidelity.clr;
    a.bias.incr = bias.incr;
    a.bias.clr = bias.clr;
    get_thread()->set_addr_mod(mod_index, a);
}

//
//    addr_mod_pack_t
//

void addr_mod_pack_t::set(uint32_t mod_index) const {
    core::AddrModPack a;
    a.y_src.incr = y_src.incr;
    a.y_src.clr = y_src.clr;
    a.y_src.cr = y_src.cr;
    a.y_dst.incr = y_dst.incr;
    a.y_dst.clr = y_dst.clr;
    a.y_dst.cr = y_dst.cr;
    a.z_src.incr = z_src.incr;
    a.z_src.clr = z_src.clr;
    a.z_dst.incr = z_dst.incr;
    a.z_dst.clr = z_dst.clr;
    get_thread()->set_addr_mod_pack(mod_index, a);
}

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

