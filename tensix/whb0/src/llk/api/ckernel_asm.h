// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
//    Auto-generated file, do not modify!
//

#define TT_OP_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::ADDDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_ADDDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(ResultRegIndex, 11) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::ADDDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::ADDDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::make_op(ckernel::OpType::ADDRCRXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TT_ADDRCRXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3) && \
        ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::ADDRCRXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TTI_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::ADDRCRXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::make_op(ckernel::OpType::ADDRCRZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TT_ADDRCRZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3) && \
        ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::ADDRCRZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TTI_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::ADDRCRZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::make_op(ckernel::OpType::APOOL3S1, clear_dvalid, addr_mode, index_en, dst)
#define TT_APOOL3S1_VALID(clear_dvalid, addr_mode, index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(addr_mode, 7) && \
        ckernel::is_valid(index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::APOOL3S1, clear_dvalid, addr_mode, index_en, dst)
#define TTI_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::APOOL3S1, clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::make_op(ckernel::OpType::APOOL3S2, clear_dvalid, addr_mode, index_en, dst)
#define TT_APOOL3S2_VALID(clear_dvalid, addr_mode, index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(addr_mode, 7) && \
        ckernel::is_valid(index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::APOOL3S2, clear_dvalid, addr_mode, index_en, dst)
#define TTI_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::APOOL3S2, clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::ATCAS, MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TT_ATCAS_VALID(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(MemHierSel, 1) && \
        ckernel::is_valid(SwapVal, 5) && \
        ckernel::is_valid(CmpVal, 4) && \
        ckernel::is_valid(Sel32b, 2) && \
        ckernel::is_valid(DataRegIndex, 6) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATCAS, MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TTI_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATCAS, MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATGETM(mutex_index) \
    ckernel::make_op(ckernel::OpType::ATGETM, mutex_index)
#define TT_ATGETM_VALID(mutex_index) \
    (ckernel::is_valid(mutex_index, 24))
#define TT_ATGETM(mutex_index) \
    ckernel::run_op(OpType::ATGETM, mutex_index)
#define TTI_ATGETM(mutex_index) \
    ckernel::run_op(OpType::ATGETM, mutex_index)

#define TT_OP_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::ATINCGET, MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TT_ATINCGET_VALID(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(MemHierSel, 1) && \
        ckernel::is_valid(WrapVal, 9) && \
        ckernel::is_valid(Sel32b, 2) && \
        ckernel::is_valid(DataRegIndex, 6) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATINCGET, MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TTI_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATINCGET, MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::ATINCGETPTR, MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TT_ATINCGETPTR_VALID(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(MemHierSel, 1) && \
        ckernel::is_valid(NoIncr, 1) && \
        ckernel::is_valid(IncrVal, 4) && \
        ckernel::is_valid(WrapVal, 4) && \
        ckernel::is_valid(Sel32b, 2) && \
        ckernel::is_valid(DataRegIndex, 6) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATINCGETPTR, MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)
#define TTI_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATINCGETPTR, MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATRELM(mutex_index) \
    ckernel::make_op(ckernel::OpType::ATRELM, mutex_index)
#define TT_ATRELM_VALID(mutex_index) \
    (ckernel::is_valid(mutex_index, 24))
#define TT_ATRELM(mutex_index) \
    ckernel::run_op(OpType::ATRELM, mutex_index)
#define TTI_ATRELM(mutex_index) \
    ckernel::run_op(OpType::ATRELM, mutex_index)

#define TT_OP_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::ATSWAP, MemHierSel, SwapMask, DataRegIndex, AddrRegIndex)
#define TT_ATSWAP_VALID(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(MemHierSel, 1) && \
        ckernel::is_valid(SwapMask, 9) && \
        ckernel::is_valid(DataRegIndex, 8) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATSWAP, MemHierSel, SwapMask, DataRegIndex, AddrRegIndex)
#define TTI_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::ATSWAP, MemHierSel, SwapMask, DataRegIndex, AddrRegIndex)

#define TT_OP_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::BITWOPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_BITWOPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(OpSel, 5) && \
        ckernel::is_valid(ResultRegIndex, 6) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::BITWOPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::BITWOPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CLEARDVALID(cleardvalid, reset) \
    ckernel::make_op(ckernel::OpType::CLEARDVALID, cleardvalid, reset)
#define TT_CLEARDVALID_VALID(cleardvalid, reset) \
    (ckernel::is_valid(cleardvalid, 2) && \
        ckernel::is_valid(reset, 22))
#define TT_CLEARDVALID(cleardvalid, reset) \
    ckernel::run_op(OpType::CLEARDVALID, cleardvalid, reset)
#define TTI_CLEARDVALID(cleardvalid, reset) \
    ckernel::run_op(OpType::CLEARDVALID, cleardvalid, reset)

#define TT_OP_CLREXPHIST \
    ckernel::make_op(ckernel::OpType::CLREXPHIST)
#define TTI_CLREXPHIST \
    ckernel::run_op(OpType::CLREXPHIST)

#define TT_OP_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::CMPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_CMPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(OpSel, 5) && \
        ckernel::is_valid(ResultRegIndex, 6) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::CMPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::CMPDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::CONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)
#define TT_CONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(rotate_weights, 5) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(dst, 15))
#define TT_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::CONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)
#define TTI_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::CONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::CONV3S2, clear_dvalid, rotate_weights, addr_mode, dst)
#define TT_CONV3S2_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(rotate_weights, 5) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(dst, 15))
#define TT_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::CONV3S2, clear_dvalid, rotate_weights, addr_mode, dst)
#define TTI_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::CONV3S2, clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_DMANOP \
    ckernel::make_op(ckernel::OpType::DMANOP)
#define TTI_DMANOP \
    ckernel::run_op(OpType::DMANOP)

