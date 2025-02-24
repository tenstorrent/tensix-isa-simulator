// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <cassert>

#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    RegMap
//

RegMap::RegMap():
        m_map(1024, REG_NONE),
        m_top(0) { }

RegMap::~RegMap() { }

void RegMap::reset() {
    uint32_t size = uint32_t(m_map.size());
    for (uint32_t i = 0; i < size; i++) {
        m_map[i] = REG_NONE;
    }
    m_top = 0;
}

uint32_t RegMap::size() {
    return m_top;
}

void RegMap::bind(uint32_t id, uint32_t reg) {
    uint32_t size = uint32_t(m_map.size());
    if (id >= size) {
        uint32_t size = (id & ~uint32_t(1023)) + 1024;
        m_map.resize(size, REG_NONE);
    }
    assert(m_map[id] == REG_NONE);
    m_map[id] = reg;
    if (id >= m_top) {
        m_top = id + 1;
    }
}

uint32_t RegMap::map(uint32_t id) {
    return (id < m_top) ? m_map[id] : REG_NONE;
}

//
//    Code
//

// construction/destruction

Code::Code():
        m_hw(nullptr),
        m_next_vreg(16),
        m_text_pool(nullptr) { }

Code::~Code() { }

// interface

void Code::reset() {
    m_graph.reset();
    m_next_vreg = 16;
    m_vreg_map.reset();
    m_lreg_map.reset();
}

void Code::set_text_pool(TextPool *text_pool) {
    m_text_pool = text_pool;
}

void Code::set_hw(Hw *hw) {
    m_hw = hw;
}

void Code::emit(Node *node) {
    switch (node->op()) {
    case Op::SFPINCRWC:
        emit_sfpincrwc(node);
        break;
    case Op::SFPASSIGN_LV:
        emit_sfpassign_lv(node);
        break;
    case Op::SFPLOAD:
        emit_sfpload(node);
        break;
    case Op::SFPASSIGNLREG:
        emit_sfpassignlreg(node);
        break;
    case Op::SFPPRESERVELREG:
        emit_sfppreservelreg(node);
        break;
    case Op::SFPXLOADI:
        emit_sfpxloadi(node);
        break;
    case Op::SFPSTORE:
        emit_sfpstore(node);
        break;
    case Op::SFPMOV:
        emit_sfpmov(node);
        break;
    case Op::SFPNOP:
        emit_sfpnop(node);
        break;
    case Op::SFPILLEGAL:
        emit_sfpillegal(node);
        break;
    case Op::SFPENCC:
        emit_sfpencc(node);
        break;
    case Op::SFPPUSHC:
        emit_sfppushc(node);
        break;
    case Op::SFPPOPC:
        emit_sfppopc(node);
        break;
    case Op::SFPSETCC_V:
        emit_sfpsetcc_v(node);
        break;
    case Op::SFPSETCC_I:
        emit_sfpsetcc_i(node);
        break;
    case Op::SFPXFCMPS:
        emit_sfpxfcmps(node);
        break;
    case Op::SFPXFCMPV:
        emit_sfpxfcmpv(node);
        break;
    case Op::SFPXICMPS:
        emit_sfpxicmps(node);
        break;
    case Op::SFPXICMPV:
        emit_sfpxicmpv(node);
        break;
    case Op::SFPXVIF:
        emit_sfpxvif(node);
        break;
    case Op::SFPXBOOL:
        emit_sfpxbool(node);
        break;
    case Op::SFPXCONDB:
        emit_sfpxcondb(node);
        break;
    case Op::SFPXCONDI:
        emit_sfpxcondi(node);
        break;
    case Op::SFPCOMPC:
        emit_sfpcompc(node);
        break;
    case Op::SFPADD:
        emit_sfpadd(node);
        break;
    case Op::SFPMUL:
        emit_sfpmul(node);
        break;
    case Op::SFPMAD:
        emit_sfpmad(node);
        break;
    case Op::SFPEXEXP:
        emit_sfpexexp(node);
        break;
    case Op::SFPEXMAN:
        emit_sfpexman(node);
        break;
    case Op::SFPSETEXP_I:
        emit_sfpsetexp_i(node);
        break;
    case Op::SFPSETEXP_V:
        emit_sfpsetexp_v(node);
        break;
    case Op::SFPSETMAN_I:
        emit_sfpsetman_i(node);
        break;
    case Op::SFPSETMAN_V:
        emit_sfpsetman_v(node);
        break;
    case Op::SFPABS:
        emit_sfpabs(node);
        break;
    case Op::SFPAND:
        emit_sfpand(node);
        break;
    case Op::SFPOR:
        emit_sfpor(node);
        break;
    case Op::SFPXOR:
        emit_sfpxor(node);
        break;
    case Op::SFPNOT:
        emit_sfpnot(node);
        break;
    case Op::SFPDIVP2:
        emit_sfpdivp2(node);
        break;
    case Op::SFPLZ:
        emit_sfplz(node);
        break;
    case Op::SFPSHFT_I:
        emit_sfpshft_i(node);
        break;
    case Op::SFPSHFT_V:
        emit_sfpshft_v(node);
        break;
    case Op::SFPXIADD_I:
        emit_sfpxiadd_i(node);
        break;
    case Op::SFPXIADD_V:
        emit_sfpxiadd_v(node);
        break;
    case Op::SFPSETSGN_I:
        emit_sfpsetsgn_i(node);
        break;
    case Op::SFPSETSGN_V:
        emit_sfpsetsgn_v(node);
        break;
    case Op::SFPLUT:
        emit_sfplut(node);
        break;
    case Op::SFPLUTFP32_3R:
        emit_sfplutfp32_3r(node);
        break;
    case Op::SFPLUTFP32_6R:
        emit_sfplutfp32_6r(node);
        break;
    case Op::SFPCAST:
        emit_sfpcast(node);
        break;
    case Op::SFPSTOCHRND_I:
        emit_sfpstochrnd_i(node);
        break;
    case Op::SFPSTOCHRND_V:
        emit_sfpstochrnd_v(node);
        break;
    case Op::SFPTRANSP:
        emit_sfptransp(node);
        break;
    case Op::SFPSHFT2_I:
        emit_sfpshft2_i(node);
        break;
    case Op::SFPSHFT2_V:
        emit_sfpshft2_v(node);
        break;
    case Op::SFPSHFT2_G:
        emit_sfpshft2_g(node);
        break;
    case Op::SFPSHFT2_GE:
        emit_sfpshft2_ge(node);
        break;
    case Op::SFPSHFT2_E:
        emit_sfpshft2_e(node);
        break;
    case Op::SFPSWAP:
        emit_sfpswap(node);
        break;
    case Op::SFPCONFIG_V:
        emit_sfpconfig_v(node);
        break;
    case Op::SFPDIAG:
        emit_sfpdiag(node);
        break;
    default:
        assert(false);
        break;
    }
}

