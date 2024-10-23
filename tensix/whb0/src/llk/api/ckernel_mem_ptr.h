#pragma once

#include <cstdint>

#include "ckernel_ops.h"
#include "ckernel_thread.h"

//
//    Macro TT_MEM_PTR is used to hide target-specific details
//    For silicon target it can be defined as
//
//    volatile uint32_t tt_reg_ptr *
//

#define MEM_PTR_T MemPtr

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

//
//    Elements of RISC-V address map definition (from "src/firmware/riscv/wormhole/tensix.h")
//

// Reads and writes here access the tensix core register set.
// Each register is four bytes, but subword reads are supported through byte enables.
// Register indices and contents are defined in local_regs.yaml.
static constexpr uint32_t REGFILE_BASE = 0xFFE00000; // 0xFFE00000 - 0xFFE3FFFF

// PC buffer is used to pass kernel IDs and paramters from Brisc to Triscs,
// and also as a sync point -- a read from pc buffer + 1 address
// will not return until that thread is idle.
static constexpr uint32_t PC_BUF_BASE = 0xFFE80000; // 0xFFE80000 - 0xFFEBFFFF

// Config registers
static constexpr uint32_t TENSIX_CFG_BASE = 0xFFEF0000; // 0xFFEF0000 - 0xFFF00000

//
//    Wrapper classes for RISC-V address mapping
//

class MemRef {
public:
    MemRef();
    MemRef(const MemRef &other);
    ~MemRef();
public:
    static MemRef map(uint32_t addr);
    MemRef &operator=(const MemRef &other);
    MemRef &operator=(uint32_t value);
    operator uint32_t();
private:
    uint32_t m_addr;
};

class MemPtr {
public:
    MemPtr();
    MemPtr(const MemPtr &other);
    ~MemPtr();
public:
    static MemPtr map(uint32_t addr);
    MemPtr &operator=(const MemPtr &other);
    MemRef operator[](int index);
private:
    uint32_t m_addr;
};

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