#define TT_OP_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::DOTPV, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TT_DOTPV_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(dest_accum_en, 1) && \
        ckernel::is_valid(instr_mod19, 2) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::DOTPV, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TTI_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::DOTPV, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::ELWADD, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TT_ELWADD_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(dest_accum_en, 1) && \
        ckernel::is_valid(instr_mod19, 2) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWADD, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TTI_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWADD, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::ELWMUL, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TT_ELWMUL_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(dest_accum_en, 1) && \
        ckernel::is_valid(instr_mod19, 2) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWMUL, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TTI_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWMUL, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::ELWSUB, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TT_ELWSUB_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(dest_accum_en, 1) && \
        ckernel::is_valid(instr_mod19, 2) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWSUB, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)
#define TTI_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::ELWSUB, clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_FLUSHDMA(FlushSpec) \
    ckernel::make_op(ckernel::OpType::FLUSHDMA, FlushSpec)
#define TT_FLUSHDMA_VALID(FlushSpec) \
    (ckernel::is_valid(FlushSpec, 24))
#define TT_FLUSHDMA(FlushSpec) \
    ckernel::run_op(OpType::FLUSHDMA, FlushSpec)
#define TTI_FLUSHDMA(FlushSpec) \
    ckernel::run_op(OpType::FLUSHDMA, FlushSpec)

#define TT_OP_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::make_op(ckernel::OpType::GAPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)
#define TT_GAPOOL_VALID(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(instr_mod19, 3) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(max_pool_index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::run_op(OpType::GAPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)
#define TTI_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::run_op(OpType::GAPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)

#define TT_OP_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
    ckernel::make_op(ckernel::OpType::GATESRCRST, reset_srcb_gate_control, reset_srca_gate_control)
#define TT_GATESRCRST_VALID(reset_srcb_gate_control, reset_srca_gate_control) \
    (ckernel::is_valid(reset srcb gate control, 23) && \
        ckernel::is_valid(reset srca gate control, 1))
#define TT_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
    ckernel::run_op(OpType::GATESRCRST, reset_srcb_gate_control, reset_srca_gate_control)
#define TTI_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
    ckernel::run_op(OpType::GATESRCRST, reset_srcb_gate_control, reset_srca_gate_control)

#define TT_OP_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::make_op(ckernel::OpType::GMPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)
#define TT_GMPOOL_VALID(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(instr_mod19, 3) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(max_pool_index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::run_op(OpType::GMPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)
#define TTI_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
    ckernel::run_op(OpType::GMPOOL, clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)

#define TT_OP_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::make_op(ckernel::OpType::INCADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
#define TT_INCADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::run_op(OpType::INCADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
#define TTI_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::run_op(OpType::INCADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::make_op(ckernel::OpType::INCADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
#define TT_INCADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::run_op(OpType::INCADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
#define TTI_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
    ckernel::run_op(OpType::INCADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
    ckernel::make_op(ckernel::OpType::INCRWC, rwc_cr, rwc_d, rwc_b, rwc_a)
#define TT_INCRWC_VALID(rwc_cr, rwc_d, rwc_b, rwc_a) \
    (ckernel::is_valid(rwc_cr, 6) && \
        ckernel::is_valid(rwc_d, 4) && \
        ckernel::is_valid(rwc_b, 4) && \
        ckernel::is_valid(rwc_a, 4))
#define TT_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
    ckernel::run_op(OpType::INCRWC, rwc_cr, rwc_d, rwc_b, rwc_a)
#define TTI_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
    ckernel::run_op(OpType::INCRWC, rwc_cr, rwc_d, rwc_b, rwc_a)

#define TT_OP_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::LOADIND, SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)
#define TT_LOADIND_VALID(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(SizeSel, 2) && \
        ckernel::is_valid(OffsetIndex, 8) && \
        ckernel::is_valid(AutoIncSpec, 2) && \
        ckernel::is_valid(DataRegIndex, 6) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::LOADIND, SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)
#define TTI_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::LOADIND, SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_LOADREG(TdmaDataRegIndex, RegAddr) \
    ckernel::make_op(ckernel::OpType::LOADREG, TdmaDataRegIndex, RegAddr)
#define TT_LOADREG_VALID(TdmaDataRegIndex, RegAddr) \
    (ckernel::is_valid(TdmaDataRegIndex, 6) && \
        ckernel::is_valid(RegAddr, 18))
#define TT_LOADREG(TdmaDataRegIndex, RegAddr) \
    ckernel::run_op(OpType::LOADREG, TdmaDataRegIndex, RegAddr)
#define TTI_LOADREG(TdmaDataRegIndex, RegAddr) \
    ckernel::run_op(OpType::LOADREG, TdmaDataRegIndex, RegAddr)

#define TT_OP_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::MFCONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)
#define TT_MFCONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(rotate_weights, 5) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(dst, 15))
#define TT_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::MFCONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)
#define TTI_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
    ckernel::run_op(OpType::MFCONV3S1, clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_MOP(mop_type, loop_count, zmask_lo16) \
    ckernel::make_op(ckernel::OpType::MOP, mop_type, loop_count, zmask_lo16)
#define TT_MOP_VALID(mop_type, loop_count, zmask_lo16) \
    (ckernel::is_valid(mop_type, 1) && \
        ckernel::is_valid(loop_count, 7) && \
        ckernel::is_valid(zmask_lo16, 16))
#define TT_MOP(mop_type, loop_count, zmask_lo16) \
    ckernel::run_op(OpType::MOP, mop_type, loop_count, zmask_lo16)
#define TTI_MOP(mop_type, loop_count, zmask_lo16) \
    ckernel::run_op(OpType::MOP, mop_type, loop_count, zmask_lo16)

#define TT_OP_MOP_CFG(zmask_hi16) \
    ckernel::make_op(ckernel::OpType::MOP_CFG, zmask_hi16)
#define TT_MOP_CFG_VALID(zmask_hi16) \
    (ckernel::is_valid(zmask_hi16, 24))
#define TT_MOP_CFG(zmask_hi16) \
    ckernel::run_op(OpType::MOP_CFG, zmask_hi16)
#define TTI_MOP_CFG(zmask_hi16) \
    ckernel::run_op(OpType::MOP_CFG, zmask_hi16)

#define TT_OP_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::make_op(ckernel::OpType::MOVA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TT_MOVA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    (ckernel::is_valid(dest_32b_lo, 1) && \
        ckernel::is_valid(src, 6) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(dst, 12))
