#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "sfpi/sfpi_hw.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Constants
//

constexpr uint32_t 
    SFPPUSHCC_MOD1_PUSH = 0,
    SFPPUSHCC_MOD1_REPLACE = 1;

constexpr uint32_t SFPPOPCC_MOD1_POP = 0; 

constexpr uint32_t SFPXIADD_MOD1_DST_UNUSED = 128; 

//
//    Op
//

enum class Op {
    SFPINCRWC,
    SFPASSIGN_LV,
    SFPLOAD,
    SFPASSIGNLREG,
    SFPPRESERVELREG,
    SFPXLOADI,
    SFPSTORE,
    SFPMOV,
    SFPNOP,
    SFPILLEGAL,
    SFPENCC,
    SFPPUSHC,
    SFPPOPC,
    SFPSETCC_V,
    SFPSETCC_I,
    SFPXFCMPS,
    SFPXFCMPV,
    SFPXICMPS,
    SFPXICMPV,
    SFPXVIF,
    SFPXBOOL,
    SFPXCONDB,
    SFPXCONDI,
    SFPCOMPC,
    SFPADD,
    SFPMUL,
    SFPMAD,
    SFPEXEXP,
    SFPEXMAN,
    SFPSETEXP_I,
    SFPSETEXP_V,
    SFPSETMAN_I,
    SFPSETMAN_V,
    SFPABS,
    SFPAND,
    SFPOR,
    SFPXOR,
    SFPNOT,
    SFPDIVP2,
    SFPLZ,
    SFPSHFT_I,
    SFPSHFT_V,
    SFPXIADD_I,
    SFPXIADD_V,
    SFPSETSGN_I,
    SFPSETSGN_V,
    SFPLUT,
    SFPLUTFP32_3R,
    SFPLUTFP32_6R,
    SFPCAST,
    SFPSTOCHRND_I,
    SFPSTOCHRND_V,
    SFPTRANSP,
    SFPSHFT2_I,
    SFPSHFT2_V,
    SFPSHFT2_G,
    SFPSHFT2_GE,
    SFPSHFT2_E,
    SFPSWAP,
    SFPCONFIG_V,
    // emitted only
    SFPLOADI,
    SFPIADD_I,
    SFPIADD_V,
    // pseudo-instructions
    SFPDIAG
};

//
//    Node
//

class Node {
    friend class Graph;
public:
    Node();
    ~Node();
public:
    Node *prev() {
        return m_prev;
    }
    Node *next() {
        return m_next;
    }
    Op op() {
        return m_op;
    }
    uint32_t ret() {
        return m_ret;
    }
    uint32_t arg(int index) {
        assert(index >= 0 && index < 8);
        return m_args[index];
    }
    void set_arg(int index, uint32_t value) {
        assert(index >= 0 && index < 8);
        m_args[index] = value;
    }
private:
    Node *m_prev;
    Node *m_next;
    Op m_op;
    uint32_t m_ret;
    uint32_t m_args[8];
};

//
//    Graph
//

class Graph {
public:
    Graph();
    ~Graph();
public:
    void reset();
    Node *head() {
        return m_head;
    }
    Node *tail() {
        return m_tail;
    }
    Node *new_node(
        Op op,
        uint32_t ret,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0);
    void push(Node *node);
    void insert(Node *node, Node *next);
    void remove(Node *node);
private:
    std::vector<std::unique_ptr<Node>> m_nodes;
    int m_top;
    Node *m_head;
    Node *m_tail;
};

//
//    Node formatting
//

std::string fmt_print(const char *fmt, ...);
std::string fmt_node(Node *node);

//
//    TextPool
//

class TextPool {
public:
    TextPool();
    ~TextPool();
public:
    void reset();
    uint32_t push(const std::string &text);
    std::string get(uint32_t id);
private:
    std::vector<std::string> m_pool;
};

//
//    Ir
//

