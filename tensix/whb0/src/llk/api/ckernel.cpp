
#include <cstdint>

#include "ckernel_mem_ptr.h"
#include "ckernel.h"

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

MemPtr regfile = MemPtr::map(REGFILE_BASE);
MemPtr pc_buf_base = MemPtr::map(PC_BUF_BASE);

// never accessed
uint32_t *instrn_buffer = nullptr; 

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

