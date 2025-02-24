// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <string>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    RvttImpl
//

//
//    Construction/destruction
//

RvttImpl::RvttImpl():
        m_ir_expand(new_ir_expand()),
        m_ir_cc(new_ir_cc()) { 
    m_ir.set_text_pool(&m_text_pool);
    m_code.set_text_pool(&m_text_pool);
}

RvttImpl::~RvttImpl() { }

void RvttImpl::set_hw(Hw *hw) {
    m_code.set_hw(hw);
}

void RvttImpl::reset() {
    m_ir.reset();
    m_code.reset();
    m_text_pool.reset();
}

void RvttImpl::compile() {
    dump_graph("Before IR expand");
    m_ir_expand->transform(&m_ir);

    dump_graph("Before IR CC");
    m_ir_cc->transform(&m_ir);

    dump_graph("Before code emit");
    for (Node *node = m_ir.head(); node != nullptr; node = node->next()) {
        m_code.emit(node);
    }

    dump_graph("Before code alloc LREGs");
    m_code.alloc_lregs();
    m_code.schedule();

    m_code.gen();
}

void RvttImpl::sfpincrwc(uint32_t cr, uint32_t d, uint32_t b, uint32_t a) {
    push_node_ret_none(Op::SFPINCRWC, cr, d, b, a);
}

Vec RvttImpl::sfpassign_lv(Vec v, Vec in) {
    uint32_t ret = push_node_ret_auto(Op::SFPASSIGN_LV, v.get(), in.get());
    return Vec(ret);
}

Vec RvttImpl::sfpload(uint32_t mod0, uint32_t mode, uint32_t addr) {
    uint32_t ret = push_node_ret_auto(Op::SFPLOAD, mod0, mode, addr);
    return Vec(ret);
}

Vec RvttImpl::sfpassignlreg(int lr) {
    uint32_t ret = push_node_ret_auto(Op::SFPASSIGNLREG, uint32_t(lr));
    return Vec(ret);
}

void RvttImpl::sfppreservelreg(Vec x, int n) {
    push_node_ret_none(Op::SFPPRESERVELREG, x.get(), uint32_t(n));
}

Vec RvttImpl::sfpxloadi(uint32_t mod0, uint32_t imm) {
    uint32_t ret = push_node_ret_auto(Op::SFPXLOADI, mod0, imm);
    return Vec(ret);
}

void RvttImpl::sfpstore(Vec src, uint32_t mod0, uint32_t mode, uint32_t addr) {
    push_node_ret_none(Op::SFPSTORE, src.get(), mod0, mode, addr);
}

Vec RvttImpl::sfpmov(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPMOV, src.get(), mod1);
    return Vec(ret);
}

void RvttImpl::sfpnop() {
    push_node_ret_none(Op::SFPNOP);
}

void RvttImpl::sfpillegal() {
    push_node_ret_none(Op::SFPILLEGAL);
}

void RvttImpl::sfpencc(uint32_t imm, uint32_t mod1) {
    push_node_ret_none(Op::SFPENCC, imm, mod1); 
}

void RvttImpl::sfppushc() {
    push_node_ret_none(Op::SFPPUSHC);
}

void RvttImpl::sfppopc() {
    push_node_ret_none(Op::SFPPOPC);
}

void RvttImpl::sfpsetcc_v(Vec src, uint32_t mod1) {
    push_node_ret_none(Op::SFPSETCC_V, src.get(), mod1);
}
 
void RvttImpl::sfpsetcc_i(uint32_t imm, uint32_t mod1) {
    push_node_ret_none(Op::SFPSETCC_I, imm, mod1);
}

int RvttImpl::sfpxfcmps(Vec v, uint32_t f, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXFCMPS, v.get(), f, mod1);
    return int(ret);
}

int RvttImpl::sfpxfcmpv(Vec v1, Vec v2, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXFCMPV, v1.get(), v2.get(), mod1);
    return int(ret);
}