class Ir {
public:
    Ir();
    ~Ir();
public:
    void set_text_pool(TextPool *text_pool);
    void reset();
    Node *head() {
        return m_graph.head();
    }
    Node *tail() {
        return m_graph.tail();
    }
    Node *new_node(
        Op op,
        uint32_t ret,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0);
    void push(Node *node) {
        m_graph.push(node);
    }
    void insert(Node *node, Node *next) {
        m_graph.insert(node, next);
    }
    void remove(Node *node) {
        m_graph.remove(node);
    }
    Node *map_node(uint32_t id) {
        assert(id < m_node_map.size());
        return m_node_map[id];
    }
    void dump_graph();
public:
    static constexpr uint32_t
        RET_NONE = ~uint32_t(0),
        RET_AUTO = ~uint32_t(1);
private:
    Graph m_graph;
    std::vector<Node *> m_node_map;
    TextPool *m_text_pool;
};

//
//    IrPass
//

class IrPass {
public:
    IrPass() { }
    virtual ~IrPass() { }
public:
    virtual void transform(Ir *ir) = 0;
};

//
//    RegMap
//

class RegMap {
public:
    RegMap();
    ~RegMap();
public:
    void reset();
    uint32_t size();
    void bind(uint32_t id, uint32_t reg);
    uint32_t map(uint32_t id);
public:
    static constexpr uint32_t REG_NONE = ~uint32_t(0);
private:
    std::vector<uint32_t> m_map;
    uint32_t m_top;
};

//
//    RegPool
//

class RegPool {
public:
    RegPool();
    ~RegPool();
public:
    void reset();
    bool is_free(uint32_t reg);
    uint32_t alloc();
    void reserve(uint32_t reg);
    void free(uint32_t reg);
private:
    static void fatal(const std::string &msg);
private:
    static constexpr uint32_t REG_NONE = ~uint32_t(0);
private:
    bool m_free[8];
};

//
//    RegAlloc
//

class RegAlloc {
public:
    RegAlloc();
    ~RegAlloc();
public:
    void run(Graph *graph, RegMap *reg_map);
    uint32_t alloc();
    void free(uint32_t reg);
private:
    void reset(Graph *graph, RegMap *reg_map);
    void build_defs();
    void set_hints(Node *node);
    void graph_alloc();
    void node_alloc(Node *node);
    bool alloc_one_addr(Node *node);
    bool alloc_two_addr(Node *node);
    bool alloc_fixed_lregs(Node *node);
    void reserve_assigned_lreg(uint32_t vreg, uint32_t lreg);
    void insert_move(Node *node, uint32_t ret, uint32_t src);
    uint32_t vreg_alloc();
    uint32_t lreg_alloc(uint32_t vreg);
    static const uint32_t *op_has_fixed_lregs(Op op);
    static bool is_one_addr_op(Op op);
    static bool is_two_addr_op(Op op);
    static bool is_comm_op(Node *node);
    void diag_free_lregs(const char *tag);
    static void warn(const std::string &msg);
    static void fatal(const std::string &msg);
private:
    Graph *m_graph;
    RegMap *m_reg_map;
    uint32_t m_next_vreg;
    RegPool m_reg_pool;
    std::vector<Node *> m_defs;
    std::vector<bool> m_resv;
    std::vector<uint32_t> m_hints;
};

//
//    Code
//