void Code::alloc_lregs() {
    m_reg_alloc.run(&m_graph, &m_lreg_map);
}

void Code::gen() {
    for (Node *node = m_graph.head(); node != nullptr; node = node->next()) {
        gen_node(node);
    }
}

void Code::dump_graph() {
    for (Node *node = m_graph.head(); node != nullptr; node = node->next()) {
        if (node->op() == Op::SFPDIAG) {
            if (m_text_pool != nullptr) {
                uint32_t id = node->arg(0);
                std::string text = m_text_pool->get(id);
                printf("# %s\n", text.c_str());
            }
        } else {
            std::string text = fmt_node(node);
            printf("%s\n", text.c_str());
        }
    }
}

// emit

void Code::emit_sfpincrwc(Node *node) {
    uint32_t cr = node->arg(0);
    uint32_t d = node->arg(1);
    uint32_t b = node->arg(2);
    uint32_t a = node->arg(3);
    push_node(Op::SFPINCRWC, RET_NONE, cr, d, b, a);
}

void Code::emit_sfpassign_lv(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t v = vreg_map_arg(node, 0);
    uint32_t in = vreg_map_arg(node, 1);
    push_node(Op::SFPASSIGN_LV, ret, v, in);
}

void Code::emit_sfpload(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t mod0 = node->arg(0);
    uint32_t mode = node->arg(1);
    uint32_t addr = node->arg(2);
    expand_sfpload(ret, mod0, mode, addr);
}

void Code::emit_sfpassignlreg(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t lr = node->arg(0);
    expand_sfpassignlreg(ret, lr);
}

