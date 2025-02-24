// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <cassert>
#include <unordered_map>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

//
//    Expand boolean trees
//
//    The hardware does not support OR and generates some comparisons (LTE, GE)
//    by ANDing others together and issuing a compc. This requires refactoring
//    boolean expressions using De Morgan's laws. The root of a tree is anchored
//    by an sfpxcondb. All dependent operations are chained to this by their
//    return values. This pass traverses the tree, more or less deletes it and
//    replaces it with one that works w/ the HW. 
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    IrExpand
//

class IrExpand: public IrPass {
public:
    IrExpand();
    ~IrExpand();
public:
    void transform(Ir *ir);
private:
    void process_tree(Node *stmt, Node *parent);
    void process_tree_node(
        Node *&pre_pos, 
        Node *&post_pos,
        bool &negated,
        Node *stmt, 
        Node *parent,
        bool negate);
    bool process_xcondi(Node *stmt, Node *parent, bool optimizeit);
    Node *find_use_xcondb(Node *stmt, uint32_t cond);
    void process_bool_tree(
        Node *&pre_pos, 
        Node *&post_pos,
        bool &negated, 
        Node *stmt, 
        uint32_t op, 
        bool negate);
    void expand_xcondi(Node *stmt); 
    Node *find_top_of_cond_tree(Node *stmt);
    void emit_setcc_v(Node *&pos, Node *stmt, Node *in, bool emit_before);
    Node *emit_loadi_lv(
        Node *&pos, 
        Node *stmt, 
        Node *lhs, 
        Node *in, 
        uint32_t val, 
        bool emit_before);
    Node *emit_loadi(Node *&pos, Node *stmt, uint32_t val, bool emit_before);
    void emit_compc(Node *&pos, Node *stmt, bool emit_before);
    void emit_popc(Node *&pos, Node *stmt, bool insert_before);
    void emit_pushc(Node *&pos, Node *stmt, bool insert_before);
    void finish_new_insn(Node *&pos, bool insert_before, Node *new_stmt, Node *stmt);
    Node *copy_and_replace_icmp(Node *stmt, Op op);
    static uint32_t flip_negated_cmp(Node *stmt, bool negate);
    static uint32_t get_bool_type(uint32_t op, bool negate);
    static bool cmp_issues_compc(uint32_t mod);
    static uint32_t negate_cmp_mod(uint32_t mod);
    void remove_stmt(Node *stmt);
    Node *map_node_arg(Node *stmt, int index);
    static void dump(const char *fmt, ...);
public:
    static constexpr bool ENABLE_DUMP = true;
private:
    Ir *m_ir;
};

IrExpand::IrExpand():
        m_ir(nullptr) { }

IrExpand::~IrExpand() { }

void IrExpand::transform(Ir *ir) {
    dump("IR expand pass\n");

    m_ir = ir;

    dump("  process vif loop\n");
    Node *stmt = m_ir->head();
    while (stmt != nullptr) {
        Node *next_stmt = stmt->next();
        if (stmt->op() == Op::SFPXCONDB) {
            dump("  process xcondb\n");
            // sfpxcondb(&s, @i)
            // This will be the sfpxvif stmt
            Node *child = map_node_arg(stmt, 0);
            Node *top = map_node_arg(stmt, 1);
            process_tree(child, stmt);
            remove_stmt(stmt);
            remove_stmt(top);
        }
        stmt = next_stmt;
    }

    // Now process any xcondis that aren't associated w/ a xcondbs
    dump("  process outside vif loop\n");
    stmt = ir->head();
    while (stmt != nullptr) {
        Node *next_stmt = stmt->next();
        if (stmt->op() == Op::SFPXCONDI) {
            dump("  process xcondi tree\n");
            // $ret = sfpxcondi(&i)
            Node *child = map_node_arg(stmt, 0);
            expand_xcondi(stmt);
            process_tree(child, stmt);
        }
        stmt = next_stmt;
    }

    dump("Done: IR expand pass\n\n");
}

