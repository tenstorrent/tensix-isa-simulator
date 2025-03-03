// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

using TemplateOp = std::function<void ()>;

struct MopTemplate0 {
    bool unpack_b;
    bool unpack_halo;
    TemplateOp a0_instr;
    TemplateOp a1_instr;
    TemplateOp a2_instr; 
    TemplateOp a3_instr;
    TemplateOp b_instr;
    TemplateOp skip_a_instr;
    TemplateOp skip_b_instr; 

    MopTemplate0():
        unpack_b(false),
        unpack_halo(false),
        a0_instr(nullptr),
        a1_instr(nullptr),
        a2_instr(nullptr),
        a3_instr(nullptr),
        b_instr(nullptr),
        skip_a_instr(nullptr),
        skip_b_instr(nullptr) { }
};

struct MopTemplate1 {
    uint32_t loop_outer;
    TemplateOp start_op;
    uint32_t loop_inner;
    TemplateOp loop_op0;
    TemplateOp loop_op1;
    TemplateOp last_inner_op;
    TemplateOp last_outer_op;
    TemplateOp end_op0;
    TemplateOp end_op1;

    MopTemplate1():
        loop_outer(0),
        start_op(nullptr),
        loop_inner(0),
        loop_op0(nullptr),
        loop_op1(nullptr),
        last_inner_op(nullptr),
        last_outer_op(nullptr),
        end_op0(nullptr),
        end_op1(nullptr) { }
};

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