void Code::emit_sfppreservelreg(Node *node) {
    uint32_t x = vreg_map_arg(node, 0);
    uint32_t n = node->arg(1);
    // SKIPPED: Mapping to SFPPRESERVELREG<n>
    push_node(Op::SFPPRESERVELREG, RET_NONE, x, n);
}

void Code::emit_sfpxloadi(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t mod0 = node->arg(0);
    uint32_t imm = node->arg(1);
    expand_sfpxloadi(ret, mod0, imm);
}

void Code::emit_sfpstore(Node *node) {
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod0 = node->arg(1);
    uint32_t mode = node->arg(2);
    uint32_t addr = node->arg(3);
    expand_sfpstore(src, mod0, mode, addr);
}

void Code::emit_sfpmov(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPMOV, ret, src, mod1);
}

void Code::emit_sfpnop(Node *node) {
    push_node(Op::SFPNOP, RET_NONE);
}

void Code::emit_sfpillegal(Node *node) {
    push_node(Op::SFPILLEGAL, RET_NONE);
}

void Code::emit_sfpencc(Node *node) {
    uint32_t imm = node->arg(0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPENCC, RET_NONE, imm, mod1); 
}

void Code::emit_sfppushc(Node *node) {
    push_node(Op::SFPPUSHC, RET_NONE);
}

void Code::emit_sfppopc(Node *node) {
    push_node(Op::SFPPOPC, RET_NONE);
}

void Code::emit_sfpsetcc_v(Node *node) {
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPSETCC_V, RET_NONE, src, mod1);
}

void Code::emit_sfpsetcc_i(Node *node) {
    uint32_t imm = node->arg(0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPSETCC_I, RET_NONE, imm, mod1);
}

void Code::emit_sfpxfcmps(Node *node) {
    uint32_t v = vreg_map_arg(node, 0);
    uint32_t f = node->arg(1);
    uint32_t mod1 = node->arg(2);
    expand_sfpxfcmps(v, f, mod1);
}

void Code::emit_sfpxfcmpv(Node *node) {
    uint32_t v1 = vreg_map_arg(node, 0);
    uint32_t v2 = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    expand_sfpxfcmpv(v1, v2, mod1);
}

void Code::emit_sfpxicmps(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v = vreg_map_arg(node, 0);
    uint32_t i = node->arg(1);
    uint32_t mod1 = node->arg(2);
    push_node(Op::SFPXICMPS, ret, v, i, mod1);
}

void Code::emit_sfpxicmpv(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v1 = vreg_map_arg(node, 0);
    uint32_t v2 = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    push_node(Op::SFPXICMPV, ret, v1, v2, mod1);
}

void Code::emit_sfpxvif(Node *node) {
    uint32_t ret = node->ret();
    push_node(Op::SFPXVIF, ret);
}

void Code::emit_sfpxbool(Node *node) {
    uint32_t ret = node->ret();
    uint32_t t = node->arg(0);
    uint32_t a = node->arg(1);
    uint32_t b = node->arg(2);
    push_node(Op::SFPXBOOL, ret, t, a, b);
}

void Code::emit_sfpxcondb(Node *node) {
    uint32_t s = node->arg(0);
    uint32_t i = node->arg(1);
    push_node(Op::SFPXCONDB, RET_NONE, s, i);
}

void Code::emit_sfpxcondi(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t i = node->arg(0);
    push_node(Op::SFPXCONDI, ret, i);
}

void Code::emit_sfpcompc(Node *node) {
    push_node(Op::SFPCOMPC, RET_NONE);
}

void Code::emit_sfpadd(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t va = vreg_map_arg(node, 0);
    uint32_t vb = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    push_node(Op::SFPADD, ret, va, vb, mod1);
}

void Code::emit_sfpmul(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t va = vreg_map_arg(node, 0);
    uint32_t vb = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    push_node(Op::SFPMUL, ret, va, vb, mod1);
}

void Code::emit_sfpmad(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t va = vreg_map_arg(node, 0);
    uint32_t vb = vreg_map_arg(node, 1);
    uint32_t vc = vreg_map_arg(node, 2);
    uint32_t mod1 = node->arg(3);
    push_node(Op::SFPMAD, ret, va, vb, vc, mod1);
}

