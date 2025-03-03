// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    OpRegDef
//

class OpRegDef {
public:
    OpRegDef();
    ~OpRegDef();
public:
    bool is_valid(Op op) {
        assert(int(op) < OP_COUNT);
        return m_map[int(op)].is_valid;
    }
    bool has_ret(Op op) {
        assert(int(op) < OP_COUNT);
        return m_map[int(op)].has_ret;
    }
    int arg_count(Op op) {
        assert(int(op) < OP_COUNT);
        return m_map[int(op)].arg_count;
    }
    int arg_pos(Op op, int index) {
        assert(int(op) < OP_COUNT);
        assert(index >= 0 && index < m_map[int(op)].arg_count);
        return m_map[int(op)].arg_pos[index];
    }
private:
    void init();
    void enter(
        Op op, 
        bool has_ret, 
        int arg_count, 
        int arg_pos0 = 0,
        int arg_pos1 = 0,
        int arg_pos2 = 0,
        int arg_pos3 = 0,
        int arg_pos4 = 0,
        int arg_pos5 = 0,
        int arg_pos6 = 0,
        int arg_pos7 = 0);
private:
    struct Entry {
        bool is_valid;
        bool has_ret;
        int arg_count;
        int arg_pos[8];
    };
private:
    static constexpr int OP_COUNT = int(Op::SFPDIAG) + 1;
private:
    Entry m_map[OP_COUNT];
};

OpRegDef::OpRegDef() {
    init();
}

OpRegDef::~OpRegDef() { }

void OpRegDef::init() {
    for (int i = 0; i < OP_COUNT; i++) {
        Entry &entry = m_map[i];
        entry.is_valid = false;
        entry.has_ret = false;
        entry.arg_count = 0;
        for (int k = 0; k < 8; k++) {
            entry.arg_pos[k] = 0;
        }
    }
    enter(Op::SFPINCRWC, false, 0);
    enter(Op::SFPASSIGN_LV, true, 2, 0, 1);
    enter(Op::SFPLOAD, true, 0);
    enter(Op::SFPASSIGNLREG, false, 0);
    enter(Op::SFPPRESERVELREG, false, 0);
//    SFPXLOADI
    enter(Op::SFPSTORE, false, 1, 0);
    enter(Op::SFPMOV, true, 1, 0);
    enter(Op::SFPNOP, false, 0);
    enter(Op::SFPILLEGAL, false, 0);
    enter(Op::SFPENCC, false, 0);
    enter(Op::SFPPUSHC, false, 0);
    enter(Op::SFPPOPC, false, 0);
    enter(Op::SFPSETCC_V, false, 1, 0);
    enter(Op::SFPSETCC_I, false, 0, 0);
//    SFPXFCMPS
//    SFPXFCMPV
//    SFPXICMPS
//    SFPXICMPV
//    SFPXVIF
//    SFPXBOOL
//    SFPXCONDB
//    SFPXCONDI
    enter(Op::SFPCOMPC, false, 0);
    enter(Op::SFPADD, true, 2, 0, 1);
    enter(Op::SFPMUL, true, 2, 0, 1);
    enter(Op::SFPMAD, true, 3, 0, 1, 2);
    enter(Op::SFPEXEXP, true, 1, 0);
    enter(Op::SFPEXMAN, true, 1, 0);
    enter(Op::SFPSETEXP_I, true, 1, 1);
    enter(Op::SFPSETEXP_V, true, 2, 0, 1);
    enter(Op::SFPSETMAN_I, true, 1, 1);
    enter(Op::SFPSETMAN_V, true, 2, 0, 1);
    enter(Op::SFPABS, true, 1, 0);
    enter(Op::SFPAND, true, 2, 0, 1);
    enter(Op::SFPOR, true, 2, 0, 1);
    enter(Op::SFPXOR, true, 2, 0, 1);
    enter(Op::SFPNOT, true, 1, 0);
    enter(Op::SFPDIVP2, true, 1, 1);
    enter(Op::SFPLZ, true, 1, 0);
    enter(Op::SFPSHFT_I, true, 1, 0);
    enter(Op::SFPSHFT_V, true, 2, 0, 1);
//    SFPXIADD_I
//    SFPXIADD_V
    enter(Op::SFPSETSGN_I, true, 1, 1);
    enter(Op::SFPSETSGN_V, true, 2, 0, 1);
    enter(Op::SFPLUT, true, 4, 0, 1, 2, 3);
    enter(Op::SFPLUTFP32_3R, true, 4, 0, 1, 2, 3);
    enter(Op::SFPLUTFP32_6R, true, 7, 0, 1, 2, 3, 4, 5, 6);
    enter(Op::SFPCAST, true, 1, 0);
    enter(Op::SFPSTOCHRND_I, true, 1, 2);
    enter(Op::SFPSTOCHRND_V, true, 2, 1, 2);
    enter(Op::SFPTRANSP, false, 4, 0, 1, 2, 3);
    enter(Op::SFPSHFT2_I, true, 1, 0);
    enter(Op::SFPSHFT2_V, true, 2, 0, 1);
    enter(Op::SFPSHFT2_G, false, 4, 0, 1, 2, 3);
    enter(Op::SFPSHFT2_GE, false, 5, 0, 1, 2, 3, 4);
    enter(Op::SFPSHFT2_E, true, 1, 0);
    enter(Op::SFPSWAP, false, 2, 0, 1);
    enter(Op::SFPCONFIG_V, false, 1, 0);
    enter(Op::SFPLOADI, true, 0);
    enter(Op::SFPIADD_I, true, 1, 0);
    enter(Op::SFPIADD_V, true, 2, 0, 1);
}