class Code {
public:
    Code();
    ~Code();
public:
    void set_text_pool(TextPool *text_pool);
    void set_hw(Hw *hw);
    void reset();
    void emit(Node *node);
    void alloc_lregs();
    void schedule();
    void gen();
    void dump_graph();
private:
    void emit_sfpincrwc(Node *node);
    void emit_sfpassign_lv(Node *node);
    void emit_sfpload(Node *node);
    void emit_sfpassignlreg(Node *node);
    void emit_sfppreservelreg(Node *node);
    void emit_sfpxloadi(Node *node);
    void emit_sfpstore(Node *node);
    void emit_sfpmov(Node *node);
    void emit_sfpnop(Node *node);
    void emit_sfpillegal(Node *node);
    void emit_sfpencc(Node *node);
    void emit_sfppushc(Node *node);
    void emit_sfppopc(Node *node);
    void emit_sfpsetcc_v(Node *node);
    void emit_sfpsetcc_i(Node *node);
    void emit_sfpxfcmps(Node *node);
    void emit_sfpxfcmpv(Node *node);
    void emit_sfpxicmps(Node *node);
    void emit_sfpxicmpv(Node *node);
    void emit_sfpxvif(Node *node);
    void emit_sfpxbool(Node *node);
    void emit_sfpxcondb(Node *node);
    void emit_sfpxcondi(Node *node);
    void emit_sfpcompc(Node *node);
    void emit_sfpadd(Node *node);
    void emit_sfpmul(Node *node);
    void emit_sfpmad(Node *node);
    void emit_sfpexexp(Node *node);
    void emit_sfpexman(Node *node);
    void emit_sfpsetexp_i(Node *node);
    void emit_sfpsetexp_v(Node *node);
    void emit_sfpsetman_i(Node *node);
    void emit_sfpsetman_v(Node *node);
    void emit_sfpabs(Node *node);
    void emit_sfpand(Node *node);
    void emit_sfpor(Node *node);
    void emit_sfpxor(Node *node);
    void emit_sfpnot(Node *node);
    void emit_sfpdivp2(Node *node);
    void emit_sfplz(Node *node);
    void emit_sfpshft_i(Node *node);
    void emit_sfpshft_v(Node *node);
    void emit_sfpxiadd_i(Node *node);
    void emit_sfpxiadd_v(Node *node);
    void emit_sfpsetsgn_i(Node *node);
    void emit_sfpsetsgn_v(Node *node);
    void emit_sfplut(Node *node);
    void emit_sfplutfp32_3r(Node *node);
    void emit_sfplutfp32_6r(Node *node);
    void emit_sfpcast(Node *node);
    void emit_sfpstochrnd_i(Node *node);
    void emit_sfpstochrnd_v(Node *node);
    void emit_sfptransp(Node *node);
    void emit_sfpshft2_i(Node *node);
    void emit_sfpshft2_v(Node *node);
    void emit_sfpshft2_g(Node *node);
    void emit_sfpshft2_ge(Node *node);
    void emit_sfpshft2_e(Node *node);
    void emit_sfpswap(Node *node);
    void emit_sfpconfig_v(Node *node);
    void emit_sfpdiag(Node *node);
private:
    void expand_sfpassignlreg(uint32_t ret, uint32_t lr);
    void expand_sfpload(uint32_t ret, uint32_t mod, uint32_t mode, uint32_t addr);
    void expand_sfpxloadi(uint32_t ret, uint32_t mod, uint32_t imm);
    void expand_sfpstore(uint32_t src, uint32_t mod, uint32_t mode, uint32_t addr);
    // SKIPPED: expand_sfpmuli/expand_sfpaddi - produced by combine optimizetion pass
    void expand_sfpshft_i(uint32_t ret, uint32_t dst, uint32_t imm);
    void expand_sfpxiadd_i(uint32_t ret, uint32_t src, uint32_t imm, uint32_t mod);
    void expand_sfpxiadd_v(uint32_t ret, uint32_t dst, uint32_t src, uint32_t mod);
    void expand_sfpxfcmps(uint32_t v, uint32_t f, uint32_t mod);
    void expand_sfpxfcmpv(uint32_t v1, uint32_t v2, uint32_t mod);
    void expand_sfpdivp2(uint32_t ret, uint32_t imm, uint32_t src, uint32_t mod);
    void expand_sfpstochrnd_i(
        uint32_t ret, 
        uint32_t mode, 
        uint32_t imm, 
        uint32_t src, 
        uint32_t mod);
    void expand_sfpsetexp_i(uint32_t ret, uint32_t imm, uint32_t src);
    void expand_sfpsetsgn_i(uint32_t ret, uint32_t imm, uint32_t src);
    void expand_sfpsetman(uint32_t ret, uint32_t imm, uint32_t src);
    void expand_sfpshft2_e(uint32_t ret, uint32_t src, uint32_t mod);
private:
    void schedule_node(Node *node);
    bool is_input_lreg(Node *node, uint32_t lreg);
private:
    void gen_node(Node *node);
    void gen_sfpincrwc(Node *node);
    void gen_sfpassign_lv(Node *node);
    void gen_sfpload(Node *node);
    void gen_sfpassignlreg(Node *node);
    void gen_sfppreservelreg(Node *node);
    void gen_sfploadi(Node *node);
    void gen_sfpstore(Node *node);
    void gen_sfpmov(Node *node);
    void gen_sfpnop(Node *node);
    void gen_sfpillegal(Node *node);
    void gen_sfpencc(Node *node);
    void gen_sfppushc(Node *node);
    void gen_sfppopc(Node *node);
    void gen_sfpsetcc_v(Node *node);
    void gen_sfpsetcc_i(Node *node);
    void gen_sfpcompc(Node *node);
    void gen_sfpadd(Node *node);
    void gen_sfpmul(Node *node);
    void gen_sfpmad(Node *node);
    void gen_sfpexexp(Node *node);
    void gen_sfpexman(Node *node);
    void gen_sfpsetexp_i(Node *node);
    void gen_sfpsetexp_v(Node *node);
    void gen_sfpsetman_i(Node *node);
    void gen_sfpsetman_v(Node *node);
    void gen_sfpabs(Node *node);
    void gen_sfpand(Node *node);
    void gen_sfpor(Node *node);
    void gen_sfpxor(Node *node);
    void gen_sfpnot(Node *node);
    void gen_sfpdivp2(Node *node);
    void gen_sfplz(Node *node);
    void gen_sfpshft_i(Node *node);
    void gen_sfpshft_v(Node *node);
    void gen_sfpiadd_i(Node *node);
    void gen_sfpiadd_v(Node *node);
    void gen_sfpsetsgn_i(Node *node);
    void gen_sfpsetsgn_v(Node *node);
    void gen_sfplut(Node *node);
    void gen_sfplutfp32_3r(Node *node);
    void gen_sfplutfp32_6r(Node *node);
    void gen_sfpcast(Node *node);
    void gen_sfpstochrnd_i(Node *node);
    void gen_sfpstochrnd_v(Node *node);
    void gen_sfptransp(Node *node);
    void gen_sfpshft2_i(Node *node);
    void gen_sfpshft2_v(Node *node);
    void gen_sfpshft2_g(Node *node);
    void gen_sfpshft2_ge(Node *node);
    void gen_sfpshft2_e(Node *node);
    void gen_sfpswap(Node *node);
    void gen_sfpconfig_v(Node *node);
    void gen_sfpdiag(Node *node);
private:
    static uint32_t clamp_signed(uint32_t v, uint32_t mask);
    static uint32_t clamp_unsigned(uint32_t v, uint32_t mask);
    static uint32_t fp32_to_fp16a(uint32_t val);
    static uint32_t fp32_to_fp16b(uint32_t val);
    static uint32_t scmp2loadi_mod(uint32_t mod);
private:
    void push_node(
        Op op,
        uint32_t ret,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0);
    uint32_t vreg_map_arg(Node *node, int index);
    uint32_t vreg_bind_ret(Node *node);
    uint32_t vreg_alloc();
    void vreg_bind(uint32_t id, uint32_t vreg);
    void lreg_bind(uint32_t vreg, uint32_t lreg);
    uint32_t lreg_map(uint32_t vreg);
public:
    static constexpr uint32_t RET_NONE = ~uint32_t(0);
private:
    Hw *m_hw;
    Graph m_graph;
    uint32_t m_next_vreg;
    RegMap m_vreg_map;
    RegMap m_lreg_map;
    RegAlloc m_reg_alloc;
    TextPool *m_text_pool;
};