void Code::emit_sfpexexp(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPEXEXP, ret, src, mod1);
}

void Code::emit_sfpexman(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPEXMAN, ret, src, mod1);
}

void Code::emit_sfpsetexp_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t imm = node->arg(0);
    uint32_t src = vreg_map_arg(node, 1);
    expand_sfpsetexp_i(ret, imm, src);
}

void Code::emit_sfpsetexp_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPSETEXP_V, ret, dst, src);
}

void Code::emit_sfpsetman_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node); 
    uint32_t imm = node->arg(0);
    uint32_t src = vreg_map_arg(node, 1);
    uint32_t mod1 = vreg_map_arg(node, 2);
    expand_sfpsetman(ret, imm, src);
}

void Code::emit_sfpsetman_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPSETMAN_V, ret, dst, src);
}

void Code::emit_sfpabs(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPABS, ret, src, mod1);
}

void Code::emit_sfpand(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPAND, ret, dst, src);
}

void Code::emit_sfpor(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPOR, ret, dst, src);
}

void Code::emit_sfpxor(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPXOR, ret, dst, src);
}

void Code::emit_sfpnot(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    push_node(Op::SFPNOT, ret, src);
}

void Code::emit_sfpdivp2(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t imm = node->arg(0);
    uint32_t src = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    expand_sfpdivp2(ret, imm, src, mod1);
}

void Code::emit_sfplz(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPLZ, ret, src, mod1);
}

void Code::emit_sfpshft_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t imm = node->arg(1);
    expand_sfpshft_i(ret, dst, imm);
}

void Code::emit_sfpshft_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPSHFT_V, ret, dst, src);
}

void Code::emit_sfpxiadd_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t imm = node->arg(1);
    uint32_t mod1 = node->arg(2);
    expand_sfpxiadd_i(ret, src, imm, mod1);
}

void Code::emit_sfpxiadd_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    expand_sfpxiadd_v(ret, dst, src, mod1);
}

void Code::emit_sfpsetsgn_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t imm = node->arg(0);
    uint32_t src = vreg_map_arg(node, 1);
    expand_sfpsetsgn_i(ret, imm, src);
}

void Code::emit_sfpsetsgn_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPSETSGN_V, ret, dst, src);
}

void Code::emit_sfplut(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l2 and dst (= l3)
    uint32_t ret = vreg_bind_ret(node);
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t l1 = vreg_map_arg(node, 1);
    uint32_t l2 = vreg_map_arg(node, 2);
    uint32_t dst = vreg_map_arg(node, 3);
    uint32_t mod0 = node->arg(4);
    push_node(Op::SFPLUT, ret, l0, l1, l2, dst, mod0);
}

void Code::emit_sfplutfp32_3r(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l2
    uint32_t ret = vreg_bind_ret(node);
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t l1 = vreg_map_arg(node, 1);
    uint32_t l2 = vreg_map_arg(node, 2);
    uint32_t l3 = vreg_map_arg(node, 3);
    uint32_t mod0 = node->arg(4);
    push_node(Op::SFPLUTFP32_3R, ret, l0, l1, l2, l3, mod0);
}

void Code::emit_sfplutfp32_6r(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l6
    uint32_t ret = vreg_bind_ret(node);
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t l1 = vreg_map_arg(node, 1);
    uint32_t l2 = vreg_map_arg(node, 2);
    uint32_t l4 = vreg_map_arg(node, 3);
    uint32_t l5 = vreg_map_arg(node, 4);
    uint32_t l6 = vreg_map_arg(node, 5);
    uint32_t l3 = vreg_map_arg(node, 6);
    uint32_t mod0 = node->arg(7);
    push_node(Op::SFPLUTFP32_6R, ret, l0, l1, l2, l4, l5, l6, l3, mod0);
}

void Code::emit_sfpcast(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    push_node(Op::SFPCAST, ret, src, mod1);
}

void Code::emit_sfpstochrnd_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t mode = node->arg(0);
    uint32_t imm = node->arg(1);
    uint32_t srcc = vreg_map_arg(node, 2);
    uint32_t mod1 = node->arg(3);
    expand_sfpstochrnd_i(ret, mode, imm, srcc, mod1);
}