void OpRegDef::enter(
        Op op, 
        bool has_ret, 
        int arg_count, 
        int arg_pos0/* = 0*/,
        int arg_pos1/* = 0*/,
        int arg_pos2/* = 0*/,
        int arg_pos3/* = 0*/,
        int arg_pos4/* = 0*/,
        int arg_pos5/* = 0*/,
        int arg_pos6/* = 0*/,
        int arg_pos7/* = 0*/) {
    assert(int(op) < OP_COUNT);
    assert(arg_count >= 0 && arg_count < 8);
    Entry &entry = m_map[int(op)];
    entry.is_valid = true;
    entry.has_ret = has_ret;
    entry.arg_count = arg_count;
    entry.arg_pos[0] = arg_pos0;
    entry.arg_pos[1] = arg_pos1;
    entry.arg_pos[2] = arg_pos2;
    entry.arg_pos[3] = arg_pos3;
    entry.arg_pos[4] = arg_pos4;
    entry.arg_pos[5] = arg_pos5;
    entry.arg_pos[6] = arg_pos6;
    entry.arg_pos[7] = arg_pos7;
}

OpRegDef g_op_reg_def;

//
//    RegPool
//

RegPool::RegPool() {
    reset();
}

RegPool::~RegPool() { }

void RegPool::reset() {
    for (int i = 7; i >= 0; i--) {
        m_free[i] = true;
    }
}

bool RegPool::is_free(uint32_t reg) {
    assert(reg < 8);
    return m_free[reg];
}

uint32_t RegPool::alloc() {
    uint32_t reg = REG_NONE;
    for (int i = 7; i >= 0; i--) {
        if (m_free[i]) {
            reg = uint32_t(i);
            break;
        }
    }
    if (reg == REG_NONE) {
        fatal("Cannot allocate: no free LREGs found");
    }
    m_free[reg] = false;
    return reg;
}

void RegPool::reserve(uint32_t reg) {
    assert(reg < 8);
    if (!m_free[reg]) {
        fatal("Cannot reserve: LREG " + std::to_string(reg) + " is allocated");
    }
    m_free[reg] = false;
}

void RegPool::free(uint32_t reg) {
    assert(reg < 8);
    if (m_free[reg]) {
        fatal("Cannot free: LREG " + std::to_string(reg) + " is not allocated");
    }
    m_free[reg] = true;
}

void RegPool::fatal(const std::string &msg) {
#if 1 // TODO: Revise this (still need regular error handler for coroutines)
    printf("RegPool::fatal: %s\n", msg.c_str());
#endif
    throw std::runtime_error(msg);
}

//
//    RegAlloc
//

RegAlloc::RegAlloc():
        m_graph(nullptr),
        m_reg_map(nullptr),
        m_next_vreg(0) { }

