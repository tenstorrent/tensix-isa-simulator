
#include <cstdint>
#include <cassert>
#include <string>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Code
//

//
//    Gen methods
//

void Code::gen_node(Node *node) {
    assert(m_hw != nullptr);
    switch (node->op()) {
    case Op::SFPINCRWC:
        gen_sfpincrwc(node);
        break;
    case Op::SFPASSIGN_LV:
        gen_sfpassign_lv(node);
        break;
    case Op::SFPLOAD:
        gen_sfpload(node);
        break;
    case Op::SFPASSIGNLREG:
        gen_sfpassignlreg(node);
        break;
    case Op::SFPPRESERVELREG:
        gen_sfppreservelreg(node);
        break;
    case Op::SFPLOADI:
        gen_sfploadi(node);
        break;
    case Op::SFPSTORE:
        gen_sfpstore(node);
        break;
    case Op::SFPMOV:
        gen_sfpmov(node);
        break;
    case Op::SFPNOP:
        gen_sfpnop(node);
        break;
    case Op::SFPILLEGAL:
        gen_sfpillegal(node);
        break;
    case Op::SFPENCC:
        gen_sfpencc(node);
        break;
    case Op::SFPPUSHC:
        gen_sfppushc(node);
        break;
    case Op::SFPPOPC:
        gen_sfppopc(node);
        break;
    case Op::SFPSETCC_V:
        gen_sfpsetcc_v(node);
        break;
    case Op::SFPSETCC_I:
        gen_sfpsetcc_i(node);
        break;
    case Op::SFPCOMPC:
        gen_sfpcompc(node);
        break;
    case Op::SFPADD:
        gen_sfpadd(node);
        break;
    case Op::SFPMUL:
        gen_sfpmul(node);
        break;
    case Op::SFPMAD:
        gen_sfpmad(node);
        break;
    case Op::SFPEXEXP:
        gen_sfpexexp(node);
        break;
    case Op::SFPEXMAN:
        gen_sfpexman(node);
        break;
    case Op::SFPSETEXP_I:
        gen_sfpsetexp_i(node);
        break;
    case Op::SFPSETEXP_V:
        gen_sfpsetexp_v(node);
        break;
    case Op::SFPSETMAN_I:
        gen_sfpsetman_i(node);
        break;
    case Op::SFPSETMAN_V:
        gen_sfpsetman_v(node);
        break;
    case Op::SFPABS:
        gen_sfpabs(node);
        break;
    case Op::SFPAND:
        gen_sfpand(node);
        break;
    case Op::SFPOR:
        gen_sfpor(node);
        break;
    case Op::SFPXOR:
        gen_sfpxor(node);
        break;
    case Op::SFPNOT:
        gen_sfpnot(node);
        break;
    case Op::SFPDIVP2:
        gen_sfpdivp2(node);
        break;
    case Op::SFPLZ:
        gen_sfplz(node);
        break;
    case Op::SFPSHFT_I:
        gen_sfpshft_i(node);
        break;
    case Op::SFPSHFT_V:
        gen_sfpshft_v(node);
        break;
    case Op::SFPIADD_I:
        gen_sfpiadd_i(node);
        break;
    case Op::SFPIADD_V:
        gen_sfpiadd_v(node);
        break;
    case Op::SFPSETSGN_I:
        gen_sfpsetsgn_i(node);
        break;
    case Op::SFPSETSGN_V:
        gen_sfpsetsgn_v(node);
        break;
    case Op::SFPLUT:
        gen_sfplut(node);
        break;
    case Op::SFPLUTFP32_3R:
        gen_sfplutfp32_3r(node);
        break;
    case Op::SFPLUTFP32_6R:
        gen_sfplutfp32_6r(node);
        break;
    case Op::SFPCAST:
        gen_sfpcast(node);
        break;
    case Op::SFPSTOCHRND_I:
        gen_sfpstochrnd_i(node);
        break;
    case Op::SFPSTOCHRND_V:
        gen_sfpstochrnd_v(node);
        break;
    case Op::SFPTRANSP:
        gen_sfptransp(node);
        break;
    case Op::SFPSHFT2_I:
        gen_sfpshft2_i(node);
        break;
    case Op::SFPSHFT2_V:
        gen_sfpshft2_v(node);
        break;
    case Op::SFPSHFT2_G:
        gen_sfpshft2_g(node);
        break;
    case Op::SFPSHFT2_GE:
        gen_sfpshft2_ge(node);
        break;
    case Op::SFPSHFT2_E:
        gen_sfpshft2_e(node);
        break;
    case Op::SFPSWAP:
        gen_sfpswap(node);
        break;
    case Op::SFPCONFIG_V:
        gen_sfpconfig_v(node);
        break;
    case Op::SFPDIAG:
        gen_sfpdiag(node);
        break;
    default:
        assert(false);
        break;
    }
}

