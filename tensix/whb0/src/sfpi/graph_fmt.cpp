// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <string>

#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Node formatting
//

std::string fmt_print(const char *fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 256, fmt, args);
    va_end(args);
    return std::string(buf);
}

namespace {

std::string fmt_sfpincrwc(Node *node) {
    uint32_t cr = node->arg(0);
    uint32_t d = node->arg(1);
    uint32_t b = node->arg(2);
    uint32_t a = node->arg(3);
    return fmt_print("spfincrwc(%d, %d, %d, %d)", cr, d, b, a);
}

std::string fmt_sfpassign_lv(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v = node->arg(0);
    uint32_t in = node->arg(1);
    return fmt_print("$%d = sfpassign_lv($%d, $%d)", ret, v, in);
}

std::string fmt_sfpload(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mod0 = node->arg(0);
    uint32_t mode = node->arg(1);
    uint32_t addr = node->arg(2);
    return fmt_print("$%d = sfpload(%d, %d, %d)", ret, mod0, mode, addr);
}

std::string fmt_sfpassignlreg(Node *node) {
    uint32_t ret = node->ret();
    uint32_t lr = node->arg(0);
    return fmt_print("$%d = sfpassignlreg(%d)", ret, lr);
}

std::string fmt_sfppreservelreg(Node *node) {
    uint32_t x = node->arg(0);
    uint32_t n = node->arg(1);
    return fmt_print("sfppreservelreg($%d, %d)", x, n);
}

std::string fmt_sfpxloadi(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mod0 = node->arg(0);
    uint32_t imm = node->arg(1);
    return fmt_print("$%d = sfpxloadi(%d, 0x%x)", ret, mod0, imm); 
}

std::string fmt_sfpstore(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t mod0 = node->arg(1);
    uint32_t mode = node->arg(2);
    uint32_t addr = node->arg(3);
    return fmt_print("spfstore($%d, %d, %d, %d)", src, mod0, mode, addr);
}

std::string fmt_sfpmov(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpmov($%d, %d);", ret, src, mod1);
}

std::string fmt_sfpnop(Node *node) {
    return fmt_print("sfpnop()");
}

std::string fmt_sfpillegal(Node *node) {
    return fmt_print("sfpillegal()");
}

std::string fmt_sfpencc(Node *node) {
    uint32_t imm = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("spfencc(0x%x, %d)", imm, mod1);
}

std::string fmt_sfppushc(Node *node) {
    return fmt_print("sfppushc()");
}

std::string fmt_sfppopc(Node *node) {
    return fmt_print("sfppopc()");
}

std::string fmt_sfpsetcc_v(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("sfpsetcc_v($%d, %d)", src, mod1);
}

std::string fmt_sfpsetcc_i(Node *node) {
    uint32_t imm = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("sfpsetcc_i(0x%x, %d)", imm, mod1);
}

std::string fmt_sfpxfcmps(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v = node->arg(0);
    uint32_t f = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("&%d = sfpxfcmps($%d, 0x%x, %d)", ret, v, f, mod1);
}

std::string fmt_sfpxfcmpv(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v1 = node->arg(0);
    uint32_t v2 = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("&%d = sfpxfcmpv($%d, $%d, %d)", ret, v1, v2, mod1);
}

std::string fmt_sfpxicmps(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v = node->arg(0);
    uint32_t i = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("&%d = sfpxicmps($%d, 0x%x, %d)", ret, v, i, mod1);
}

std::string fmt_sfpxicmpv(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v1 = node->arg(0);
    uint32_t v2 = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("&%d = sfpxicmpv($%d, $%d, %d)", ret, v1, v2, mod1);
}

std::string fmt_sfpxvif(Node *node) {
    uint32_t ret = node->ret();
    return fmt_print("@%d = sfpxvif()", ret);
}

std::string fmt_sfpxbool(Node *node) {
    uint32_t ret = node->ret();
    uint32_t t = node->arg(0);
    uint32_t a = node->arg(1);
    uint32_t b = node->arg(2);
    return fmt_print("&%d = sfpxbool(%d, &%d, &%d)", ret, t, a, b);
}

std::string fmt_sfpxcondb(Node *node) {
    uint32_t s = node->arg(0);
    uint32_t i = node->arg(1);
    return fmt_print("sfpxcondb(&%d, @%d)", s, i); 
}

std::string fmt_sfpxcondi(Node *node) {
    uint32_t ret = node->ret();
    uint32_t i = node->arg(0);
    return fmt_print("$%d = sfpxcondi(&%d)", ret, i);
}

std::string fmt_sfpcompc(Node *node) {
    return fmt_print("sfpcompc()");
}

std::string fmt_sfpadd(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpadd($%d, $%d, %d)", ret, va, vb, mod1);
}

std::string fmt_sfpmul(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpfmul($%d, $%d, %d)", ret, va, vb, mod1);
}

std::string fmt_sfpmad(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t vc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    return fmt_print("$%d = sfpmad($%d, $%d, $%d, %d)", ret, va, vb, vc, mod1);
}

std::string fmt_sfpexexp(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpexexp($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpexman(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpexman($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpsetexp_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpsetexp_i(0x%x, $%d)", ret, imm, src);
}

std::string fmt_sfpsetexp_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpsetexp_i(0x%x, $%d)", ret, dst, src);
}

std::string fmt_sfpsetman_i(Node *node) {
    uint32_t ret = node->ret(); 
    uint32_t imm = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpsetman_i(0x%x, $%d, %d)", ret, imm, src, mod1);
}

std::string fmt_sfpsetman_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpsetman_v($%d, $%d)", ret, dst, src);
}

std::string fmt_sfpabs(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpabs($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpand(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = spfand($%d, $%d)", ret, dst, src);
}

std::string fmt_sfpor(Node *node) {
    uint32_t ret = node->ret(); 
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = spfor($%d, $%d)", ret, dst, src);
}

std::string fmt_sfpxor(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = spfxor($%d, $%d)", ret, dst, src);
}

std::string fmt_sfpnot(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    return fmt_print("$%d = sfpnot($%d)", ret, src);
}

std::string fmt_sfpdivp2(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpdivp2(0x%x, $%d, %d)", ret, imm, src, mod1);
}

std::string fmt_sfplz(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfplz($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpshft_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t imm = node->arg(1);
    return fmt_print("$%d = sfpshft_i($%d, 0x%x)", ret, dst, imm);
}

std::string fmt_sfpshft_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpshft_v($%d, $%d)", ret, dst, src);
}

std::string fmt_sfpxiadd_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t imm = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpxiadd_i($%d, 0x%x, %d)", ret, src, imm, mod1);
}

std::string fmt_sfpxiadd_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpxiadd_v($%d, $%d, %d)", ret, dst, src, mod1);
}

std::string fmt_sfpsetsgn_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpsetsgn_i(0x%x, $%d)", ret, imm, src);
}

std::string fmt_sfpsetsgn_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpsetsgn_v($%d, $%d)", ret, dst, src);
}

