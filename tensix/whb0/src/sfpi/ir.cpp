
#include <cstdint>
#include <string>

#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Ir
//

Ir::Ir():
        m_text_pool(nullptr) { }

Ir::~Ir() { }

void Ir::set_text_pool(TextPool *text_pool) {
    m_text_pool = text_pool;
}

void Ir::reset() {
    m_graph.reset();
    m_node_map.clear();
}

Node *Ir::new_node(
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
    if (ret == RET_AUTO) {
        ret = uint32_t(m_node_map.size());
    }
    Node *node = 
        m_graph.new_node(
            op,
            ret,
            arg0,
            arg1,
            arg2,
            arg3,
            arg4,
            arg5,
            arg6,
            arg7);
    if (ret != RET_NONE) {
        m_node_map.push_back(node);
    }
    return node;
}

void Ir::dump_graph() {
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

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