void IrExpand::process_tree(Node *stmt, Node *parent) {
    Node *pre_pos = nullptr;
    Node *post_pos = nullptr;
    bool negated = false;
    process_tree_node(pre_pos, post_pos, negated, stmt, parent, false); 
}

void IrExpand::process_tree_node(
        Node *&pre_pos, 
        Node *&post_pos,
        bool &negated,
        Node *stmt, 
        Node *parent,
        bool negate) {
    dump("  process %s n:%d\n", fmt_node(stmt).c_str(), int(negate));
    switch (stmt->op()) {
    case Op::SFPXFCMPS:
    case Op::SFPXFCMPV:
        {
            uint32_t mod = flip_negated_cmp(stmt, negate);
            if (cmp_issues_compc(mod)) {
                negated = true;
            }
            // TODO(?): set_lhs(stmt, NULL_TREE);
            pre_pos = stmt;
            post_pos = stmt;
        }
        break;

    case Op::SFPXICMPS:
        {
            // Note: negation happens at the use of these trees below the fall through
            // &ret = sfpxicmps($v, i, mod)
            Node *child = map_node_arg(stmt, 0);
            if (child->op() == Op::SFPXCONDI) {
                dump("  descending to process xcondi before xicmps\n");
                // Process child before fixing up this insn
                if (process_xcondi(child, stmt, true)) {
                    // Optimized this node away...
                    break;
                }
            }
        }
        // Fall through

    case Op::SFPXICMPV:
        {
            // iadd insns return a vector while icmp insns return an int, remap
            uint32_t mod = flip_negated_cmp(stmt, negate);
            if (cmp_issues_compc(mod)) {
                negated = true;
            }
            Op op = (stmt->op() == Op::SFPXICMPS) ? Op::SFPXIADD_I : Op::SFPXIADD_V;
            stmt = copy_and_replace_icmp(stmt, op);
            pre_pos = stmt;
            post_pos = stmt;
        }
        break;

    case Op::SFPXBOOL:
        {
            // &ret = sfpxbool(t, &a, &b)
            uint32_t op = stmt->arg(0);
            if (op == SFPXBOOL_MOD1_NOT) { 
                Node *child = map_node_arg(stmt, 1);
                process_tree_node(pre_pos, post_pos, negated, child, stmt, !negate);
            } else {
                process_bool_tree(pre_pos, post_pos, negated, stmt, op, negate);
            }
            remove_stmt(stmt);
        }
        break;

    case Op::SFPXCONDI:
        process_xcondi(stmt, parent, false);
        break;

    default:
        fprintf(stderr, 
            "Illegal rvtt builtin found in conditional tree: %s\n", fmt_node(stmt).c_str());
        assert(false);
        break;
    } 
}

bool IrExpand::process_xcondi(Node *stmt, Node *parent, bool optimizeit) {
    // Process the child as a new tree
    // $ret = sfpxcondi(&i)
    Node *child = map_node_arg(stmt, 0);
     
    bool optimized = false;

    if (optimizeit) {
        Node *xcondb = find_use_xcondb(parent, parent->ret());
        if (xcondb != nullptr) {
            dump("  optimizing away xcondi\n");

            // Parent is an xicmps, the single vuse is an xcondb, move the
            // conditional into the xcondb and optimize away the xcondi and the
            // associated xicmps
            // Stuff the xcondi arg into the use of the icmps (and xcondi or xcondb)
            uint32_t xcondi_op = stmt->arg(0);
            xcondb->set_arg(0, xcondi_op);

            remove_stmt(parent);
            remove_stmt(stmt);

            optimized = true;
        } 
    }

    if (!optimized) {
        dump("  expanding xcondi\n");

        // The integer conditional comparison falls outside a v_if, can't optimize
        // Instead, save the result in an int to be used later
        expand_xcondi(stmt);
    }

    process_tree(child, stmt);

    return optimized; 
}