void Code::gen_sfpincrwc(Node *node) {
    uint32_t cr = node->arg(0);
    uint32_t d = node->arg(1);
    uint32_t b = node->arg(2);
    uint32_t a = node->arg(3);
    m_hw->INCRWC(cr, d, b, a);
}

void Code::gen_sfpassign_lv(Node *node) {
    uint32_t ret = node->ret();
    uint32_t v = node->arg(0);
    uint32_t in = node->arg(1);
    m_hw->SFPMOV(0, lreg_map(in), lreg_map(ret), 0);
}

void Code::gen_sfpload(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mod0 = node->arg(0);
    uint32_t mode = node->arg(1);
    uint32_t addr = node->arg(2);
    m_hw->SFPLOAD(lreg_map(ret), mod0, mode, addr);
}

void Code::gen_sfpassignlreg(Node *node) {
    // nothing to do
}

void Code::gen_sfppreservelreg(Node *node) {
    // nothing to do
}

void Code::gen_sfploadi(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mod0 = node->arg(0);
    uint32_t imm16 = node->arg(1);
    m_hw->SFPLOADI(lreg_map(ret), mod0, imm16);
}

void Code::gen_sfpstore(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t mod0 = node->arg(1);
    uint32_t mode = node->arg(2);
    uint32_t addr = node->arg(3);
    m_hw->SFPSTORE(lreg_map(src), mod0, mode, addr);
}

