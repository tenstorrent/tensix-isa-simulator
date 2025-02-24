// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cstdarg>

#include "sfpi/hw_dump.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    DumpHw
//

DumpHw::DumpHw() { }

DumpHw::~DumpHw() { }

void DumpHw::INCRWC(
        uint32_t rwc_cr,
        uint32_t rwc_d,
        uint32_t rwc_b,
        uint32_t rwc_a) {
    print_code("INCRWC(%d, %d, %d, %d)", rwc_cr, rwc_d, rwc_b, rwc_a);
}

void DumpHw::SFPLOAD(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    print_code("SFPLOAD(%d, %d, %d, %d)", lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void DumpHw::SFPLOADI(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t imm16) {
    print_code("SFPLOADI(%d, %d, 0x%x)", lreg_ind, instr_mod0, imm16);
}

void DumpHw::SFPSTORE(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    print_code("SFPSTORE(%d, %d, %d, %d)", lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void DumpHw::SFPLUT(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t dest_reg_addr) {
    print_code("SFPLUT(%d, %d, %d)", lreg_ind, instr_mod0, dest_reg_addr);
}

void DumpHw::SFPMULI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPMULI(0x%x, %d, %d)", imm16_math, lreg_dest, instr_mod1);
}

void DumpHw::SFPADDI(
        uint32_t imm16_math,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPADDI(0x%x, %d, %d)", imm16_math, lreg_dest, instr_mod1);
}

void DumpHw::SFPDIVP2(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPDIVP2(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPEXEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPEXEXP(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPEXMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPEXMAN(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPIADD(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPIADD(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPSHFT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSHFT(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPSETCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSETCC(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPMOV(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPMOV(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPABS(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPABS(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPAND(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPAND(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPOR(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPNOT(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPNOT(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPLZ(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPLZ(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPSETEXP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSETEXP(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPSETMAN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSETMAN(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPMAD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code(
        "SFPMAD(%d, %d, %d, %d, %d)", 
            lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPADD(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code(
        "SFPADD(%d, %d, %d, %d, %d)",
            lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPMUL(
        uint32_t lreg_src_a,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code(
        "SFPMUL(%d, %d, %d, %d, %d)",
            lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPPUSHC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPPUSHC(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPPOPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPPOPC(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPSETSGN(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSETSGN(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPENCC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPENCC(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPCOMPC(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPCOMPC(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPTRANSP(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPTRANSP(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPXOR(
        uint32_t imm12_math,
        uint32_t lreg_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPXOR(0x%x, %d, %d, %d)", imm12_math, lreg_c, lreg_dest, instr_mod1);
}

void DumpHw::SFP_STOCH_RND(
        uint32_t rnd_mode,
        uint32_t imm8_math,
        uint32_t lreg_src_b,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code(
        "SFP_STOCH_RND(%d, 0x%x, %d, %d, %d, %d)", 
            rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPNOP() {
    print_code("SFPNOP()");
}

void DumpHw::SFPCAST(
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPCAST(%d, %d, %d)", lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPCONFIG(
        uint32_t imm16_math,
        uint32_t config_dest,
        uint32_t instr_mod1) {
    print_code("SFPCONFIG(0x%x, %d, %d)", imm16_math, config_dest, instr_mod1);
}

void DumpHw::SFPSWAP(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSWAP(0x%x, %d, %d, %d)", imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPLOADMACRO(
        uint32_t lreg_ind,
        uint32_t instr_mod0,
        uint32_t sfpu_addr_mode,
        uint32_t dest_reg_addr) {
    print_code(
        "SFPLOADMACRO(%d, %d, %d, %d)", 
            lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr);
}

void DumpHw::SFPSHFT2(
        uint32_t imm12_math,
        uint32_t lreg_src_c,
        uint32_t lreg_dest,
        uint32_t instr_mod1) {
    print_code("SFPSHFT2(0x%x, %d, %d, %d)", imm12_math, lreg_src_c, lreg_dest, instr_mod1);
}

void DumpHw::SFPLUTFP32(uint32_t lreg_dest, uint32_t instr_mod1) {
    print_code("SFPLUTFP32(%d, %d)", lreg_dest, instr_mod1);
}

void DumpHw::SFPDIAG(const std::string &text) {
    printf("// %s\n", text.c_str());
}

void DumpHw::print_code(const char *fmt, ...) {
    printf("    ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf(";\n");
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

