
#include <cstdint>
#include <string>

#include "sfpi/rvtt.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace sfpi {

//
//    Node
//

Node::Node():
        m_prev(nullptr),
        m_next(nullptr),
        m_op(Op(0)),
        m_ret(0) {
    for (int i = 0; i < 8; i++) {
        m_args[i] = 0;
    }
}

Node::~Node() { }

//
//    Graph
//

Graph::Graph():
        m_nodes(1024),
        m_top(0),
        m_head(nullptr),
        m_tail(nullptr) { }

Graph::~Graph() { }

void Graph::reset() { 
    m_top = 0;
    m_head = nullptr;
    m_tail = nullptr;
}

Node *Graph::new_node(
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
    int size = int(m_nodes.size());
    if (m_top >= size) {
        assert(m_top == size);
        m_nodes.resize(size + 1024);
    }
    Node *node = m_nodes[m_top].get();
    if (node == nullptr) {
        node = new Node();
        m_nodes[m_top].reset(node);
    }
    m_top++;
    node->m_op = op;
    node->m_ret = ret;
    node->m_args[0] = arg0;
    node->m_args[1] = arg1;
    node->m_args[2] = arg2;
    node->m_args[3] = arg3;
    node->m_args[4] = arg4;
    node->m_args[5] = arg5;
    node->m_args[6] = arg6;
    node->m_args[7] = arg7;
    return node;
}

void Graph::push(Node *node) {
    node->m_prev = m_tail;
    node->m_next = nullptr;
    if (m_head == nullptr) {
        m_head = node;
    } else {
        m_tail->m_next = node;
    }
    m_tail = node;
}

void Graph::insert(Node *node, Node *next) {
    if (next == nullptr) {
        push(node);
        return;
    }
    Node *prev = next->m_prev;
    node->m_prev = prev;
    node->m_next = next;
    if (prev != nullptr) {
        prev->m_next = node;
    } else {
        m_head = node;
    }
    next->m_prev = node;
}

void Graph::remove(Node *node) {
    Node *prev = node->m_prev;
    Node *next = node->m_next;
    if (prev != nullptr) {
        prev->m_next = next;
    } else {
        m_head = next;
    }
    if (next != nullptr) {
        next->m_prev = prev;
    } else {
        m_tail = prev;
    }
    node->m_prev = nullptr;
    node->m_next = nullptr;
}

//
//    TextPool
//

TextPool::TextPool() { }

TextPool::~TextPool() { }

void TextPool::reset() {
    m_pool.clear();
}

uint32_t TextPool::push(const std::string &text) {
    uint32_t id = uint32_t(m_pool.size());
    m_pool.push_back(text);
    return id;
}

std::string TextPool::get(uint32_t id) {
    assert(id < m_pool.size());
    return m_pool[id];
}

} // namespace sfpi
} // namespace whb0
} // namespace iss
} // namespace ronin