#define TT_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TTI_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVB2A(srca, addr_mode, instr_mod, srcb) \
    ckernel::make_op(ckernel::OpType::MOVB2A, srca, addr_mode, instr_mod, srcb)
#define TT_MOVB2A_VALID(srca, addr_mode, instr_mod, srcb) \
    (ckernel::is_valid(srca, 7) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(srcb, 12))
#define TT_MOVB2A(srca, addr_mode, instr_mod, srcb) \
    ckernel::run_op(OpType::MOVB2A, srca, addr_mode, instr_mod, srcb)
#define TTI_MOVB2A(srca, addr_mode, instr_mod, srcb) \
    ckernel::run_op(OpType::MOVB2A, srca, addr_mode, instr_mod, srcb)

#define TT_OP_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::make_op(ckernel::OpType::MOVB2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TT_MOVB2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    (ckernel::is_valid(dest_32b_lo, 1) && \
        ckernel::is_valid(src, 6) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(dst, 12))
#define TT_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVB2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TTI_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVB2D, dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::make_op(ckernel::OpType::MOVD2A, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TT_MOVD2A_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    (ckernel::is_valid(dest_32b_lo, 1) && \
        ckernel::is_valid(src, 6) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(dst, 12))
#define TT_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVD2A, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TTI_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVD2A, dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::make_op(ckernel::OpType::MOVD2B, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TT_MOVD2B_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    (ckernel::is_valid(dest_32b_lo, 1) && \
        ckernel::is_valid(src, 6) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(dst, 12))
#define TT_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVD2B, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TTI_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVD2B, dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::make_op(ckernel::OpType::MOVDBGA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TT_MOVDBGA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    (ckernel::is_valid(dest_32b_lo, 1) && \
        ckernel::is_valid(src, 6) && \
        ckernel::is_valid(addr_mode, 2) && \
        ckernel::is_valid(instr_mod, 3) && \
        ckernel::is_valid(dst, 12))
#define TT_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVDBGA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)
#define TTI_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    ckernel::run_op(OpType::MOVDBGA2D, dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::make_op(ckernel::OpType::MPOOL3S1, clear_dvalid, addr_mode, index_en, dst)
#define TT_MPOOL3S1_VALID(clear_dvalid, addr_mode, index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(addr_mode, 7) && \
        ckernel::is_valid(index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::MPOOL3S1, clear_dvalid, addr_mode, index_en, dst)
#define TTI_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::MPOOL3S1, clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::make_op(ckernel::OpType::MPOOL3S2, clear_dvalid, addr_mode, index_en, dst)
#define TT_MPOOL3S2_VALID(clear_dvalid, addr_mode, index_en, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(addr_mode, 7) && \
        ckernel::is_valid(index_en, 1) && \
        ckernel::is_valid(dst, 14))