Node *IrExpand::find_use_xcondb(Node *stmt, uint32_t cond) {
    // sfpxcondb(&s, @i)
    for (Node *node = stmt->next(); node != nullptr; node = node->next()) {
        if (node->op() == Op::SFPXCONDB && node->arg(0) == cond) {
            // single use by IR construction
            return node;
        }
    }
    return nullptr;
}

//
//    Handle AND and OR conditionals
//
//    Recursively processes a tree of boolean expressions. ORs are converted to
//    ANDs by negating the children of the current node. The negation is toggled
//    as the tree is traversed to avoid accumulating redundant negations.
//
//    Descending the LHS uses the last PUSHC as the "fence" against which a COMPC
//    can be issued, however, descending the RHS would mess up the results from
//    the LHS w/o a new fence, hence the PUSHC prior to the RHS.  The POPC would
//    destroy the results of the RHS and so those results are saved/restored with
//    saved_enables. 
//

void IrExpand::process_bool_tree(
        Node *&pre_pos, 
        Node *&post_pos,
        bool &negated, 
        Node *stmt, 
        uint32_t op, 
        bool negate) {
    dump("    process %s n:%d\n", op == SFPXBOOL_MOD1_AND ? "AND" : "OR", int(negate));

    bool negate_node = false;
    if (get_bool_type(op, negate) == SFPXBOOL_MOD1_OR) {
        negate_node = true;
        negate = !negate;
    }

    // &ret = sfpxbool(t, &a, &b)

    // Emit LEFT
    Node *left_post_pos = nullptr;
    bool left_negated = false;
    dump("    left\n");
    process_tree_node(
        pre_pos, 
        left_post_pos, 
        left_negated, 
        map_node_arg(stmt, 1), 
        stmt, 
        negate);

    // Emit RIGHT
    Node *right_pre_pos = nullptr;
    bool right_negated = false;
    dump("    right\n");
    process_tree_node(
        right_pre_pos, 
        post_pos, 
        right_negated,
        map_node_arg(stmt, 2),
        stmt, 
        negate);

    if (right_negated) {
        dump("        right negated, emitting pre/post\n");
        emit_pushc(right_pre_pos, stmt, true);
#if 0 // TODO: Revise this
        Node *saved_enables = emit_loadi(right_pre_pos, stmt, 1, true);
        saved_enables = emit_loadi_lv(post_pos, stmt, nullptr, saved_enables, 0, false);
#else
        Node *saved_enables = emit_loadi(post_pos, stmt, 0, false);
#endif
        emit_popc(post_pos, stmt, false);
        emit_setcc_v(post_pos, stmt, saved_enables, false);
    }

    if (negate_node) {
        dump("        node negated, emitting compc\n");
        negated = true;
        emit_compc(post_pos, stmt, false);
    }

    if (left_negated) {
        // Parent needs a fence for this node's left and side
        // (if the parent isn't the root)
        negated = true;
    }

    dump("    exiting bool %d %d\n", op, int(negate)); 
}

//
//    Expand xcondi into:
//        loadi(0)
//        pushc
//        loadi(1)
//        popc
//    Returns results of loadi back to the same SSA as the xcondi for testing, up
//    to the caller to adjust the test as needed (compare against 0) 
//

void IrExpand::expand_xcondi(Node *stmt) {
    // $ret = sfpxcondi(&i)
    Node *child = map_node_arg(stmt, 0);
    Node *top = find_top_of_cond_tree(child);

    Node *pos = top;
    Node *save = emit_loadi(pos, top, 0, true);
    emit_pushc(pos, top, true);
    pos = child;
    // stmt->ret() will be reused: valid because stmt will be removed
    save = emit_loadi_lv(pos, top, stmt, save, 1, false);
    emit_popc(pos, top, false);

    m_ir->remove(stmt);
}

