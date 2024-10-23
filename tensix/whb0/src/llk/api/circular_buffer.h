#pragma once

#include <cstdint>

namespace ronin {
namespace iss {
namespace whb0 {
namespace llk {
namespace api {

struct CQReadInterface {
    uint32_t fifo_size;
    uint32_t fifo_limit;
    uint32_t fifo_rd_ptr;
    uint32_t fifo_rd_toggle;
};

struct CBInterface {
    uint32_t fifo_size;
    uint32_t fifo_limit;
    uint32_t fifo_page_size;
    uint32_t fifo_num_pages;

    uint32_t fifo_rd_ptr;
    uint32_t fifo_wr_ptr;

    uint32_t tiles_acked;
    uint32_t tiles_received;

    // used by packer for in-order packing
    uint32_t fifo_wr_tile_ptr;
};

} // namespace api
} // namespace llk
} // namespace whb0
} // namespace iss
} // namespace ronin