void Code::emit_sfpstochrnd_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t mode = node->arg(0);
    uint32_t srcb = vreg_map_arg(node, 1);
    uint32_t srcc = vreg_map_arg(node, 2);
    uint32_t mod1 = node->arg(3);
    push_node(Op::SFPSTOCHRND_V, ret, mode, srcb, srcc, mod1);
}

void Code::emit_sfptransp(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l3 ?
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t l1 = vreg_map_arg(node, 1);
    uint32_t l2 = vreg_map_arg(node, 2);
    uint32_t l3 = vreg_map_arg(node, 3);
    push_node(Op::SFPTRANSP, RET_NONE, l0, l1, l2, l3);
}

void Code::emit_sfpshft2_i(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t imm = node->arg(1);
    push_node(Op::SFPSHFT2_I, ret, dst, imm);
}

void Code::emit_sfpshft2_v(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    push_node(Op::SFPSHFT2_V, ret, dst, src);
}

void Code::emit_sfpshft2_g(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l3 ?
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t l1 = vreg_map_arg(node, 1);
    uint32_t l2 = vreg_map_arg(node, 2);
    uint32_t l3 = vreg_map_arg(node, 3);
    uint32_t mod1 = node->arg(4);
    push_node(Op::SFPSHFT2_G, RET_NONE, l0, l1, l2, l3, mod1);
}

void Code::emit_sfpshft2_ge(Node *node) {
    // ACHTUNG: Special requirements for l0 .. l3 ?
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t l0 = vreg_map_arg(node, 1);
    uint32_t l1 = vreg_map_arg(node, 2);
    uint32_t l2 = vreg_map_arg(node, 3);
    uint32_t l3 = vreg_map_arg(node, 4);
    push_node(Op::SFPSHFT2_GE, RET_NONE, src, l0, l1, l2, l3);
}

void Code::emit_sfpshft2_e(Node *node) {
    uint32_t ret = vreg_bind_ret(node);
    uint32_t src = vreg_map_arg(node, 0);
    uint32_t mod1 = node->arg(1);
    expand_sfpshft2_e(ret, src, mod1);
}

void Code::emit_sfpswap(Node *node) {
    uint32_t dst = vreg_map_arg(node, 0);
    uint32_t src = vreg_map_arg(node, 1);
    uint32_t mod1 = node->arg(2);
    push_node(Op::SFPSWAP, RET_NONE, dst, src, mod1);
}

void Code::emit_sfpconfig_v(Node *node) {
    uint32_t l0 = vreg_map_arg(node, 0);
    uint32_t config_dest = node->arg(1);
    push_node(Op::SFPCONFIG_V, RET_NONE, l0, config_dest);
}

void Code::emit_sfpdiag(Node *node) {
    uint32_t id = node->arg(0);
    push_node(Op::SFPDIAG, RET_NONE, id);
}

// emit helpers

void Code::push_node(
        Op op,
        uint32_t ret,
        uint32_t arg0/* = 0*/,
        uint32_t arg1/* = 0*/,
        uint32_t arg2/* = 0*/,
        uint32_t arg3/* = 0*/,
        uint32_t arg4/* = 0*/,
        uint32_t arg5/* = 0*/,
        uint32_t arg6/* = 0*/,
        uint32_t arg7/* = 0*/) {
    Node *node = m_graph.new_node(op, ret, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    m_graph.push(node);
}

uint32_t Code::vreg_map_arg(Node *node, int index) {
    uint32_t id = node->arg(index);
    uint32_t vreg = m_vreg_map.map(id);
    assert(vreg != RegMap::REG_NONE);
    return vreg;
}

uint32_t Code::vreg_bind_ret(Node *node) {
    uint32_t id = node->ret();
    assert(id != Ir::RET_NONE);
    uint32_t vreg = vreg_alloc();
    vreg_bind(id, vreg);
    return vreg;
}

uint32_t Code::vreg_alloc() {
    uint32_t vreg = m_next_vreg;
    m_next_vreg++;
    return vreg;
}

void Code::vreg_bind(uint32_t id, uint32_t vreg) {
    m_vreg_map.bind(id, vreg);
}

void Code::lreg_bind(uint32_t vreg, uint32_t lreg) {
    m_lreg_map.bind(vreg, lreg);
}

uint32_t Code::lreg_map(uint32_t vreg) {
    return m_lreg_map.map(vreg);
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