RegAlloc::~RegAlloc() { }

void RegAlloc::run(Graph *graph, RegMap *reg_map) {
    reset(graph, reg_map);
    build_defs();
    graph_alloc();
}

uint32_t RegAlloc::alloc() {
    return m_reg_pool.alloc();
}

void RegAlloc::free(uint32_t reg) {
    m_reg_pool.free(reg);
}

void RegAlloc::reset(Graph *graph, RegMap *reg_map) {
    m_graph = graph;
    m_reg_map = reg_map;
    // m_next_vreg will be set by build_defs()
    m_reg_pool.reset();
}

void RegAlloc::build_defs() {
    uint32_t vreg_count = 0;
    for (Node *node = m_graph->head(); node != nullptr; node = node->next()) {
        uint32_t ret = node->ret();
        if (ret != Code::RET_NONE) {
            vreg_count = std::max(vreg_count, ret + 1);
        }
    }
    m_next_vreg = vreg_count;
    m_defs.resize(vreg_count);
    m_resv.resize(vreg_count);
    m_hints.resize(vreg_count);
    for (uint32_t i = 0; i < vreg_count; i++) {
        m_defs[i] = nullptr;
        m_resv[i] = false;
        m_hints[i] = RegMap::REG_NONE;
    }
    for (Node *node = m_graph->head(); node != nullptr; node = node->next()) {
        uint32_t ret = node->ret();
        if (ret != Code::RET_NONE && m_defs[ret] == nullptr) {
            m_defs[ret] = node;
            if (node->op() == Op::SFPASSIGNLREG) {
                m_resv[ret] = true;
            }
        }
        set_hints(node);
    }
}

void RegAlloc::set_hints(Node *node) {
    Op op = node->op();
    const uint32_t *fixed_lregs = op_has_fixed_lregs(op);
    if (fixed_lregs == nullptr) {
        return;
    }
    int arg_count = g_op_reg_def.arg_count(op);
    for (int i = 0; i < arg_count; i++) {
        uint32_t fixed_lreg = fixed_lregs[i];
        int arg_pos = g_op_reg_def.arg_pos(op, i);
        uint32_t arg = node->arg(arg_pos);
        m_hints[arg] = fixed_lreg;
    }
}

void RegAlloc::graph_alloc() {
    Node *node = m_graph->tail();
    while (node != nullptr) {
        Node *prev = node->prev();
        node_alloc(node);
        node = prev;
    }
}

void RegAlloc::node_alloc(Node *node) {
    Op op = node->op();
    if (op == Op::SFPDIAG) {
        return;
    }
    if (!g_op_reg_def.is_valid(op)) {
        fatal("Missing op reg definition for " + fmt_node(node));
    }
    uint32_t ret = node->ret();
    if (ret != Code::RET_NONE) {
        uint32_t ret_lreg = m_reg_map->map(ret);
        if (ret_lreg == RegMap::REG_NONE) {
            // this can happen, e.g., when SFPIADD_I is used solely for setting CC
            ret_lreg = m_reg_pool.alloc();
            m_reg_map->bind(ret, ret_lreg);
        }
        if (m_defs[ret] == node && ret_lreg < 8) {
            // sfpassignlreg can have ret >= 8
            m_reg_pool.free(ret_lreg);
        }
    }
    if (alloc_one_addr(node)) {
        return;
    }
    if (alloc_two_addr(node)) {
        return;
    }
    if (alloc_fixed_lregs(node)) {
        return;
    }
    int arg_count = g_op_reg_def.arg_count(op);
    for (int i = 0; i < arg_count; i++) {
        int arg_pos = g_op_reg_def.arg_pos(op, i);
        uint32_t arg = node->arg(arg_pos);
        uint32_t lreg = m_reg_map->map(arg);
        if (lreg == RegMap::REG_NONE) {
            lreg = lreg_alloc(arg);
            m_reg_map->bind(arg, lreg);
        } else {
            reserve_assigned_lreg(arg, lreg);
        }
    }
}