Node *IrExpand::find_top_of_cond_tree(Node *stmt) {
    switch (stmt->op()) {
    case Op::SFPXFCMPS:
    case Op::SFPXFCMPV:
    case Op::SFPXICMPS:
    case Op::SFPXICMPV:
        break;

    case Op::SFPXBOOL:
        {
            // Follow only child for NOT, left-most child for AND/OR, all degenerate to same case
            // &ret = sfpxbool(t, &a, &b)
            Node *child = map_node_arg(stmt, 1);
            return find_top_of_cond_tree(child);
        }
        break;

    case Op::SFPXCONDI:
        // Should never get this deep
        assert(false);
        break;

    default:
        fprintf(stderr, 
            "Illegal rvtt builtin found in conditional tree: %s\n", fmt_node(stmt).c_str());
        assert(false);
        break;
    }

    return stmt; 
}

void IrExpand::emit_setcc_v(Node *&pos, Node *stmt, Node *in, bool emit_before) {
    // sfpsetcc_v($src, mod1)
    Node *new_stmt = 
        m_ir->new_node(
            Op::SFPSETCC_V, 
            Ir::RET_AUTO, 
            in->ret(), 
            SFPSETCC_MOD1_LREG_EQ0);
  finish_new_insn(pos, emit_before, new_stmt, stmt); 
}

Node *IrExpand::emit_loadi_lv(
        Node *&pos, 
        Node *stmt, 
        Node *lhs, 
        Node *in, 
        uint32_t val, 
        bool emit_before) {
    // ACHTUNG: For the time being, emit SFPXLOADI rather than SFPXLOADI_LV
    //     (Revise once usability of liveness is fully understood)
    // Reuse lhs->ret(), if any
    uint32_t ret = (lhs != nullptr) ? lhs->ret() : Ir::RET_AUTO;
    Node *new_stmt = 
        m_ir->new_node(
            Op::SFPXLOADI, 
            ret, 
            SFPLOADI_MOD0_SHORT, 
            val);
    finish_new_insn(pos, emit_before, new_stmt, stmt);
    return new_stmt; 
}

Node *IrExpand::emit_loadi(Node *&pos, Node *stmt, uint32_t val, bool emit_before) {
    // $ret = sfploadi(mod0, imm)
    Node *new_stmt = 
        m_ir->new_node(
            Op::SFPXLOADI, 
            Ir::RET_AUTO, 
            SFPLOADI_MOD0_SHORT, 
            val);
    finish_new_insn(pos, emit_before, new_stmt, stmt);
    return new_stmt; 
}

void IrExpand::emit_compc(Node *&pos, Node *stmt, bool emit_before) {
    // sfpcompc()
    Node *new_stmt = m_ir->new_node(Op::SFPCOMPC, Ir::RET_NONE);
    finish_new_insn(pos, emit_before, new_stmt, stmt); 
}

void IrExpand::emit_popc(Node *&pos, Node *stmt, bool insert_before) {
    // sfppopc()
    // implicit: mod1 = SFPPOPCC_MOD1_POP
    Node *new_stmt = m_ir->new_node(Op::SFPPOPC, Ir::RET_NONE);
    finish_new_insn(pos, insert_before, new_stmt, stmt); 
}

void IrExpand::emit_pushc(Node *&pos, Node *stmt, bool insert_before) {
    // sfpushc()
    // implicit: mod1 = SFPPUSHCC_MOD1_PUSH
    Node *new_stmt = m_ir->new_node(Op::SFPPUSHC, Ir::RET_NONE);
    finish_new_insn(pos, insert_before, new_stmt, stmt); 
}

void IrExpand::finish_new_insn(Node *&pos, bool insert_before, Node *new_stmt, Node *stmt) {
    assert(new_stmt != nullptr);
    // ACHTUNG: Argument stmt was used only to pass its location to new_stmt
    if (insert_before) {
        m_ir->insert(new_stmt, pos);
    } else {
        m_ir->insert(new_stmt, pos->next());
    } 
    pos = new_stmt;
}