int RvttImpl::sfpxicmps(Vec v, uint32_t i, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXICMPS, v.get(), i, mod1);
    return int(ret);
}

int RvttImpl::sfpxicmpv(Vec v1, Vec v2, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXICMPV, v1.get(), v2.get(), mod1);
    return int(ret);
}

int RvttImpl::sfpxvif() {
    uint32_t ret = push_node_ret_auto(Op::SFPXVIF);
    return int(ret);
}

int RvttImpl::sfpxbool(int t, int a, int b) {
    uint32_t ret = push_node_ret_auto(Op::SFPXBOOL, uint32_t(t), uint32_t(a), uint32_t(b));
    return int(ret);
}

void RvttImpl::sfpxcondb(int s, int i) {
    push_node_ret_none(Op::SFPXCONDB, uint32_t(s), uint32_t(i));
}

Vec RvttImpl::sfpxcondi(int i) {
    uint32_t ret = push_node_ret_auto(Op::SFPXCONDI, uint32_t(i));
    return Vec(ret);
}

void RvttImpl::sfpcompc() {
    push_node_ret_none(Op::SFPCOMPC);
}

Vec RvttImpl::sfpadd(Vec va, Vec vb, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPADD, va.get(), vb.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpmul(Vec va, Vec vb, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPMUL, va.get(), vb.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpmad(Vec va, Vec vb, Vec vc, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPMAD, va.get(), vb.get(), vc.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpexexp(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPEXEXP, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpexman(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPEXMAN, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpsetexp_i(uint32_t imm, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETEXP_I, imm, src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpsetexp_v(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETEXP_V, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpsetman_i(uint32_t imm, Vec src, uint32_t mod) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETMAN_I, imm, src.get(), mod);
    return Vec(ret);
}

Vec RvttImpl::sfpsetman_v(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETMAN_V, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpabs(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPABS, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpand(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPAND, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpor(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPOR, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpxor(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPXOR, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpnot(Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPNOT, src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpdivp2(uint32_t imm, Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPDIVP2, imm, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfplz(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPLZ, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpshft_i(Vec dst, uint32_t imm) {
    uint32_t ret = push_node_ret_auto(Op::SFPSHFT_I, dst.get(), imm);
    return Vec(ret);
}

Vec RvttImpl::sfpshft_v(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSHFT_V, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpxiadd_i(Vec src, uint32_t imm, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXIADD_I, src.get(), imm, mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpxiadd_v(Vec dst, Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPXIADD_V, dst.get(), src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpsetsgn_i(uint32_t imm, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETSGN_I, imm, src.get());
    return Vec(ret);
}

Vec RvttImpl::sfpsetsgn_v(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSETSGN_V, dst.get(), src.get());
    return Vec(ret);
}

Vec RvttImpl::sfplut(Vec l0, Vec l1, Vec l2, Vec dst, uint32_t mod0) {
    uint32_t ret = 
        push_node_ret_auto(
            Op::SFPLUT, 
            l0.get(), 
            l1.get(), 
            l2.get(), 
            dst.get(),
            mod0);
    return Vec(ret);
}

Vec RvttImpl::sfplutfp32_3r(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod0) {
    uint32_t ret = 
        push_node_ret_auto(
            Op::SFPLUTFP32_3R, 
            l0.get(), 
            l1.get(), 
            l2.get(), 
            l3.get(), 
            mod0);
    return Vec(ret);
}

Vec RvttImpl::sfplutfp32_6r(
        Vec l0, 
        Vec l1, 
        Vec l2, 
        Vec l4, 
        Vec l5, 
        Vec l6, 
        Vec l3, 
        uint32_t mod0) {
    uint32_t ret =
        push_node_ret_auto(
            Op::SFPLUTFP32_6R, 
            l0.get(), 
            l1.get(), 
            l2.get(), 
            l4.get(), 
            l5.get(), 
            l6.get(), 
            l3.get(), 
            mod0);
    return Vec(ret);
}

Vec RvttImpl::sfpcast(Vec src, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPCAST, src.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpstochrnd_i(uint32_t mode, uint32_t imm, Vec srcc, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPSTOCHRND_I, mode, imm, srcc.get(), mod1);
    return Vec(ret);
}

Vec RvttImpl::sfpstochrnd_v(uint32_t mode, Vec srcb, Vec srcc, uint32_t mod1) {
    uint32_t ret = push_node_ret_auto(Op::SFPSTOCHRND_V, mode, srcb.get(), srcc.get(), mod1);
    return Vec(ret);
}

void RvttImpl::sfptransp(Vec l0, Vec l1, Vec l2, Vec l3) {
    push_node_ret_none(Op::SFPTRANSP, l0.get(), l1.get(), l2.get(), l3.get());
}

Vec RvttImpl::sfpshft2_i(Vec dst, uint32_t imm) {
    uint32_t ret = push_node_ret_auto(Op::SFPSHFT2_I, dst.get(), imm);
    return Vec(ret);
}

Vec RvttImpl::sfpshft2_v(Vec dst, Vec src) {
    uint32_t ret = push_node_ret_auto(Op::SFPSHFT2_V, dst.get(), src.get());
    return Vec(ret);
}

void RvttImpl::sfpshft2_g(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod) {
    push_node_ret_none(Op::SFPSHFT2_G, l0.get(), l1.get(), l2.get(), l3.get(), mod);
}

void RvttImpl::sfpshft2_ge(Vec src, Vec l0, Vec l1, Vec l2, Vec l3) {
    push_node_ret_none(Op::SFPSHFT2_GE, src.get(), l0.get(), l1.get(), l2.get(), l3.get());
}

Vec RvttImpl::sfpshft2_e(Vec src, uint32_t mod) {
    uint32_t ret = push_node_ret_auto(Op::SFPSHFT2_E, src.get(), mod);
    return Vec(ret);
}

void RvttImpl::sfpswap(Vec dst, Vec src, uint32_t mod) {
    push_node_ret_none(Op::SFPSWAP, dst.get(), src.get(), mod);
}

void RvttImpl::sfpconfig_v(Vec l0, uint32_t config_dest) {
    push_node_ret_none(Op::SFPCONFIG_V, l0.get(), config_dest);
}

void RvttImpl::print_text(const char *fmt, ...) {
    if (!ENABLE_PRINT_TEXT) {
        return;
    }
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 256, fmt, args);
    va_end(args);
    uint32_t id = m_text_pool.push(std::string(buf));
    push_node_ret_none(Op::SFPDIAG, id);
}

void RvttImpl::push_node_ret_none(
        Op op,
        uint32_t arg0/* = 0*/,
        uint32_t arg1/* = 0*/,
        uint32_t arg2/* = 0*/,
        uint32_t arg3/* = 0*/,
        uint32_t arg4/* = 0*/,
        uint32_t arg5/* = 0*/,
        uint32_t arg6/* = 0*/,
        uint32_t arg7/* = 0*/) {
    Node *node = m_ir.new_node(op, Ir::RET_NONE, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    m_ir.push(node);
}

uint32_t RvttImpl::push_node_ret_auto(
        Op op,
        uint32_t arg0/* = 0*/,
        uint32_t arg1/* = 0*/,
        uint32_t arg2/* = 0*/,
        uint32_t arg3/* = 0*/,
        uint32_t arg4/* = 0*/,
        uint32_t arg5/* = 0*/,
        uint32_t arg6/* = 0*/,
        uint32_t arg7/* = 0*/) {
    Node *node = m_ir.new_node(op, Ir::RET_AUTO, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    m_ir.push(node);
    return node->ret();
}

void RvttImpl::dump_graph(const char *tag) {
    if (!ENABLE_DUMP_GRAPH) {
        return;
    }
    printf("---- %s\n", tag);
    m_ir.dump_graph();
    printf("\n");
}

//
//    Global variables
//

std::unique_ptr<Rvtt> rvtt(new RvttImpl());

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