bool RegAlloc::alloc_one_addr(Node *node) {
    Op op = node->op();
    if (!is_one_addr_op(op)) {
        return false;
    }

    assert(g_op_reg_def.arg_count(op) == 1);
    uint32_t ret = node->ret();
    uint32_t ret_lreg = m_reg_map->map(ret);
    assert(ret_lreg != RegMap::REG_NONE);

    uint32_t dst_pos = g_op_reg_def.arg_pos(op, 0);
    uint32_t dst = node->arg(dst_pos);
    uint32_t dst_lreg = m_reg_map->map(dst);
    if (dst_lreg == RegMap::REG_NONE) {
        dst_lreg = lreg_alloc(dst);
        m_reg_map->bind(dst, dst_lreg);
    } else {
        reserve_assigned_lreg(dst, dst_lreg);
    }

    if (dst_lreg != ret_lreg) {
        // move dst to ret
        uint32_t temp = vreg_alloc();
        m_reg_map->bind(temp, ret_lreg);
        insert_move(node, temp, dst);
        node->set_arg(dst_pos, temp);
    }

    return true;
}

bool RegAlloc::alloc_two_addr(Node *node) {
    Op op = node->op();
    if (!is_two_addr_op(op)) {
        return false;
    }

    assert(g_op_reg_def.arg_count(op) == 2);
    uint32_t ret = node->ret();
    uint32_t ret_lreg = m_reg_map->map(ret);
    assert(ret_lreg != RegMap::REG_NONE);

    uint32_t dst_pos = g_op_reg_def.arg_pos(op, 0);
    uint32_t dst = node->arg(dst_pos);
    uint32_t dst_lreg = m_reg_map->map(dst);
    if (dst_lreg == RegMap::REG_NONE) {
        dst_lreg = lreg_alloc(dst);
        m_reg_map->bind(dst, dst_lreg);
    } else {
        reserve_assigned_lreg(dst, dst_lreg);
    }

    uint32_t src_pos = g_op_reg_def.arg_pos(op, 1);
    uint32_t src = node->arg(src_pos);
    uint32_t src_lreg = m_reg_map->map(src);
    if (src_lreg == RegMap::REG_NONE) {
        src_lreg = lreg_alloc(src);
        m_reg_map->bind(src, src_lreg);
    } else {
        reserve_assigned_lreg(src, src_lreg);
    }

    if (dst_lreg != ret_lreg) {
        if (src_lreg == ret_lreg) {
            if (is_comm_op(node)) {
                node->set_arg(dst_pos, src);
                node->set_arg(src_pos, dst);
                return true;
            }
            // move src to temp register
            uint32_t temp = vreg_alloc();
            uint32_t temp_lreg = m_reg_pool.alloc();
            m_reg_map->bind(temp, temp_lreg);
            insert_move(node, temp, src);
            node->set_arg(src_pos, temp);
            m_reg_pool.free(temp_lreg);
        }
        // move dst to ret
        uint32_t temp = vreg_alloc();
        m_reg_map->bind(temp, ret_lreg);
        insert_move(node, temp, dst);
        node->set_arg(dst_pos, temp);
    }

    return true;
}