std::string fmt_sfplut(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t dst = node->arg(3);
    uint32_t mod0 = node->arg(4);
    return fmt_print("$%d = sfplut($%d, $%d, $%d, $%d, %d)", ret, l0, l1, l2, dst, mod0);
}

std::string fmt_sfplutfp32_3r(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    uint32_t mod0 = node->arg(4);
    return fmt_print(
        "$%d = sfplutfp32_3r($%d, $%d, $%d, $%d, %d)", 
            ret, l0,  l1,  l2,  l3,  mod0);
}

std::string fmt_sfplutfp32_6r(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l4 = node->arg(3);
    uint32_t l5 = node->arg(4);
    uint32_t l6 = node->arg(5);
    uint32_t l3 = node->arg(6);
    uint32_t mod0 = node->arg(7);
    return fmt_print(
        "$%d = sfplutfp32_6r($%d, $%d, $%d, $%d, $%d, $%d, $%d, %d)",
            ret, l0, l1, l2, l4, l5, l6, l3, mod0);
}

std::string fmt_sfpcast(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpcast($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpstochrnd_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mode = node->arg(0);
    uint32_t imm = node->arg(1);
    uint32_t srcc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    return fmt_print("$%d = sfpstochrnd_i(%d, 0x%x, $%d, %d)", ret, mode, imm, srcc, mod1);
}

std::string fmt_sfpstochrnd_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mode = node->arg(0);
    uint32_t srcb = node->arg(1);
    uint32_t srcc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    return fmt_print("$%d = sfpstochrnd_v(%d, $%d, $%d, %d)", ret, mode, srcb, srcc, mod1);
}