//
//    RvttImpl
//

class RvttImpl: public Rvtt {
public:
    RvttImpl();
    ~RvttImpl();
public:
    void set_hw(Hw *hw) override;
    void reset() override;
    void compile() override;
public:
    void sfpincrwc(uint32_t cr, uint32_t d, uint32_t b, uint32_t a) override;
    Vec sfpassign_lv(Vec v, Vec in) override;
    Vec sfpload(uint32_t mod0, uint32_t mode, uint32_t addr) override;
    Vec sfpassignlreg(int lr) override;
    void sfppreservelreg(Vec x, int n) override;
    Vec sfpxloadi(uint32_t mod0, uint32_t imm) override;
    void sfpstore(Vec src, uint32_t mod0, uint32_t mode, uint32_t addr) override;
    Vec sfpmov(Vec src, uint32_t mod1) override;
    void sfpnop() override;
    void sfpillegal() override;
    void sfpencc(uint32_t imm, uint32_t mod1) override;
    void sfppushc() override;
    void sfppopc() override;
    void sfpsetcc_v(Vec src, uint32_t mod1) override;
    void sfpsetcc_i(uint32_t imm, uint32_t mod1) override;
    int sfpxfcmps(Vec v, uint32_t f, uint32_t mod1) override;
    int sfpxfcmpv(Vec v1, Vec v2, uint32_t mod1) override;
    int sfpxicmps(Vec v, uint32_t i, uint32_t mod1) override;
    int sfpxicmpv(Vec v1, Vec v2, uint32_t mod1) override;
    int sfpxvif() override;
    int sfpxbool(int t, int a, int b) override;
    void sfpxcondb(int s, int i) override;
    Vec sfpxcondi(int i) override;
    void sfpcompc() override;
    Vec sfpadd(Vec va, Vec vb, uint32_t mod1) override;
    Vec sfpmul(Vec va, Vec vb, uint32_t mod1) override;
    Vec sfpmad(Vec va, Vec vb, Vec vc, uint32_t mod1) override;
    Vec sfpexexp(Vec src, uint32_t mod1) override;
    Vec sfpexman(Vec src, uint32_t mod1) override;
    Vec sfpsetexp_i(uint32_t imm, Vec src) override;
    Vec sfpsetexp_v(Vec dst, Vec src) override;
    Vec sfpsetman_i(uint32_t imm, Vec src, uint32_t mod) override;
    Vec sfpsetman_v(Vec dst, Vec src) override;
    Vec sfpabs(Vec src, uint32_t mod1) override;
    Vec sfpand(Vec dst, Vec src) override;
    Vec sfpor(Vec dst, Vec src) override;
    Vec sfpxor(Vec dst, Vec src) override;
    Vec sfpnot(Vec src) override;
    Vec sfpdivp2(uint32_t imm, Vec src, uint32_t mod1) override;
    Vec sfplz(Vec src, uint32_t mod1) override;
    Vec sfpshft_i(Vec dst, uint32_t imm) override;
    Vec sfpshft_v(Vec dst, Vec src) override;
    Vec sfpxiadd_i(Vec src, uint32_t imm, uint32_t mod1) override;
    Vec sfpxiadd_v(Vec dst, Vec src, uint32_t mod1) override;
    Vec sfpsetsgn_i(uint32_t imm, Vec src) override;
    Vec sfpsetsgn_v(Vec dst, Vec src) override;
    Vec sfplut(Vec l0, Vec l1, Vec l2, Vec dst, uint32_t mod0) override;
    Vec sfplutfp32_3r(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod0) override;
    Vec sfplutfp32_6r(
        Vec l0, 
        Vec l1, 
        Vec l2, 
        Vec l4, 
        Vec l5, 
        Vec l6, 
        Vec l3, 
        uint32_t mod0) override;
    Vec sfpcast(Vec src, uint32_t mod1) override;
    Vec sfpstochrnd_i(uint32_t mode, uint32_t imm, Vec srcc, uint32_t mod1) override;
    Vec sfpstochrnd_v(uint32_t mode, Vec srcb, Vec srcc, uint32_t mod1) override;
    void sfptransp(Vec l0, Vec l1, Vec l2, Vec l3) override;
    Vec sfpshft2_i(Vec dst, uint32_t imm) override;
    Vec sfpshft2_v(Vec dst, Vec src) override;
    void sfpshft2_g(Vec l0, Vec l1, Vec l2, Vec l3, uint32_t mod) override;
    void sfpshft2_ge(Vec src, Vec l0, Vec l1, Vec l2, Vec l3) override;
    Vec sfpshft2_e(Vec src, uint32_t mod) override;
    void sfpswap(Vec dst, Vec src, uint32_t mod) override;
    void sfpconfig_v(Vec l0, uint32_t config_dest) override;
public:
    void print_text(const char *fmt, ...) override;
private:
    void push_node_ret_none(
        Op op,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0);
    uint32_t push_node_ret_auto(
        Op op,
        uint32_t arg0 = 0,
        uint32_t arg1 = 0,
        uint32_t arg2 = 0,
        uint32_t arg3 = 0,
        uint32_t arg4 = 0,
        uint32_t arg5 = 0,
        uint32_t arg6 = 0,
        uint32_t arg7 = 0);
    void dump_graph(const char *tag);
public:
    static constexpr bool ENABLE_PRINT_TEXT = true;
    static constexpr bool ENABLE_DUMP_GRAPH = true;
private:
    Ir m_ir;
    Code m_code;
    TextPool m_text_pool;
    std::unique_ptr<IrPass> m_ir_expand;
    std::unique_ptr<IrPass> m_ir_cc;
};

//
//    Public functions
//

IrPass *new_ir_expand();
IrPass *new_ir_cc();

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