bool RegAlloc::alloc_fixed_lregs(Node *node) {
    Op op = node->op();
    const uint32_t *fixed_lregs = op_has_fixed_lregs(op);
    if (fixed_lregs == nullptr) {
        return false;
    }
    uint32_t rename[8];
    bool spill[8];
    bool free[8];
    uint32_t move_src[8];
    uint32_t move_dst[8];
    uint32_t move_top = 0;
    for (int i = 0; i < 8; i++) {
        rename[i] = i;
        spill[i] = false;
        free[i] = false;
        move_src[i] = 0;
        move_dst[i] = 0;
    }
    int arg_count = g_op_reg_def.arg_count(op);
    for (int i = 0; i < arg_count; i++) {
        uint32_t fixed_lreg = fixed_lregs[i];
        int arg_pos = g_op_reg_def.arg_pos(op, i);
        uint32_t arg = node->arg(arg_pos);
        uint32_t lreg = m_reg_map->map(arg);
        if (lreg == RegMap::REG_NONE) {
            if (m_reg_pool.is_free(fixed_lreg)) {
                m_reg_pool.reserve(fixed_lreg);
                lreg = fixed_lreg;
            } else {
                lreg = m_reg_pool.alloc();
            }
            m_reg_map->bind(arg, fixed_lreg);
        } else {
            reserve_assigned_lreg(arg, lreg);
        }
        if (lreg != fixed_lreg) {
            if (rename[lreg] != lreg) {
                // use rename target instead of original LREG for moves
                lreg = rename[lreg];
                arg = vreg_alloc();
                m_reg_map->bind(arg, lreg);
            }
            if (m_reg_pool.is_free(fixed_lreg)) {
                m_reg_pool.reserve(fixed_lreg);
                uint32_t temp = vreg_alloc();
                m_reg_map->bind(temp, fixed_lreg);
                insert_move(node, temp, arg);
                move_src[move_top] = arg;
                move_dst[move_top] = temp;
                move_top++;
                node->set_arg(arg_pos, temp);
                free[fixed_lreg] = true;
                spill[lreg] = true;
            } else if (spill[fixed_lreg]) {
                uint32_t temp = vreg_alloc();
                m_reg_map->bind(temp, fixed_lreg);
                insert_move(node, temp, arg);
                move_src[move_top] = arg;
                move_dst[move_top] = temp;
                move_top++;
                node->set_arg(arg_pos, temp);
                spill[lreg] = true;
            } else {
                uint32_t temp = vreg_alloc();
                m_reg_map->bind(temp, fixed_lreg);
                uint32_t save_lreg = m_reg_pool.alloc();
                uint32_t save = vreg_alloc();
                m_reg_map->bind(save, save_lreg);
                insert_move(node, save, temp);
                move_src[move_top] = temp;
                move_dst[move_top] = save;
                move_top++;
                insert_move(node, temp, arg);
                move_src[move_top] = arg;
                move_dst[move_top] = temp;
                move_top++;
                node->set_arg(arg_pos, temp);
                rename[fixed_lreg] = save_lreg;
                spill[lreg] = true;
                free[save_lreg] = true;
            }
        }
    }
    for (uint32_t i = 0; i < move_top; i++) {
        insert_move(node->next(), move_src[i], move_dst[i]);
    }
    for (uint32_t i = 0; i < 8; i++) {
        if (free[i]) {
            m_reg_pool.free(i);
        }
    }
    return true;
}

void RegAlloc::reserve_assigned_lreg(uint32_t vreg, uint32_t lreg) {
    if (!m_resv[vreg]) {
        return;
    }
    if (lreg < 8) { 
        m_reg_pool.reserve(lreg);
    }
    m_resv[vreg] = false;
}

void RegAlloc::insert_move(Node *node, uint32_t ret, uint32_t src) {
    // 2 = unconditional upon CC: is this correct?
    Node *move = m_graph->new_node(Op::SFPMOV, ret, src, 2);
    m_graph->insert(move, node);
}

uint32_t RegAlloc::vreg_alloc() {
    uint32_t vreg = m_next_vreg;
    m_next_vreg++;
    m_defs.resize(m_next_vreg, nullptr);
    m_resv.resize(m_next_vreg, false);
    m_hints.resize(m_next_vreg, RegMap::REG_NONE);
    return vreg;
}

uint32_t RegAlloc::lreg_alloc(uint32_t vreg) {
    uint32_t hint_lreg = m_hints[vreg];
    if (hint_lreg != RegMap::REG_NONE && m_reg_pool.is_free(hint_lreg)) {
        m_reg_pool.reserve(hint_lreg);
        return hint_lreg;
    }
    return m_reg_pool.alloc();
}

const uint32_t *RegAlloc::op_has_fixed_lregs(Op op) {
    static const uint32_t sfplut_lregs[] = { 0, 1, 2, 3 };
    static const uint32_t sfplutfp32_3r_lregs[] = { 0, 1, 2, 3 };
    static const uint32_t sfplutfp32_6r_lregs[] = { 0, 1, 2, 4, 5, 6, 3 };
    static const uint32_t sfptransp_lregs[] = { 0, 1, 2, 3 };
    static const uint32_t sfpconfig_v_lregs[] = { 0 };
    switch (op) {
    case Op::SFPLUT:
        return sfplut_lregs;
    case Op::SFPLUTFP32_3R:
        return sfplutfp32_3r_lregs;
    case Op::SFPLUTFP32_6R:
        return sfplutfp32_6r_lregs;
    case Op::SFPTRANSP:
        return sfptransp_lregs;
    case Op::SFPCONFIG_V:
        return sfpconfig_v_lregs;
    default:
        break;
    }
    return nullptr;
}

