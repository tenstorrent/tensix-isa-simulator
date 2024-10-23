#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

//
//    Public constants
//

constexpr uint32_t 
    ADDR_MOD_0 = 0,
    ADDR_MOD_1 = 1,
    ADDR_MOD_2 = 2,
    ADDR_MOD_3 = 3,
    ADDR_MOD_4 = 4,
    ADDR_MOD_5 = 5,
    ADDR_MOD_6 = 6,
    ADDR_MOD_7 = 7;

//
//    addr_mod_t
//

struct addr_mod_t {
    // CLR, CR, INCR (4 bits)
    struct addr_mod_src_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
        uint32_t cr = 0;
    };
    // CLR, CR, INCR (8 bits)
    struct addr_mod_dest_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
        uint32_t cr = 0;
        uint32_t c_to_cr = 0;
    };
    // CLR, INCR (2 bits)
    struct addr_mod_fidelity_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
    };
    // CLR, INCR (4 bits)
    struct addr_mod_bias_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
    };

    // Set defaults so that we can skip unchanged in initialization list
    addr_mod_src_t srca = {};
    addr_mod_src_t srcb = {};
    addr_mod_dest_t dest = {};
    addr_mod_fidelity_t fidelity = {};
    addr_mod_bias_t bias = {};
#if 0 // TODO: Revise this (unused)
    addr_mod_src_t pack_ysrc = {};
    addr_mod_src_t pack_ydst = {};
#endif

    // Program source and dest registers
    void set(uint32_t mod_index) const;
};

//
//    addr_mod_pack_t
//

struct addr_mod_pack_t {
    // CLR, CR, INCR (4 bits)
    struct addr_mod_vals_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
        uint32_t cr = 0;
    };

    struct addr_mod_reduced_t {
        uint32_t incr = 0;
        uint32_t clr = 0;
    };

    // Set defaults so that we can skip unchanged in initialization list
    addr_mod_vals_t y_src = {};
    addr_mod_vals_t y_dst = {};
    addr_mod_reduced_t z_src = {};
    addr_mod_reduced_t z_dst = {};

    // Program source and dest registers
    void set(uint32_t mod_index) const;
};

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