void Code::gen_sfpmov(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPMOV(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpnop(Node *node) {
    m_hw->SFPNOP();
}

void Code::gen_sfpillegal(Node *node) {
    // nothing to do
}

void Code::gen_sfpencc(Node *node) {
    uint32_t imm12 = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPENCC(imm12, 0, 0, mod1);
}

void Code::gen_sfppushc(Node *node) {
    m_hw->SFPPUSHC(0, 0, 0, 0);
}

void Code::gen_sfppopc(Node *node) {
    m_hw->SFPPOPC(0, 0, 0, 0);
}

void Code::gen_sfpsetcc_v(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPSETCC(0, lreg_map(src), 0, mod1);
}

void Code::gen_sfpsetcc_i(Node *node) {
    uint32_t imm12 = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPSETCC(imm12, 0, 0, mod1);
}

void Code::gen_sfpcompc(Node *node) {
    m_hw->SFPCOMPC(0, 0, 0, 0);
}

void Code::gen_sfpadd(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t mod1 = node->arg(2);
    // lreg[10] = 1
    m_hw->SFPADD(lreg_map(va), 10, lreg_map(vb), lreg_map(ret), mod1);
}

void Code::gen_sfpmul(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t mod1 = node->arg(2);
    // lreg[9] = 0
    m_hw->SFPMUL(lreg_map(va), lreg_map(vb), 9, lreg_map(ret), mod1);
}

void Code::gen_sfpmad(Node *node) {
    uint32_t ret = node->ret();
    uint32_t va = node->arg(0);
    uint32_t vb = node->arg(1);
    uint32_t vc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    m_hw->SFPMAD(lreg_map(va), lreg_map(vb), lreg_map(vc), lreg_map(ret), mod1);
}

void Code::gen_sfpexexp(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPEXEXP(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpexman(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPEXMAN(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpsetexp_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm12 = node->arg(0);
    uint32_t src = node->arg(1);
    m_hw->SFPSETEXP(imm12, lreg_map(src), lreg_map(ret), 1);
}

void Code::gen_sfpsetexp_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPSETEXP(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpsetman_i(Node *node) {
    uint32_t ret = node->ret(); 
    uint32_t imm12 = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    m_hw->SFPSETMAN(imm12, lreg_map(src), lreg_map(ret), 1);
}

void Code::gen_sfpsetman_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPSETMAN(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpabs(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPABS(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpand(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPAND(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpor(Node *node) {
    uint32_t ret = node->ret(); 
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPOR(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpxor(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPXOR(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpnot(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    m_hw->SFPNOT(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpdivp2(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm12 = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    m_hw->SFPDIVP2(imm12, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfplz(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPLZ(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpshft_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t imm12 = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPSHFT(imm12, 0, lreg_map(ret), 1);
}

void Code::gen_sfpshft_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPSHFT(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfpiadd_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t imm12 = node->arg(1);
    uint32_t mod1 = node->arg(2);
    m_hw->SFPIADD(imm12, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpiadd_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPIADD(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpsetsgn_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t imm12 = node->arg(0);
    uint32_t src = node->arg(1);
    m_hw->SFPSETSGN(imm12, lreg_map(src), lreg_map(ret), 1);
}

void Code::gen_sfpsetsgn_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    assert(lreg_map(dst) == lreg_map(ret));
    m_hw->SFPSETSGN(0, lreg_map(src), lreg_map(ret), 0);
}

void Code::gen_sfplut(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t dst = node->arg(3);
    uint32_t mod0 = node->arg(4);
    // ACHTUNG: l0, l1, l2 must be in lreg[0], lreg[1], lreg[2]
    //     SFPI also requires dst to be in lreg[3] (and does not mention l3)
    m_hw->SFPLUT(lreg_map(ret), mod0, 0);
}

void Code::gen_sfplutfp32_3r(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    uint32_t mod0 = node->arg(4);
    // ACHTUNG: l0 ... l3 must be in lreg[0] ... lreg[3]
    //    Clobbers lreg[7]
    // load register number of ret in lreg[7]
    // TODO: Verify this
    m_hw->SFPLOADI(7, 2, lreg_map(ret));
    m_hw->SFPLUTFP32(lreg_map(ret), mod0);
}

void Code::gen_sfplutfp32_6r(Node *node) {
    uint32_t ret = node->ret();
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l4 = node->arg(3);
    uint32_t l5 = node->arg(4);
    uint32_t l6 = node->arg(5);
    uint32_t l3 = node->arg(6);
    uint32_t mod0 = node->arg(7);
    // ACHTUNG: l0 ... l6 must be in lreg[0] ... lreg[6]
    m_hw->SFPLUTFP32(lreg_map(ret), mod0);
}

void Code::gen_sfpcast(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPCAST(lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpstochrnd_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mode = node->arg(0);
    uint32_t imm8 = node->arg(1);
    uint32_t srcc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    m_hw->SFP_STOCH_RND(mode, imm8, 0, lreg_map(srcc), lreg_map(ret), mod1);
}

void Code::gen_sfpstochrnd_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t mode = node->arg(0);
    uint32_t srcb = node->arg(1);
    uint32_t srcc = node->arg(2);
    uint32_t mod1 = node->arg(3);
    m_hw->SFP_STOCH_RND(mode, 0, lreg_map(srcb), lreg_map(srcc), lreg_map(ret), mod1);
}

void Code::gen_sfptransp(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    // ACHTUNG: l0 ... l3 must be in lreg[0] ... lreg[3]
    m_hw->SFPTRANSP(0, 0, 0, 0);
}

void Code::gen_sfpshft2_i(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t imm = node->arg(1);
    // TODO
}

void Code::gen_sfpshft2_v(Node *node) {
    uint32_t ret = node->ret();
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    // TODO
}

void Code::gen_sfpshft2_g(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t l1 = node->arg(1);
    uint32_t l2 = node->arg(2);
    uint32_t l3 = node->arg(3);
    uint32_t mod1 = node->arg(4);
    // ACHTUNG: l0 ... l3 must be in lreg[0] ... lreg[3]
    // TODO
}

void Code::gen_sfpshft2_ge(Node *node) {
    uint32_t src = node->arg(0);
    uint32_t l0 = node->arg(1);
    uint32_t l1 = node->arg(2);
    uint32_t l2 = node->arg(3);
    uint32_t l3 = node->arg(4);
    // ACHTUNG: l0 ... l3 must be in lreg[0] ... lreg[3]
    // TODO
}

void Code::gen_sfpshft2_e(Node *node) {
    uint32_t ret = node->ret();
    uint32_t src = node->arg(0);
    uint32_t mod1 = node->arg(1);
    m_hw->SFPSHFT2(0, lreg_map(src), lreg_map(ret), mod1);
}

void Code::gen_sfpswap(Node *node) {
    uint32_t dst = node->arg(0);
    uint32_t src = node->arg(1);
    uint32_t mod1 = node->arg(2);
    m_hw->SFPSWAP(0, lreg_map(src), lreg_map(dst), mod1);
}

void Code::gen_sfpconfig_v(Node *node) {
    uint32_t l0 = node->arg(0);
    uint32_t config_dest = node->arg(1);
    m_hw->SFPCONFIG(0, config_dest, 0);
}

void Code::gen_sfpdiag(Node *node) {
    if (m_text_pool == nullptr) {
        return;
    }
    uint32_t id = node->arg(0);
    std::string text = m_text_pool->get(id);
    m_hw->SFPDIAG(text);
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