bool RegAlloc::is_one_addr_op(Op op) {
    return (op == Op::SFPSHFT_I);
}

bool RegAlloc::is_two_addr_op(Op op) {
    switch (op) {
    case Op::SFPSETEXP_V:
    case Op::SFPSETMAN_V:
    case Op::SFPAND:
    case Op::SFPOR:
    case Op::SFPXOR:
    case Op::SFPSHFT_V:
    case Op::SFPIADD_V:
    case Op::SFPSETSGN_V:
    case Op::SFPSHFT2_V:
        return true;
    }
    return false;
}

bool RegAlloc::is_comm_op(Node *node) {
    // relevant for 2-addr ops only
    Op op = node->op();
    switch (op) {
    case Op::SFPAND:
    case Op::SFPOR:
    case Op::SFPXOR:
        return true;
    case Op::SFPIADD_V:
        {
            uint32_t mod1 = node->arg(2);
            return (mod1 == 0 || mod1 == 4 || mod1 == 8 || mod1 == 12);
        }
    }
    return false;
}

void RegAlloc::diag_free_lregs(const char *tag) {
    printf("%s: FREE LREGS:", tag);
    for (uint32_t i = 0; i < 8; i++) {
        if (m_reg_pool.is_free(i)) {
            printf(" %d", i);
        }
    }
    printf("\n");
}

void RegAlloc::warn(const std::string &msg) {
    printf("WARNING: %s\n", msg.c_str());
}

void RegAlloc::fatal(const std::string &msg) {
    throw std::runtime_error(msg);
}

//
//    Code
//

void Code::schedule() {
    Node *node = m_graph.head();
    while (node != nullptr) {
        Node *next = node->next();
        if (next != nullptr) {
            switch (node->op()) {
            case Op::SFPLUT:
#if 0 // currently unused - reserved
            case Op::SFPMULI:
            case Op::SFPADDI:
#endif
            case Op::SFPMAD:
            case Op::SFPADD:
            case Op::SFPMUL:
            case Op::SFPSWAP:
            case Op::SFPSHFT2_E:
            case Op::SFPLUTFP32_3R:
            case Op::SFPLUTFP32_6R:
                schedule_node(node);
                break;
            }
        }
        node = next;
    }
}

void Code::schedule_node(Node *node) {
    Op op = node->op();
    Node *next = node->next();
    assert(next != nullptr);
    bool must_schedule = false;
    if (op == Op::SFPSWAP) {
#if 0 // TODO: Revise this
        uint32_t arg0 = node->arg(0);
        uint32_t arg1 = node->arg(1);
        uint32_t arg0_lreg = m_lreg_map.map(arg0);
        uint32_t arg1_lreg = m_lreg_map.map(arg1);
        must_schedule = (is_input_lreg(next, arg0_lreg) || is_input_lreg(next, arg1_lreg));
#endif
        // SFPU spec says always add SFPNOP
        must_schedule = true;
    } else if (op == Op::SFPSHFT2_E) {
        // SFPU spec says always add SFPNOP for global modes
        uint32_t mod1 = node->arg(1);
        must_schedule = (mod1 == 2 || mod1 == 3 || mod1 == 4);
    } else {
        uint32_t ret = node->ret();
        assert(ret != RET_NONE);
        uint32_t ret_lreg = m_lreg_map.map(ret);
        must_schedule = is_input_lreg(next, ret_lreg);
    }
    if (must_schedule) {
        Node *nop = m_graph.new_node(Op::SFPNOP, RET_NONE);
        m_graph.insert(nop, next);
    }
}

bool Code::is_input_lreg(Node *node, uint32_t lreg) {
    Op op = node->op();
    int arg_count = g_op_reg_def.arg_count(op);
    for (int i = 0; i < arg_count; i++) {
        int arg_pos = g_op_reg_def.arg_pos(op, i);
        uint32_t arg = node->arg(arg_pos);
        uint32_t arg_lreg = m_lreg_map.map(arg);
        if (arg_lreg == lreg) {
            return true;
        }
    }
    return false;
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

