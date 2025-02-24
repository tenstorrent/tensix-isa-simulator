// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <cassert>
#include <vector>
#include <stdexcept>

#include "sfpi/sfpi_hw.hpp"
#include "sfpi/rvtt.hpp"

//
//    This pass optimizes 2 things:
//    1) The outermost pushc/popc: remove pushc, turn popc into encc
//    2) Tail popc: if 2 popcs occur without intervening instructions and the
//       inner pushc/popc did not have a compc in between, the inner pushc/popc
//       can be removed.  Tricky because you have to track the prior popc.
//

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    IrCc
//

class IrCc: public IrPass {
public:
    IrCc();
    ~IrCc();
public:
    void transform(Ir *ir);
private:
    void process_stmts();
    static void error(const char *msg);
    static void dump(const char *fmt, ...);
public:
    static constexpr bool ENABLE_DUMP = true;
private:
    struct StackEntry {
        bool prior_removable;
        bool prior_replace;
        Node *pos;
    };
private:
    Ir *m_ir;
    std::vector<StackEntry> m_stack;
};

IrCc::IrCc():
        m_ir(nullptr) { 
    m_stack.reserve(16);        
}

IrCc::~IrCc() { }

void IrCc::transform(Ir *ir) {
    dump("IR CC pass\n");

    m_ir = ir;

    process_stmts();

    dump("Done: IR CC pass\n\n");
}

void IrCc::process_stmts() {
    Node *prior_pushc = nullptr;
    Node *prior_popc = nullptr;
    bool prior_removable = false;
    bool prior_replace = false;

    Node *pos = m_ir->head();
    while (pos != nullptr) {
        Node *stmt = pos;
        switch (stmt->op()) {
        case Op::SFPPUSHC:
            {
                // sfppushc(mod1)
                bool replace = (stmt->arg(0) == SFPPUSHCC_MOD1_REPLACE);

                prior_removable = false;
                dump("PUSHC(%s): stack size %zd\n", replace ? "replace" : "push", m_stack.size());

                if (m_stack.size() == 0) {
                    if (replace) {
                        error("malformed program, pushc replace at outer level");
                    }

                    dump("  removing outermost pushc\n");

                    // Remove outermost pushc
                    pos = stmt->next();
                    m_ir->remove(stmt);

                    m_stack.emplace_back(StackEntry{false, false, pos});

                    // skip advancing pos at end of iteration
                    continue;
                } else {
                    if (replace) {
                        m_stack.pop_back();
                    }
                    prior_replace = replace;
                    m_stack.emplace_back(StackEntry{false, replace, pos});
                }
            }
            break;

        case Op::SFPCOMPC:
            {
                // Set compc to true for current pushc
                if (m_stack.size() == 0) {
                    error("malformed program, sfpcompc outside of pushc/popc");
                }
                prior_removable = false;
                StackEntry &top = m_stack.back();
                top.prior_removable = true;
            }
            break;

        case Op::SFPPOPC:
            {
                dump("POPC: stack size %zd\n", m_stack.size());

                if (m_stack.size() == 0) {
                    error("malformed program, popc without matching pushc");
                }

                if (prior_removable) {
                    dump("  removing inner PUSHC\n");
                    m_ir->remove(prior_pushc);
                    prior_pushc = nullptr; // TODO: Verify this

                    if (!prior_replace) {
                        dump("  removing inner POPC\n");
                        m_ir->remove(prior_popc);
                        prior_popc = nullptr; // TODO: Verify this
                    }
                }

                // Not removable if we saw a compc
                StackEntry &top = m_stack.back();
                prior_removable = !top.prior_removable; 
                prior_replace = top.prior_replace;
                prior_pushc = top.pos;
                prior_popc = pos;

                m_stack.pop_back();
                if (m_stack.size() == 0) {
                    dump("  replacing outermost popc with encc\n");

                    // Replace outermost popc with encc
                    // spfencc(imm, mod1)
                    Node *new_stmt = m_ir->new_node(Op::SFPENCC, Ir::RET_NONE, 3, 10);
                    m_ir->insert(new_stmt, stmt);
                    m_ir->remove(stmt);
                    pos = new_stmt;
                    prior_removable = false;
                }
            }
            break;

        default:
            dump("Intervening %s\n", fmt_node(stmt).c_str());
            prior_removable = false;
            break;
        }

        pos = pos->next();
    } 
}

void IrCc::error(const char *msg) {
    throw std::runtime_error(msg);
}

void IrCc::dump(const char *fmt, ...) {
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

IrPass *new_ir_cc() {
    return new IrCc();
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

