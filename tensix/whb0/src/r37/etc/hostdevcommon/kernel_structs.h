// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <cstdint>

namespace tt {

// All CBs can used for dataflow in/out
// Certain CBs are specifically designed to handle compute input, output, and intermediates.
enum CB : std::uint8_t
{
  // Designed to be used as compute inputs, or dataflow in/out
  c_in0       = 0,
  c_in1       = 1,
  c_in2       = 2,
  c_in3       = 3,
  c_in4       = 4,
  c_in5       = 5,
  c_in6       = 6,
  c_in7       = 7,

  // Dataflow in/out only
  dataflow0           = 8,
  dataflow1           = 9,
  dataflow2           = 10,
  dataflow3           = 11,
  dataflow4           = 12,
  dataflow5           = 13,
  dataflow6           = 14,
  dataflow7           = 15,

  // Designed to be used as compute outputs, or dataflow in/out
  c_out0      = 16,
  c_out1      = 17,
  c_out2      = 18,
  c_out3      = 19,
  c_out4      = 20,
  c_out5      = 21,
  c_out6      = 22,
  c_out7      = 23,

  // Designed to be used as compute intermediates, or dataflow in/out
  c_intermed0 = 24,
  c_intermed1 = 25,
  c_intermed2 = 26,
  c_intermed3 = 27,
  c_intermed4 = 28,
  c_intermed5 = 29,
  c_intermed6 = 30,
  c_intermed7 = 31,
};
  /////////////////////////////
 // end of user facing APIs //
/////////////////////////////


enum class Dim : std::uint8_t
{
    None      = 0,
    R         = 1,
    C         = 2,
    Z         = 3,
    RC        = 4,
    ZR        = 5,
    Invalid   = 0xFF,
};

enum class SfpuOp : std::uint8_t
{
    Exp,
    Log,
    Sqrt,
    Gelu,
    GeluDerivative,
    Reciprocal,
    Sigmoid,
    Tanh,
    Dropout,
    Datacopy, // This just means passthrough and no sfpu
    Transpose, // datacopy + math transpose
    Invalid,
};

enum class BinaryOp : std::uint8_t
{
    Add,
    Subtract,
    Multiply,
    Power,
    AddRelu, // Add + Relu
    SubtractRelu, // Subtract + Relu
    Invalid,
};

enum class TmOp : std::uint8_t
{
    rBroadcast,
    cBroadcast,
    zBroadcast,
    hSlice,
    hStack,
    vSlice,
    vStack,
    Transpose,
    Invalid,
};

enum class ReduceFunc : std::uint8_t
{
    Sum,
    Avg,  // Needed only on tensor level to compute correct coefficient. Kernel uses Sum.
    Max,
    Invalid,
};

enum DstMode : std::uint8_t
{
    Full          = 0,
    Half          = 1,
    Tile          = 2,
    NUM_DST_MODES = 3,
};

enum class Action {
    None,
    Slice,
    Stack,
};

// To be deprecated: the old enum from which CBs evolved
enum HlkOperand : std::uint8_t
{
  in0       = 0,
  in1       = 1,
  in2       = 2,
  in3       = 3,
  in4       = 4,
  in5       = 5,
  in6       = 6,
  in7       = 7,

  param0    = 8,
  param1    = 9,
  param2    = 10,
  param3    = 11,
  param4    = 12,
  param5    = 13,
  param6    = 14,
  param7    = 15,

  out0      = 16,
  out1      = 17,
  out2      = 18,
  out3      = 19,
  out4      = 20,
  out5      = 21,
  out6      = 22,
  out7      = 23,

  intermed0 = 24,
  intermed1 = 25,
  intermed2 = 26,
  intermed3 = 27,
  intermed4 = 28,
  intermed5 = 29,
  intermed6 = 30,
  intermed7 = 31,
};

enum class OpCode : uint8_t
{
    Exponential = 0,
    Reciprocal  = 1,
    Gelu        = 2,
    Add         = 3,
    Subtract    = 4,
    Multiply    = 5

};


constexpr std::uint32_t NUM_MAX_IN_BUFFERS_PER_CORE = HlkOperand::in7 - HlkOperand::in0 + 1;
constexpr std::uint32_t NUM_MAX_PARAM_BUFFERS_PER_CORE = HlkOperand::param7 - HlkOperand::param0 + 1;
constexpr std::uint32_t NUM_MAX_OUT_BUFFERS_PER_CORE = HlkOperand::out7 - HlkOperand::out0 + 1;
constexpr std::uint32_t NUM_MAX_INTERMED_BUFFERS_PER_CORE = HlkOperand::intermed7 - HlkOperand::intermed0 + 1;

// API for getting op_info for graph_interpreter kernel
struct op_info_t {
    uint32_t op_code; // this maps to either an eltwise unary or binary op
    uint32_t cb_in0_id; // Id of operand0
    uint32_t cb_in1_id; // Id of operand1, unused for eltwise unary op
    uint32_t cb_out_id; // Id of output
    uint32_t pop0; // Whether to pop operand0
    uint32_t pop1; // Whether to pop operand1, unused for eltwise unary op
    uint32_t unary; // Whether or not this is a unary op
};

} //  namespace tt