std::string fmt_sfptransp(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    return fmt_print("sfptransp($%d, $%d, $%d, $%d)", l0, l1, l2, l3);
}

std::string fmt_sfpsfht2_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t imm = node->arg(1);
    return fmt_print("$%d = sfpshft2_i($%d, 0x%x)", ret, dst, imm);
}

std::string fmt_sfpshft2_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    return fmt_print("$%d = sfpshft2_i($%d, 0x%x)", ret, dst, src);
}

std::string fmt_sfpshft2_g(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    uint32_t mod1 = node->arg(4);
    return fmt_print("sfpshft2_g($%d, $%d, $%d, $%d, %d)", l0, l1, l2, l3, mod1);
}

std::string fmt_sfpshft2_ge(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t l0 = node->arg(1);
    uint32_t l1 = node->arg(2);
    uint32_t l2 = node->arg(3);
    uint32_t l3 = node->arg(4);
    return fmt_print("sfpshft2_ge($%d, $%d, $%d, $%d, $%d)", src, l0, l1, l2, l3);
}

std::string fmt_sfpshft2_e(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    return fmt_print("$%d = sfpshft2_e($%d, %d)", ret, src, mod1);
}

std::string fmt_sfpswap(Node *node) {
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("sfpswap($%d, $%d, %d)", dst, src, mod1);
}

std::string fmt_sfpconfig_v(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t config_dest = node->arg(1);
    return fmt_print("sfpconfig_v($%d, %d)", l0, config_dest);
}

std::string fmt_sfploadi(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mod0 = node->arg(0);
    uint32_t imm = node->arg(1);
    return fmt_print("$%d = sfploadi(%d, 0x%x)", ret, mod0, imm); 
}

std::string fmt_sfpiadd_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t imm = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpiadd_i($%d, 0x%x, %d)", ret, src, imm, mod1);
}

std::string fmt_sfpiadd_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    return fmt_print("$%d = sfpiadd_v($%d, $%d, %d)", ret, dst, src, mod1);
}

std::string fmt_sfpdiag(Node *node) {
    uint32_t id = node->arg(0);
    return fmt_print("sfpdiag(%d)", id);
}

} // namespace