Node *IrExpand::copy_and_replace_icmp(Node *stmt, Op op) {
    // SFPXICMPS -> SFPXIADD_I
    // SFPXICMPV -> SFPXIADD_V
    // &ret = sfpxicmps($v, i, mod1)
    // &ret = sfpxicmpv($v1, $v2, mod1)
    // $ret = sfpxiadd_i($src, imm, mod1)
    // $ret = sfpxiadd_v($dst, $src, mod1)
    Node *new_stmt =
        m_ir->new_node(
            op,
            Ir::RET_AUTO,
            stmt->arg(0),
            stmt->arg(1),
            stmt->arg(2));
    m_ir->insert(new_stmt, stmt);
    remove_stmt(stmt);

    // Make the iadd do a subtract for the compare
    // Make sure other code knows this is a compare
    uint32_t mod = new_stmt->arg(2);
    mod |= SFPXIADD_MOD1_IS_SUB;
    if (op == Op::SFPXIADD_I) {
        mod |= SFPXIADD_MOD1_DST_UNUSED;
    }
    new_stmt->set_arg(2, mod);

    return new_stmt; 
}

uint32_t IrExpand::flip_negated_cmp(Node *stmt, bool negate) {
    // &ret = sfpxicmps($v, i, mod1)
    // &ret = sfpxicmpv($v1, $v2, mod1)
    // &ret = sfpxfcmps($v, f, mod1)
    // &ret = sfpxfcmpv($v1, $v2, mod1)
    uint32_t mod = stmt->arg(2);
    if (negate) {
        // Flip the operation
        mod = negate_cmp_mod(mod);
        stmt->set_arg(2, mod);
    }
    return mod; 
}

uint32_t IrExpand::get_bool_type(uint32_t op, bool negate) {
    if (op == SFPXBOOL_MOD1_OR) {
        return negate ? SFPXBOOL_MOD1_AND : SFPXBOOL_MOD1_OR;
    }
    if (op == SFPXBOOL_MOD1_AND) {
        return negate ? SFPXBOOL_MOD1_OR : SFPXBOOL_MOD1_AND;
    }
    assert(0); 
    return 0;
}

bool IrExpand::cmp_issues_compc(uint32_t mod) {
    return ((mod & SFPXCMP_MOD1_CC_MASK) == SFPXCMP_MOD1_CC_LTE); 
}

uint32_t IrExpand::negate_cmp_mod(uint32_t mod) {
    uint32_t op = mod & SFPXCMP_MOD1_CC_MASK;
    uint32_t new_op = 0;

    switch (op) {
    case SFPXCMP_MOD1_CC_LT:
        new_op = SFPXCMP_MOD1_CC_GTE;
        break;
    case SFPXCMP_MOD1_CC_NE:
        new_op = SFPXCMP_MOD1_CC_EQ;
        break;
    case SFPXCMP_MOD1_CC_GTE:
        new_op = SFPXCMP_MOD1_CC_LT;
        break;
    case SFPXCMP_MOD1_CC_EQ:
        new_op = SFPXCMP_MOD1_CC_NE;
        break;
    case SFPXCMP_MOD1_CC_LTE:
        new_op = SFPXCMP_MOD1_CC_GT;
        break;
    case SFPXCMP_MOD1_CC_GT:
        new_op = SFPXCMP_MOD1_CC_LTE;
        break;
    }

    return (mod & ~SFPXCMP_MOD1_CC_MASK) | new_op; 
}

void IrExpand::remove_stmt(Node *stmt) {
    m_ir->remove(stmt);
}

Node *IrExpand::map_node_arg(Node *stmt, int index) {
    return m_ir->map_node(stmt->arg(index));
}

void IrExpand::dump(const char *fmt, ...) {
    if (!ENABLE_DUMP) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

//
//    Public functions
//

IrPass *new_ir_expand() {
    return new IrExpand();
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