#define TT_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::MPOOL3S2, clear_dvalid, addr_mode, index_en, dst)
#define TTI_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
    ckernel::run_op(OpType::MPOOL3S2, clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::MULDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_MULDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(ResultRegIndex, 11) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::MULDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::MULDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
    ckernel::make_op(ckernel::OpType::MVMUL, clear_dvalid, instr_mod19, addr_mode, dst)
#define TT_MVMUL_VALID(clear_dvalid, instr_mod19, addr_mode, dst) \
    (ckernel::is_valid(clear_dvalid, 2) && \
        ckernel::is_valid(instr_mod19, 3) && \
        ckernel::is_valid(addr_mode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::MVMUL, clear_dvalid, instr_mod19, addr_mode, dst)
#define TTI_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
    ckernel::run_op(OpType::MVMUL, clear_dvalid, instr_mod19, addr_mode, dst)

#define TT_OP_NOP \
    ckernel::make_op(ckernel::OpType::NOP)
#define TTI_NOP \
    ckernel::run_op(OpType::NOP)

#define TT_OP_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
    ckernel::make_op(ckernel::OpType::PACR, AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last)
#define TT_PACR_VALID(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
    (ckernel::is_valid(AddrMode, 9) && \
        ckernel::is_valid(ZeroWrite, 3) && \
        ckernel::is_valid(PackSel, 4) && \
        ckernel::is_valid(OvrdThreadId, 1) && \
        ckernel::is_valid(Concat, 3) && \
        ckernel::is_valid(Flush, 3) && \
        ckernel::is_valid(Last, 1))
#define TT_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
    ckernel::run_op(OpType::PACR, AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last)
#define TTI_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
    ckernel::run_op(OpType::PACR, AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last)

#define TT_OP_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
    ckernel::make_op(ckernel::OpType::PACR_SETREG, Push, AddrSel, WrData, PackSel, StreamId, Flush, Last)
#define TT_PACR_SETREG_VALID(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
    (ckernel::is_valid(Push, 1) && \
        ckernel::is_valid(AddrSel, 1) && \
        ckernel::is_valid(WrData, 10) && \
        ckernel::is_valid(PackSel, 4) && \
        ckernel::is_valid(StreamId, 6) && \
        ckernel::is_valid(Flush, 1) && \
        ckernel::is_valid(Last, 1))
#define TT_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
    ckernel::run_op(OpType::PACR_SETREG, Push, AddrSel, WrData, PackSel, StreamId, Flush, Last)
#define TTI_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
    ckernel::run_op(OpType::PACR_SETREG, Push, AddrSel, WrData, PackSel, StreamId, Flush, Last)

#define TT_OP_RAREB \
    ckernel::make_op(ckernel::OpType::RAREB)
#define TTI_RAREB \
    ckernel::run_op(OpType::RAREB)

#define TT_OP_RDCFG(GprAddress, CfgReg) \
    ckernel::make_op(ckernel::OpType::RDCFG, GprAddress, CfgReg)
#define TT_RDCFG_VALID(GprAddress, CfgReg) \
    (ckernel::is_valid(GprAddress, 8) && \
        ckernel::is_valid(CfgReg, 16))
#define TT_RDCFG(GprAddress, CfgReg) \
    ckernel::run_op(OpType::RDCFG, GprAddress, CfgReg)
#define TTI_RDCFG(GprAddress, CfgReg) \
    ckernel::run_op(OpType::RDCFG, GprAddress, CfgReg)

#define TT_OP_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
    ckernel::make_op(ckernel::OpType::REG2FLOP, SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex)
#define TT_REG2FLOP_VALID(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
    (ckernel::is_valid(SizeSel, 2) && \
        ckernel::is_valid(TargetSel, 2) && \
        ckernel::is_valid(ByteOffset, 2) && \
        ckernel::is_valid(ContextId_2, 2) && \
        ckernel::is_valid(FlopIndex, 10) && \
        ckernel::is_valid(RegIndex, 6))
#define TT_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
    ckernel::run_op(OpType::REG2FLOP, SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex)
#define TTI_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
    ckernel::run_op(OpType::REG2FLOP, SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex)

#define TT_OP_REPLAY(start_idx, len, execute_while_loading, load_mode) \
    ckernel::make_op(ckernel::OpType::REPLAY, start_idx, len, execute_while_loading, load_mode)
#define TT_REPLAY_VALID(start_idx, len, execute_while_loading, load_mode) \
    (ckernel::is_valid(start_idx, 10) && \
        ckernel::is_valid(len, 10) && \
        ckernel::is_valid(execute_while_loading, 3) && \
        ckernel::is_valid(load_mode, 1))
#define TT_REPLAY(start_idx, len, execute_while_loading, load_mode) \
    ckernel::run_op(OpType::REPLAY, start_idx, len, execute_while_loading, load_mode)
#define TTI_REPLAY(start_idx, len, execute_while_loading, load_mode) \
    ckernel::run_op(OpType::REPLAY, start_idx, len, execute_while_loading, load_mode)

#define TT_OP_RMWCIB0(Mask, Data, CfgRegAddr) \
    ckernel::make_op(ckernel::OpType::RMWCIB0, Mask, Data, CfgRegAddr)
#define TT_RMWCIB0_VALID(Mask, Data, CfgRegAddr) \
    (ckernel::is_valid(Mask, 8) && \
        ckernel::is_valid(Data, 8) && \
        ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB0(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB0, Mask, Data, CfgRegAddr)
#define TTI_RMWCIB0(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB0, Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB1(Mask, Data, CfgRegAddr) \
    ckernel::make_op(ckernel::OpType::RMWCIB1, Mask, Data, CfgRegAddr)
#define TT_RMWCIB1_VALID(Mask, Data, CfgRegAddr) \
    (ckernel::is_valid(Mask, 8) && \
        ckernel::is_valid(Data, 8) && \
        ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB1(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB1, Mask, Data, CfgRegAddr)
#define TTI_RMWCIB1(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB1, Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB2(Mask, Data, CfgRegAddr) \
    ckernel::make_op(ckernel::OpType::RMWCIB2, Mask, Data, CfgRegAddr)
#define TT_RMWCIB2_VALID(Mask, Data, CfgRegAddr) \
    (ckernel::is_valid(Mask, 8) && \
        ckernel::is_valid(Data, 8) && \
        ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB2(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB2, Mask, Data, CfgRegAddr)
#define TTI_RMWCIB2(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB2, Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB3(Mask, Data, CfgRegAddr) \
    ckernel::make_op(ckernel::OpType::RMWCIB3, Mask, Data, CfgRegAddr)
#define TT_RMWCIB3_VALID(Mask, Data, CfgRegAddr) \
    (ckernel::is_valid(Mask, 8) && \
        ckernel::is_valid(Data, 8) && \
        ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB3(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB3, Mask, Data, CfgRegAddr)
#define TTI_RMWCIB3(Mask, Data, CfgRegAddr) \
    ckernel::run_op(OpType::RMWCIB3, Mask, Data, CfgRegAddr)

#define TT_OP_RSTDMA \
    ckernel::make_op(ckernel::OpType::RSTDMA)
#define TTI_RSTDMA \
    ckernel::run_op(OpType::RSTDMA)

#define TT_OP_SEMGET(sem_sel) \
    ckernel::make_op(ckernel::OpType::SEMGET, sem_sel)
#define TT_SEMGET_VALID(sem_sel) \
    (ckernel::is_valid(sem_sel, 22))
#define TT_SEMGET(sem_sel) \
    ckernel::run_op(OpType::SEMGET, sem_sel)
#define TTI_SEMGET(sem_sel) \
    ckernel::run_op(OpType::SEMGET, sem_sel)

#define TT_OP_SEMINIT(max_value, init_value, sem_sel) \
    ckernel::make_op(ckernel::OpType::SEMINIT, max_value, init_value, sem_sel)
#define TT_SEMINIT_VALID(max_value, init_value, sem_sel) \
    (ckernel::is_valid(max_value, 4) && \
        ckernel::is_valid(init_value, 4) && \
        ckernel::is_valid(sem_sel, 14))
#define TT_SEMINIT(max_value, init_value, sem_sel) \
    ckernel::run_op(OpType::SEMINIT, max_value, init_value, sem_sel)
#define TTI_SEMINIT(max_value, init_value, sem_sel) \
    ckernel::run_op(OpType::SEMINIT, max_value, init_value, sem_sel)

#define TT_OP_SEMPOST(sem_sel) \
    ckernel::make_op(ckernel::OpType::SEMPOST, sem_sel)
#define TT_SEMPOST_VALID(sem_sel) \
    (ckernel::is_valid(sem_sel, 22))
#define TT_SEMPOST(sem_sel) \
    ckernel::run_op(OpType::SEMPOST, sem_sel)
#define TTI_SEMPOST(sem_sel) \
    ckernel::run_op(OpType::SEMPOST, sem_sel)

#define TT_OP_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
    ckernel::make_op(ckernel::OpType::SEMWAIT, stall_res, sem_sel, wait_sem_cond)
#define TT_SEMWAIT_VALID(stall_res, sem_sel, wait_sem_cond) \
    (ckernel::is_valid(stall_res, 9) && \
        ckernel::is_valid(sem_sel, 13) && \
        ckernel::is_valid(wait_sem_cond, 2))
#define TT_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
    ckernel::run_op(OpType::SEMWAIT, stall_res, sem_sel, wait_sem_cond)
#define TTI_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
    ckernel::run_op(OpType::SEMWAIT, stall_res, sem_sel, wait_sem_cond)

#define TT_OP_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
    ckernel::make_op(ckernel::OpType::SETADC, CntSetMask, ChannelIndex, DimensionIndex, Value)
#define TT_SETADC_VALID(CntSetMask, ChannelIndex, DimensionIndex, Value) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(ChannelIndex, 1) && \
        ckernel::is_valid(DimensionIndex, 2) && \
        ckernel::is_valid(Value, 18))
#define TT_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
    ckernel::run_op(OpType::SETADC, CntSetMask, ChannelIndex, DimensionIndex, Value)
#define TTI_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
    ckernel::run_op(OpType::SETADC, CntSetMask, ChannelIndex, DimensionIndex, Value)

#define TT_OP_SETADCXX(CntSetMask, x_end2, x_start) \
    ckernel::make_op(ckernel::OpType::SETADCXX, CntSetMask, x_end2, x_start)
#define TT_SETADCXX_VALID(CntSetMask, x_end2, x_start) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(x_end2, 11) && \
        ckernel::is_valid(x_start, 10))
#define TT_SETADCXX(CntSetMask, x_end2, x_start) \
    ckernel::run_op(OpType::SETADCXX, CntSetMask, x_end2, x_start)
#define TTI_SETADCXX(CntSetMask, x_end2, x_start) \
    ckernel::run_op(OpType::SETADCXX, CntSetMask, x_end2, x_start)

#define TT_OP_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::make_op(ckernel::OpType::SETADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TT_SETADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3) && \
        ckernel::is_valid(BitMask, 6))
#define TT_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::SETADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TTI_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::SETADCXY, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::make_op(ckernel::OpType::SETADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TT_SETADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    (ckernel::is_valid(CntSetMask, 3) && \
        ckernel::is_valid(Ch1_Y, 6) && \
        ckernel::is_valid(Ch1_X, 3) && \
        ckernel::is_valid(Ch0_Y, 3) && \
        ckernel::is_valid(Ch0_X, 3) && \
        ckernel::is_valid(BitMask, 6))
#define TT_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::SETADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)
#define TTI_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
    ckernel::run_op(OpType::SETADCZW, CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETASHRMH(reg_mask, halo_mask) \
    ckernel::make_op(ckernel::OpType::SETASHRMH, reg_mask, halo_mask)
#define TT_SETASHRMH_VALID(reg_mask, halo_mask) \
    (ckernel::is_valid(reg_mask, 23) && \
        ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH, reg_mask, halo_mask)
#define TTI_SETASHRMH(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH, reg_mask, halo_mask)

#define TT_OP_SETASHRMH0(reg_mask, halo_mask) \
    ckernel::make_op(ckernel::OpType::SETASHRMH0, reg_mask, halo_mask)
#define TT_SETASHRMH0_VALID(reg_mask, halo_mask) \
    (ckernel::is_valid(reg_mask, 23) && \
        ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH0(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH0, reg_mask, halo_mask)
#define TTI_SETASHRMH0(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH0, reg_mask, halo_mask)

#define TT_OP_SETASHRMH1(reg_mask, halo_mask) \
    ckernel::make_op(ckernel::OpType::SETASHRMH1, reg_mask, halo_mask)
#define TT_SETASHRMH1_VALID(reg_mask, halo_mask) \
    (ckernel::is_valid(reg_mask, 23) && \
        ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH1(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH1, reg_mask, halo_mask)
#define TTI_SETASHRMH1(reg_mask, halo_mask) \
    ckernel::run_op(OpType::SETASHRMH1, reg_mask, halo_mask)

#define TT_OP_SETASHRMV(reg_mask2) \
    ckernel::make_op(ckernel::OpType::SETASHRMV, reg_mask2)
#define TT_SETASHRMV_VALID(reg_mask2) \
    (ckernel::is_valid(reg_mask2, 24))
#define TT_SETASHRMV(reg_mask2) \
    ckernel::run_op(OpType::SETASHRMV, reg_mask2)
#define TTI_SETASHRMV(reg_mask2) \
    ckernel::run_op(OpType::SETASHRMV, reg_mask2)

#define TT_OP_SETC16(setc16_reg, setc16_value) \
    ckernel::make_op(ckernel::OpType::SETC16, setc16_reg, setc16_value)
#define TT_SETC16_VALID(setc16_reg, setc16_value) \
    (ckernel::is_valid(setc16_reg, 8) && \
        ckernel::is_valid(setc16_value, 16))
#define TT_SETC16(setc16_reg, setc16_value) \
    ckernel::run_op(OpType::SETC16, setc16_reg, setc16_value)
#define TTI_SETC16(setc16_reg, setc16_value) \
    ckernel::run_op(OpType::SETC16, setc16_reg, setc16_value)

#define TT_OP_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
    ckernel::make_op(ckernel::OpType::SETDMAREG, Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b)
#define TT_SETDMAREG_VALID(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
    (ckernel::is_valid(Payload_SigSelSize, 2) && \
        ckernel::is_valid(Payload_SigSel, 14) && \
        ckernel::is_valid(SetSignalsMode, 1) && \
        ckernel::is_valid(RegIndex16b, 7))
#define TT_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
    ckernel::run_op(OpType::SETDMAREG, Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b)
#define TTI_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
    ckernel::run_op(OpType::SETDMAREG, Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b)

#define TT_OP_SETDVALID(setvalid) \
    ckernel::make_op(ckernel::OpType::SETDVALID, setvalid)
#define TT_SETDVALID_VALID(setvalid) \
    (ckernel::is_valid(setvalid, 24))
#define TT_SETDVALID(setvalid) \
    ckernel::run_op(OpType::SETDVALID, setvalid)
#define TTI_SETDVALID(setvalid) \
    ckernel::run_op(OpType::SETDVALID, setvalid)

#define TT_OP_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
    ckernel::make_op(ckernel::OpType::SETIBRWC, rwc_cr, rwc_bias, set_inc_ctrl)
#define TT_SETIBRWC_VALID(rwc_cr, rwc_bias, set_inc_ctrl) \
    (ckernel::is_valid(rwc_cr, 6) && \
        ckernel::is_valid(rwc_bias, 12) && \
        ckernel::is_valid(set_inc_ctrl, 6))
#define TT_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
    ckernel::run_op(OpType::SETIBRWC, rwc_cr, rwc_bias, set_inc_ctrl)
#define TTI_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
    ckernel::run_op(OpType::SETIBRWC, rwc_cr, rwc_bias, set_inc_ctrl)

#define TT_OP_SETPKEDGOF(y_end, y_start, x_end, x_start) \
    ckernel::make_op(ckernel::OpType::SETPKEDGOF, y_end, y_start, x_end, x_start)
#define TT_SETPKEDGOF_VALID(y_end, y_start, x_end, x_start) \
    (ckernel::is_valid(y_end, 12) && \
        ckernel::is_valid(y_start, 4) && \
        ckernel::is_valid(x_end, 4) && \
        ckernel::is_valid(x_start, 4))
#define TT_SETPKEDGOF(y_end, y_start, x_end, x_start) \
    ckernel::run_op(OpType::SETPKEDGOF, y_end, y_start, x_end, x_start)
#define TTI_SETPKEDGOF(y_end, y_start, x_end, x_start) \
    ckernel::run_op(OpType::SETPKEDGOF, y_end, y_start, x_end, x_start)

#define TT_OP_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
    ckernel::make_op(ckernel::OpType::SETRWC, clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask)
#define TT_SETRWC_VALID(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
    (ckernel::is_valid(clear_ab_vld, 2) && \
        ckernel::is_valid(rwc_cr, 4) && \
        ckernel::is_valid(rwc_d, 4) && \
        ckernel::is_valid(rwc_b, 4) && \
        ckernel::is_valid(rwc_a, 4) && \
        ckernel::is_valid(BitMask, 6))
#define TT_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
    ckernel::run_op(OpType::SETRWC, clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask)
#define TTI_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
    ckernel::run_op(OpType::SETRWC, clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask)

#define TT_OP_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPABS, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPABS_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPABS, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPABS, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPADD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPADD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(lreg_src_a, 8) && \
        ckernel::is_valid(lreg_src_b, 4) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPADD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPADD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPADDI, imm16_math, lreg_dest, instr_mod1)
#define TT_SFPADDI_VALID(imm16_math, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm16_math, 16) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPADDI, imm16_math, lreg_dest, instr_mod1)
#define TTI_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPADDI, imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPAND, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPAND_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPAND, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPAND, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPCAST, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPCAST_VALID(lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(lreg_src_c, 16) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCAST, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCAST, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPCOMPC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPCOMPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCOMPC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCOMPC, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPCONFIG, imm16_math, config_dest, instr_mod1)
#define TT_SFPCONFIG_VALID(imm16_math, config_dest, instr_mod1) \
    (ckernel::is_valid(imm16_math, 16) && \
        ckernel::is_valid(config_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCONFIG, imm16_math, config_dest, instr_mod1)
#define TTI_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPCONFIG, imm16_math, config_dest, instr_mod1)

#define TT_OP_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPDIVP2, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPDIVP2_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPDIVP2, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPDIVP2, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPENCC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPENCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPENCC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPENCC, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPEXEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPEXEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPEXEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPEXEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPEXMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPEXMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPEXMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPEXMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPIADD, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPIADD_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPIADD, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPIADD, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::make_op(ckernel::OpType::SFPLOAD, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TT_SFPLOAD_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    (ckernel::is_valid(lreg_ind, 4) && \
        ckernel::is_valid(instr_mod0, 4) && \
        ckernel::is_valid(sfpu_addr_mode, 2) && \
        ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLOAD, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TTI_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLOAD, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLOADI(lreg_ind, instr_mod0, imm16) \
    ckernel::make_op(ckernel::OpType::SFPLOADI, lreg_ind, instr_mod0, imm16)
#define TT_SFPLOADI_VALID(lreg_ind, instr_mod0, imm16) \
    (ckernel::is_valid(lreg_ind, 4) && \
        ckernel::is_valid(instr_mod0, 4) && \
        ckernel::is_valid(imm16, 16))
#define TT_SFPLOADI(lreg_ind, instr_mod0, imm16) \
    ckernel::run_op(OpType::SFPLOADI, lreg_ind, instr_mod0, imm16)
#define TTI_SFPLOADI(lreg_ind, instr_mod0, imm16) \
    ckernel::run_op(OpType::SFPLOADI, lreg_ind, instr_mod0, imm16)

#define TT_OP_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::make_op(ckernel::OpType::SFPLOADMACRO, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TT_SFPLOADMACRO_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    (ckernel::is_valid(lreg_ind, 4) && \
        ckernel::is_valid(instr_mod0, 4) && \
        ckernel::is_valid(sfpu_addr_mode, 2) && \
        ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLOADMACRO, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TTI_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLOADMACRO, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
    ckernel::make_op(ckernel::OpType::SFPLUT, lreg_ind, instr_mod0, dest_reg_addr)
#define TT_SFPLUT_VALID(lreg_ind, instr_mod0, dest_reg_addr) \
    (ckernel::is_valid(lreg_ind, 4) && \
        ckernel::is_valid(instr_mod0, 4) && \
        ckernel::is_valid(dest_reg_addr, 16))
#define TT_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLUT, lreg_ind, instr_mod0, dest_reg_addr)
#define TTI_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
    ckernel::run_op(OpType::SFPLUT, lreg_ind, instr_mod0, dest_reg_addr)

#define TT_OP_SFPLUTFP32(lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPLUTFP32, lreg_dest, instr_mod1)
#define TT_SFPLUTFP32_VALID(lreg_dest, instr_mod1) \
    (ckernel::is_valid(lreg_dest, 20) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLUTFP32(lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPLUTFP32, lreg_dest, instr_mod1)
#define TTI_SFPLUTFP32(lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPLUTFP32, lreg_dest, instr_mod1)

#define TT_OP_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPLZ, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPLZ_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPLZ, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPLZ, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPMAD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPMAD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(lreg_src_a, 8) && \
        ckernel::is_valid(lreg_src_b, 4) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMAD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMAD, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPMOV, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPMOV_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMOV, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMOV, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPMUL, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPMUL_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(lreg_src_a, 8) && \
        ckernel::is_valid(lreg_src_b, 4) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMUL, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMUL, lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPMULI, imm16_math, lreg_dest, instr_mod1)
#define TT_SFPMULI_VALID(imm16_math, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm16_math, 16) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMULI, imm16_math, lreg_dest, instr_mod1)
#define TTI_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPMULI, imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPNOP \
    ckernel::make_op(ckernel::OpType::SFPNOP)
#define TTI_SFPNOP \
    ckernel::run_op(OpType::SFPNOP)

#define TT_OP_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPNOT, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPNOT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPNOT, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPNOT, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPOR, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPOR, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPOR, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPPOPC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPPOPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPPOPC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPPOPC, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPPUSHC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPPUSHC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPPUSHC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPPUSHC, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSETCC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPSETCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETCC, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETCC, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSETEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPSETEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETEXP, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSETMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPSETMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETMAN, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSETSGN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPSETSGN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETSGN, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSETSGN, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSHFT, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPSHFT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSHFT, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSHFT, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSHFT2, imm12_math, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPSHFT2_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSHFT2, imm12_math, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSHFT2, imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::make_op(ckernel::OpType::SFPSTORE, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TT_SFPSTORE_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    (ckernel::is_valid(lreg_ind, 4) && \
        ckernel::is_valid(instr_mod0, 4) && \
        ckernel::is_valid(sfpu_addr_mode, 2) && \
        ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPSTORE, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)
#define TTI_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
    ckernel::run_op(OpType::SFPSTORE, lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPSWAP, imm12_math, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFPSWAP_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSWAP, imm12_math, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPSWAP, imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPTRANSP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPTRANSP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPTRANSP, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPTRANSP, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFPXOR, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TT_SFPXOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(imm12_math, 12) && \
        ckernel::is_valid(lreg_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPXOR, imm12_math, lreg_c, lreg_dest, instr_mod1)
#define TTI_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFPXOR, imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::make_op(ckernel::OpType::SFP_STOCH_RND, rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TT_SFP_STOCH_RND_VALID(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    (ckernel::is_valid(rnd_mode, 3) && \
        ckernel::is_valid(imm8_math, 5) && \
        ckernel::is_valid(lreg_src_b, 4) && \
        ckernel::is_valid(lreg_src_c, 4) && \
        ckernel::is_valid(lreg_dest, 4) && \
        ckernel::is_valid(instr_mod1, 4))
#define TT_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFP_STOCH_RND, rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)
#define TTI_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
    ckernel::run_op(OpType::SFP_STOCH_RND, rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::SHIFTDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_SHIFTDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(OpSel, 5) && \
        ckernel::is_valid(ResultRegIndex, 6) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::SHIFTDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::SHIFTDMAREG, OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_SHIFTXA(log2_amount2, shift_mode) \
    ckernel::make_op(ckernel::OpType::SHIFTXA, log2_amount2, shift_mode)
#define TT_SHIFTXA_VALID(log2_amount2, shift_mode) \
    (ckernel::is_valid(log2_amount2, 22) && \
        ckernel::is_valid(shift_mode, 2))
#define TT_SHIFTXA(log2_amount2, shift_mode) \
    ckernel::run_op(OpType::SHIFTXA, log2_amount2, shift_mode)
#define TTI_SHIFTXA(log2_amount2, shift_mode) \
    ckernel::run_op(OpType::SHIFTXA, log2_amount2, shift_mode)

#define TT_OP_SHIFTXB(addr_mode, rot_shift, shift_row) \
    ckernel::make_op(ckernel::OpType::SHIFTXB, addr_mode, rot_shift, shift_row)
#define TT_SHIFTXB_VALID(addr_mode, rot_shift, shift_row) \
    (ckernel::is_valid(addr_mode, 9) && \
        ckernel::is_valid(rot_shift, 5) && \
        ckernel::is_valid(shift_row, 10))
#define TT_SHIFTXB(addr_mode, rot_shift, shift_row) \
    ckernel::run_op(OpType::SHIFTXB, addr_mode, rot_shift, shift_row)
#define TTI_SHIFTXB(addr_mode, rot_shift, shift_row) \
    ckernel::run_op(OpType::SHIFTXB, addr_mode, rot_shift, shift_row)

#define TT_OP_STALLWAIT(stall_res, wait_res) \
    ckernel::make_op(ckernel::OpType::STALLWAIT, stall_res, wait_res)
#define TT_STALLWAIT_VALID(stall_res, wait_res) \
    (ckernel::is_valid(stall_res, 9) && \
        ckernel::is_valid(wait_res, 15))
#define TT_STALLWAIT(stall_res, wait_res) \
    ckernel::run_op(OpType::STALLWAIT, stall_res, wait_res)
#define TTI_STALLWAIT(stall_res, wait_res) \
    ckernel::run_op(OpType::STALLWAIT, stall_res, wait_res)

#define TT_OP_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::make_op(ckernel::OpType::STOREIND, MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)
#define TT_STOREIND_VALID(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    (ckernel::is_valid(MemHierSel, 1) && \
        ckernel::is_valid(SizeSel, 1) && \
        ckernel::is_valid(RegSizeSel, 1) && \
        ckernel::is_valid(OffsetIndex, 7) && \
        ckernel::is_valid(AutoIncSpec, 2) && \
        ckernel::is_valid(DataRegIndex, 6) && \
        ckernel::is_valid(AddrRegIndex, 6))
#define TT_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::STOREIND, MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)
#define TTI_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
    ckernel::run_op(OpType::STOREIND, MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_STOREREG(TdmaDataRegIndex, RegAddr) \
    ckernel::make_op(ckernel::OpType::STOREREG, TdmaDataRegIndex, RegAddr)
#define TT_STOREREG_VALID(TdmaDataRegIndex, RegAddr) \
    (ckernel::is_valid(TdmaDataRegIndex, 6) && \
        ckernel::is_valid(RegAddr, 18))
#define TT_STOREREG(TdmaDataRegIndex, RegAddr) \
    ckernel::run_op(OpType::STOREREG, TdmaDataRegIndex, RegAddr)
#define TTI_STOREREG(TdmaDataRegIndex, RegAddr) \
    ckernel::run_op(OpType::STOREREG, TdmaDataRegIndex, RegAddr)

#define TT_OP_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::make_op(ckernel::OpType::SUBDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TT_SUBDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    (ckernel::is_valid(OpBisConst, 1) && \
        ckernel::is_valid(ResultRegIndex, 11) && \
        ckernel::is_valid(OpBRegIndex, 6) && \
        ckernel::is_valid(OpARegIndex, 6))
#define TT_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::SUBDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)
#define TTI_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
    ckernel::run_op(OpType::SUBDMAREG, OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_TBUFCMD \
    ckernel::make_op(ckernel::OpType::TBUFCMD)
#define TTI_TBUFCMD \
    ckernel::run_op(OpType::TBUFCMD)

#define TT_OP_TRNSPSRCA \
    ckernel::make_op(ckernel::OpType::TRNSPSRCA)
#define TTI_TRNSPSRCA \
    ckernel::run_op(OpType::TRNSPSRCA)

#define TT_OP_TRNSPSRCB \
    ckernel::make_op(ckernel::OpType::TRNSPSRCB)
#define TTI_TRNSPSRCB \
    ckernel::run_op(OpType::TRNSPSRCB)

#define TT_OP_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
    ckernel::make_op(ckernel::OpType::UNPACR, Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last)
#define TT_UNPACR_VALID(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
    (ckernel::is_valid(Unpack_block_selection, 1) && \
        ckernel::is_valid(AddrMode, 8) && \
        ckernel::is_valid(CfgContextCntInc, 2) && \
        ckernel::is_valid(CfgContextId, 3) && \
        ckernel::is_valid(AddrCntContextId, 2) && \
        ckernel::is_valid(OvrdThreadId, 1) && \
        ckernel::is_valid(SetDatValid, 1) && \
        ckernel::is_valid(rareb_en, 1) && \
        ckernel::is_valid(ZeroWrite2, 1) && \
        ckernel::is_valid(AutoIncContextID, 1) && \
        ckernel::is_valid(RowSearch, 1) && \
        ckernel::is_valid(SearchCacheFlush, 1) && \
        ckernel::is_valid(Last, 1))
#define TT_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
    ckernel::run_op(OpType::UNPACR, Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last)
#define TTI_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
    ckernel::run_op(OpType::UNPACR, Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last)

#define TT_OP_UNPACR_NOP(Unpack_block_selection, NoOp) \
    ckernel::make_op(ckernel::OpType::UNPACR_NOP, Unpack_block_selection, NoOp)
#define TT_UNPACR_NOP_VALID(Unpack_block_selection, NoOp) \
    (ckernel::is_valid(Unpack_block_selection, 1) && \
        ckernel::is_valid(NoOp, 23))
#define TT_UNPACR_NOP(Unpack_block_selection, NoOp) \
    ckernel::run_op(OpType::UNPACR_NOP, Unpack_block_selection, NoOp)
#define TTI_UNPACR_NOP(Unpack_block_selection, NoOp) \
    ckernel::run_op(OpType::UNPACR_NOP, Unpack_block_selection, NoOp)

#define TT_OP_WRCFG(GprAddress, wr128b, CfgReg) \
    ckernel::make_op(ckernel::OpType::WRCFG, GprAddress, wr128b, CfgReg)
#define TT_WRCFG_VALID(GprAddress, wr128b, CfgReg) \
    (ckernel::is_valid(GprAddress, 8) && \
        ckernel::is_valid(wr128b, 1) && \
        ckernel::is_valid(CfgReg, 15))
#define TT_WRCFG(GprAddress, wr128b, CfgReg) \
    ckernel::run_op(OpType::WRCFG, GprAddress, wr128b, CfgReg)
#define TTI_WRCFG(GprAddress, wr128b, CfgReg) \
    ckernel::run_op(OpType::WRCFG, GprAddress, wr128b, CfgReg)

#define TT_OP_XMOV(Mov_block_selection, Last) \
    ckernel::make_op(ckernel::OpType::XMOV, Mov_block_selection, Last)
#define TT_XMOV_VALID(Mov_block_selection, Last) \
    (ckernel::is_valid(Mov block selection, 1) && \
        ckernel::is_valid(Last, 23))
#define TT_XMOV(Mov_block_selection, Last) \
    ckernel::run_op(OpType::XMOV, Mov_block_selection, Last)
#define TTI_XMOV(Mov_block_selection, Last) \
    ckernel::run_op(OpType::XMOV, Mov_block_selection, Last)

#define TT_OP_ZEROACC(clear_mode, AddrMode, dst) \
    ckernel::make_op(ckernel::OpType::ZEROACC, clear_mode, AddrMode, dst)
#define TT_ZEROACC_VALID(clear_mode, AddrMode, dst) \
    (ckernel::is_valid(clear_mode, 5) && \
        ckernel::is_valid(AddrMode, 4) && \
        ckernel::is_valid(dst, 15))
#define TT_ZEROACC(clear_mode, AddrMode, dst) \
    ckernel::run_op(OpType::ZEROACC, clear_mode, AddrMode, dst)
#define TTI_ZEROACC(clear_mode, AddrMode, dst) \
    ckernel::run_op(OpType::ZEROACC, clear_mode, AddrMode, dst)

#define TT_OP_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
    ckernel::make_op(ckernel::OpType::ZEROSRC, zero_val, write_mode, bank_mask, src_mask)
#define TT_ZEROSRC_VALID(zero_val, write_mode, bank_mask, src_mask) \
    (ckernel::is_valid(zero_val, 20) && \
        ckernel::is_valid(write_mode, 1) && \
        ckernel::is_valid(bank_mask, 1) && \
        ckernel::is_valid(src_mask, 2))
#define TT_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
    ckernel::run_op(OpType::ZEROSRC, zero_val, write_mode, bank_mask, src_mask)
#define TTI_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
    ckernel::run_op(OpType::ZEROSRC, zero_val, write_mode, bank_mask, src_mask)