std::string fmt_node(Node *node) {
    switch (node->op()) {
    case Op::SFPINCRWC:
        return fmt_sfpincrwc(node);
    case Op::SFPASSIGN_LV:
        return fmt_sfpassign_lv(node);
    case Op::SFPLOAD:
        return fmt_sfpload(node);
    case Op::SFPASSIGNLREG:
        return fmt_sfpassignlreg(node);
    case Op::SFPPRESERVELREG:
        return fmt_sfppreservelreg(node);
    case Op::SFPXLOADI:
        return fmt_sfpxloadi(node);
    case Op::SFPSTORE:
        return fmt_sfpstore(node);
    case Op::SFPMOV:
        return fmt_sfpmov(node);
    case Op::SFPNOP:
        return fmt_sfpnop(node);
    case Op::SFPILLEGAL:
        return fmt_sfpillegal(node);
    case Op::SFPENCC:
        return fmt_sfpencc(node);
    case Op::SFPPUSHC:
        return fmt_sfppushc(node);
    case Op::SFPPOPC:
        return fmt_sfppopc(node);
    case Op::SFPSETCC_V:
        return fmt_sfpsetcc_v(node);
    case Op::SFPSETCC_I:
        return fmt_sfpsetcc_i(node);
    case Op::SFPXFCMPS:
        return fmt_sfpxfcmps(node);
    case Op::SFPXFCMPV:
        return fmt_sfpxfcmpv(node);
    case Op::SFPXICMPS:
        return fmt_sfpxicmps(node);
    case Op::SFPXICMPV:
        return fmt_sfpxicmpv(node);
    case Op::SFPXVIF:
        return fmt_sfpxvif(node);
    case Op::SFPXBOOL:
        return fmt_sfpxbool(node);
    case Op::SFPXCONDB:
        return fmt_sfpxcondb(node);
    case Op::SFPXCONDI:
        return fmt_sfpxcondi(node);
    case Op::SFPCOMPC:
        return fmt_sfpcompc(node);
    case Op::SFPADD:
        return fmt_sfpadd(node);
    case Op::SFPMUL:
        return fmt_sfpmul(node);
    case Op::SFPMAD:
        return fmt_sfpmad(node);
    case Op::SFPEXEXP:
        return fmt_sfpexexp(node);
    case Op::SFPEXMAN:
        return fmt_sfpexman(node);
    case Op::SFPSETEXP_I:
        return fmt_sfpsetexp_i(node);
    case Op::SFPSETEXP_V:
        return fmt_sfpsetexp_v(node);
    case Op::SFPSETMAN_I:
        return fmt_sfpsetman_i(node);
    case Op::SFPSETMAN_V:
        return fmt_sfpsetman_v(node);
    case Op::SFPABS:
        return fmt_sfpabs(node);
    case Op::SFPAND:
        return fmt_sfpand(node);
    case Op::SFPOR:
        return fmt_sfpor(node);
    case Op::SFPXOR:
        return fmt_sfpxor(node);
    case Op::SFPNOT:
        return fmt_sfpnot(node);
    case Op::SFPDIVP2:
        return fmt_sfpdivp2(node);
    case Op::SFPLZ:
        return fmt_sfplz(node);
    case Op::SFPSHFT_I:
        return fmt_sfpshft_i(node);
    case Op::SFPSHFT_V:
        return fmt_sfpshft_v(node);
    case Op::SFPXIADD_I:
        return fmt_sfpxiadd_i(node);
    case Op::SFPXIADD_V:
        return fmt_sfpxiadd_v(node);
    case Op::SFPSETSGN_I:
        return fmt_sfpsetsgn_i(node);
    case Op::SFPSETSGN_V:
        return fmt_sfpsetsgn_v(node);
    case Op::SFPLUT:
        return fmt_sfplut(node);
    case Op::SFPLUTFP32_3R:
        return fmt_sfplutfp32_3r(node);
    case Op::SFPLUTFP32_6R:
        return fmt_sfplutfp32_6r(node);
    case Op::SFPCAST:
        return fmt_sfpcast(node);
    case Op::SFPSTOCHRND_I:
        return fmt_sfpstochrnd_i(node);
    case Op::SFPSTOCHRND_V:
        return fmt_sfpstochrnd_v(node);
    case Op::SFPTRANSP:
        return fmt_sfptransp(node);
    case Op::SFPSHFT2_I:
        return fmt_sfpsfht2_i(node);
    case Op::SFPSHFT2_V:
        return fmt_sfpshft2_v(node);
    case Op::SFPSHFT2_G:
        return fmt_sfpshft2_g(node);
    case Op::SFPSHFT2_GE:
        return fmt_sfpshft2_ge(node);
    case Op::SFPSHFT2_E:
        return fmt_sfpshft2_e(node);
    case Op::SFPSWAP:
        return fmt_sfpswap(node);
    case Op::SFPCONFIG_V:
        return fmt_sfpconfig_v(node);
    // emitted only
    case Op::SFPLOADI:
        return fmt_sfploadi(node);
    case Op::SFPIADD_I:
        return fmt_sfpiadd_i(node);
    case Op::SFPIADD_V:
        return fmt_sfpiadd_v(node);
    // pseudo-instructions
    case Op::SFPDIAG:
        return fmt_sfpdiag(node);
    default:
        return fmt_print("Invalid op code: %d", int(node->op()));
    }
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

