
#include "sfpi/sfpi_hw.hpp"
#include "sfpi/hw_dump.hpp"

#include "ckernel_thread.h"
#include "sfpi.h"

namespace ckernel {

namespace {

//
//    ThreadHw
//

class ThreadHw: public sfpi::Hw {
public:
    ThreadHw();
    ~ThreadHw();
public:
    void INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) override;
    void SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) override;
    void SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) override;
    void SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) override;
    void SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) override;
    void SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPNOP() override;
    void SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) override;
    void SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) override;
    void SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) override;
    void SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) override;
    void SFPDIAG(const std::string &text) override;
};

ThreadHw::ThreadHw() { }

ThreadHw::~ThreadHw() { }

void ThreadHw::INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) {
    get_thread()->INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a);
}

void ThreadHw::SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    get_thread()->SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void ThreadHw::SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) {
    get_thread()->SFPLOADI(lreg_ind, instr_mod0, imm16);
}

void ThreadHw::SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    get_thread()->SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void ThreadHw::SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) {
    get_thread()->SFPLUT(lreg_ind, instr_mod0, dest_reg_addr);
}

void ThreadHw::SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPMULI(imm16_math, lreg_dest, instr_mod1);
}

void ThreadHw::SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPADDI(imm16_math, lreg_dest, instr_mod1);
}

void ThreadHw::SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFP_STOCH_RND(
        rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPNOP() {
    get_thread()->SFPNOP();
}

void ThreadHw::SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPCAST(lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPCONFIG(imm16_math, config_dest, instr_mod1);
}

void ThreadHw::SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    get_thread()->SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void ThreadHw::SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    get_thread()->SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void ThreadHw::SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) {
    get_thread()->SFPLUTFP32(lreg_dest, instr_mod1);
}

void ThreadHw::SFPDIAG(const std::string &text) {
#if 1 // Enable to print diagnostic messsages at runtime
    printf("SFPDIAG %s\n", text.c_str());
#endif
}

#if 0 // Enable to dump generated SFPU code
sfpi::DumpHw g_thread_hw;
#else
ThreadHw g_thread_hw;
#endif

} // namespace

//
//    Public functions
//

void begin_sfpi() {
    sfpi::rvtt->set_hw(&g_thread_hw);
    sfpi::rvtt->reset();
    sfpi::rvtt->print_text("---- BEGIN_SFPI");
}

void end_sfpi() {
    sfpi::rvtt->print_text("---- END_SFPI");
    sfpi::rvtt->compile();
    sfpi::rvtt->set_hw(nullptr);
}

} // namespace ckernel

