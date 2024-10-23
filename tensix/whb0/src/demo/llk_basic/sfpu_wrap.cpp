
#include "core/thread.hpp"

#include "demo/llk_basic/sfpu_test.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using core::Thread;

//
//    SfpuWrap
//

SfpuWrap::SfpuWrap(Thread *thread):
        m_thread(thread) { }

SfpuWrap::~SfpuWrap() { }

// thread delegates

void SfpuWrap::SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_thread->SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void SfpuWrap::SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) {
    m_thread->SFPLOADI(lreg_ind, instr_mod0, imm16);
}

void SfpuWrap::SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_thread->SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void SfpuWrap::SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) {
    m_thread->SFPLUT(lreg_ind, instr_mod0, dest_reg_addr);
}

void SfpuWrap::SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPMULI(imm16_math, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPADDI(imm16_math, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPNOP() {
    m_thread->SFPNOP();
}

void SfpuWrap::SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPCAST(lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) {
    m_thread->SFPCONFIG(imm16_math, config_dest, instr_mod1);
}

void SfpuWrap::SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    m_thread->SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void SfpuWrap::SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    m_thread->SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void SfpuWrap::SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) {
    m_thread->SFPLUTFP32(lreg_dest, instr_mod1);
}

void SfpuWrap::dump_lreg_int(const char *label, uint32_t lreg) {
    m_thread->diag_dump_lreg_int(label, lreg);
}

void SfpuWrap::dump_lreg_float(const char *label, uint32_t lreg) {
    m_thread->diag_dump_lreg_float(label, lreg);
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

