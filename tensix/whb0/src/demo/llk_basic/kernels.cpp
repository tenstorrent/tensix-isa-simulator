// SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>

#include "core/tensix_types.hpp"

#include "llk/basic/defs.hpp"
#include "llk/basic/common.hpp"
#include "llk/basic/unpack.hpp"
#include "llk/basic/math.hpp"

#include "demo/llk_basic/sfpu_test.hpp"
#include "demo/llk_basic/machine.hpp"
#include "demo/llk_basic/kernels.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_basic {

using core::DataFormat;

using llk::basic::DstSync;
using llk::basic::DstMode;
using llk::basic::EltwiseBinaryType;
using llk::basic::BroadcastType;
using llk::basic::PoolType;
using llk::basic::ReduceDim;
using llk::basic::ThreadParams;
using llk::basic::Unpack;
using llk::basic::Math;

namespace {

// thread parameters

ThreadParams g_default_params{0, DstSync::SyncHalf, true};

// memory layout

static constexpr int32_t BASE_ADDR = 512 * 1024;
static constexpr uint32_t IN0_ADDR = BASE_ADDR;
static constexpr uint32_t IN1_ADDR = BASE_ADDR + 1024 * 4;
static constexpr uint32_t OUT_ADDR = BASE_ADDR + 1024 * 8;

static constexpr uint32_t IN0_ADDR_16B = IN0_ADDR / 16;
static constexpr uint32_t IN1_ADDR_16B = IN1_ADDR / 16;
static constexpr uint32_t OUT_ADDR_16B = OUT_ADDR / 16;

static constexpr uint32_t IN0_ADDR_4x4 = BASE_ADDR;
static constexpr uint32_t IN1_ADDR_4x4 = BASE_ADDR + 1024 * 4 * 16;
static constexpr uint32_t OUT_ADDR_4x4 = BASE_ADDR + 1024 * 8 * 16;

static constexpr uint32_t IN0_ADDR_4x4_16B = IN0_ADDR_4x4 / 16;
static constexpr uint32_t IN1_ADDR_4x4_16B = IN1_ADDR_4x4 / 16;
static constexpr uint32_t OUT_ADDR_4x4_16B = OUT_ADDR_4x4 / 16;

// memory initialization

void init_const(Machine *machine, uint32_t addr, float value) {
    float *ptr = reinterpret_cast<float *>(machine->map_addr(addr));
    for (int i = 0; i < 1024; i++) {
        ptr[i] = value;
    }
}

std::function<void (Machine *)> make_init_const(uint32_t addr, float value) {
    return [addr, value](Machine *machine) {
        init_const(machine, addr, value);
    };
}

void init_const_faces(
        Machine *machine, 
        uint32_t addr, 
        float value0,
        float value1,
        float value2,
        float value3) {
    float *ptr = reinterpret_cast<float *>(machine->map_addr(addr));
    for (int i = 0; i < 256; i++) {
        ptr[i] = value0;
    }
    for (int i = 256; i < 512; i++) {
        ptr[i] = value1;
    }
    for (int i = 512; i < 768; i++) {
        ptr[i] = value2;
    }
    for (int i = 768; i < 1024; i++) {
        ptr[i] = value3;
    }
}

std::function<void (Machine *)> make_init_const_faces(
        uint32_t addr, 
        float value0,
        float value1,
        float value2,
        float value3) {
    return [addr, value0, value1, value2, value3](Machine *machine) {
        init_const_faces(machine, addr, value0, value1, value2, value3);
    };
}

void init_range(
        Machine *machine, 
        uint32_t addr,
        uint32_t count,
        float start,
        float step) {
    float *ptr = reinterpret_cast<float *>(machine->map_addr(addr));
    for (uint32_t i = 0; i < count; i++) {
        ptr[i] = start + i * step;
    }
}

std::function<void (Machine *)> make_init_range(
        uint32_t addr, 
        uint32_t count,
        float start,
        float step) {
    return [addr, count, start, step](Machine *machine) {
        init_range(machine, addr, count, start, step);
    };
}

void init_range_rows(
        Machine *machine, 
        uint32_t addr, 
        uint32_t count,
        float start,
        float step) {
    float *ptr = reinterpret_cast<float *>(machine->map_addr(addr));
    float value = start;
    for (uint32_t i = 0; i < count; i++) {
        for (uint32_t k = 0; k < 16; k++) {
            ptr[i * 16 + k] = value;
        }
        value += step;
    }
}

std::function<void (Machine *)> make_init_range_rows(
        uint32_t addr, 
        uint32_t count,
        float start,
        float step) {
    return [addr, count, start, step](Machine *machine) {
        init_range_rows(machine, addr, count, start, step);
    };
}

// fetching result

void get_result(Machine *machine, uint32_t addr, uint32_t size, std::vector<float> &result) {
    result.resize(size);
    const float *ptr = reinterpret_cast<float *>(machine->map_addr(addr));
    for (uint32_t i = 0; i < size; i++) {
        result[i] = ptr[i];
    }
}

std::function<void (Machine *, std::vector<float> &)> 
        make_get_result(uint32_t addr, uint32_t size) {
    return [addr, size](Machine *machine, std::vector<float> &result) {
        get_result(machine, addr, size, result);
    };
}

// binary_add

template<typename T>
void binary_add_main(T *thread) {
    thread->binary_op_init_common(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->binary_op_specific_init(0);
    thread->acquire_dst(DstMode::Half); 
    thread->add_tiles(IN0_ADDR_16B, IN1_ADDR_16B, 1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// bcast_h_add

template<typename T>
void bcast_h_add_main(T *thread) {
    thread->init_bcast(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::ROW,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->add_tiles_bcast(BroadcastType::ROW, IN0_ADDR_16B, IN1_ADDR_16B, 1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// bcast_w_add

template<typename T>
void bcast_w_add_main(T *thread) {
    thread->init_bcast(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::COL,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->add_tiles_bcast(BroadcastType::COL, IN0_ADDR_16B, IN1_ADDR_16B, 1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// bcast_hw_add

template<typename T>
void bcast_hw_add_main(T *thread) {
    thread->init_bcast(
        EltwiseBinaryType::ELWADD, 
        BroadcastType::SCALAR,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->add_tiles_bcast(BroadcastType::SCALAR, IN0_ADDR_16B, IN1_ADDR_16B, 1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// matmul

template<typename T>
void matmul_main(T *thread) {
    thread->mm_init(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->matmul_tiles(IN0_ADDR_16B, IN1_ADDR_16B, 1, false);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// reduce_h_sum

template<typename T>
void reduce_h_sum_main(T *thread) {
    thread->reduce_init_v2(
        true, 
        PoolType::SUM, 
        ReduceDim::REDUCE_COL, 
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->reduce_tile_v2(
        PoolType::SUM, 
        ReduceDim::REDUCE_COL, 
        IN0_ADDR_16B, 
        IN1_ADDR_16B, 
        1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// reduce_w_sum

template<typename T>
void reduce_w_sum_main(T *thread) {
    thread->reduce_init_v2(
        true, 
        PoolType::SUM, 
        ReduceDim::REDUCE_ROW, 
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->reduce_tile_v2(
        PoolType::SUM, 
        ReduceDim::REDUCE_ROW, 
        IN0_ADDR_16B, 
        IN1_ADDR_16B, 
        1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// reduce_w_sum

template<typename T>
void reduce_hw_sum_main(T *thread) {
    thread->reduce_init_v2(
        true, 
        PoolType::SUM, 
        ReduceDim::REDUCE_SCALAR, 
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->reduce_tile_v2(
        PoolType::SUM, 
        ReduceDim::REDUCE_SCALAR, 
        IN0_ADDR_16B, 
        IN1_ADDR_16B, 
        1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_exp

template<typename T>
void sfpu_exp_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->exp_tile_init();
    thread->exp_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_recip

template<typename T>
void sfpu_recip_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->recip_tile_init();
    thread->recip_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_gelu

template<typename T>
void sfpu_gelu_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->gelu_tile_init();
    thread->gelu_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_sqrt

template<typename T>
void sfpu_sqrt_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->sqrt_tile_init();
    thread->sqrt_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_sigmoid

template<typename T>
void sfpu_sigmoid_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->sigmoid_tile_init();
    thread->sigmoid_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_log

template<typename T>
void sfpu_log_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->log_tile_init();
    thread->log_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// sfpu_tanh

template<typename T>
void sfpu_tanh_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 1);
    thread->tanh_tile_init();
    thread->tanh_tile(1);
    thread->pack_tile(1, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// tilize

template<typename T>
void tilize_main(T *thread) {
    uint32_t in_addr = IN0_ADDR_4x4_16B;
    uint32_t out_addr = OUT_ADDR_4x4_16B;
    uint32_t block_count = 4;
    uint32_t tile_count = 4;
    uint32_t tile_size_16b = (1024 * 4) >> 4;
    thread->tilize_init(
        DataFormat::Float32, 
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        tile_count);
    for (uint32_t i = 0; i < block_count; i++) {
        thread->tilize_block(
            DataFormat::Float32, 
            DataFormat::Float32,
            in_addr, 
            tile_count, 
            DataFormat::Float32, 
            DataFormat::Float32,
            out_addr); 
        in_addr += tile_count * tile_size_16b;
        out_addr += tile_count * tile_size_16b;
    }
}

// untilize

template<typename T>
void untilize_main(T *thread) {
    uint32_t in_addr = IN0_ADDR_4x4_16B;
    uint32_t out_addr = OUT_ADDR_4x4_16B;
    uint32_t block_count = 4;
    uint32_t tile_count = 4;
    uint32_t tile_size_16b = (1024 * 4) >> 4;
    thread->untilize_init(
        DataFormat::Float32, 
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    for (uint32_t i = 0; i < block_count; i++) {
        thread->untilize_block(
            in_addr, 
            tile_count, 
            DataFormat::Float32, 
            DataFormat::Float32,
            out_addr); 
        in_addr += tile_count * tile_size_16b;
        out_addr += tile_count * tile_size_16b;
    }
}

// sfpu_test

template<typename T> 
void run_sfpu_test(T *thread, int n);

template<> 
void run_sfpu_test(Unpack *thread, int n) { }

template<> 
void run_sfpu_test(Pack *thread, int n) { }

template<> 
void run_sfpu_test(Math *thread, int n) {
    SfpuTest test(thread);
    test.test(n);
}

template<typename T, int N>
void sfpu_test_main(T *thread) {
    thread->init_sfpu(
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32,
        DataFormat::Float32);
    thread->acquire_dst(DstMode::Half); 
    thread->copy_tile(IN0_ADDR_16B, 0);
    run_sfpu_test(thread, N);
    thread->pack_tile(0, OUT_ADDR_16B); 
    thread->release_dst(DstMode::Half); 
}

// kernel map

std::unordered_map<std::string, KernelSpec> g_kernel_map = {
    {
        "binary_add", 
        {
            &g_default_params,
            binary_add_main<Unpack>,
            binary_add_main<Math>,
            binary_add_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_h_add", 
        {
            &g_default_params,
            bcast_h_add_main<Unpack>,
            bcast_h_add_main<Math>,
            bcast_h_add_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_w_add", 
        {
            &g_default_params,
            bcast_w_add_main<Unpack>,
            bcast_w_add_main<Math>,
            bcast_w_add_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_hw_add", 
        {
            &g_default_params,
            bcast_hw_add_main<Unpack>,
            bcast_hw_add_main<Math>,
            bcast_hw_add_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "matmul", 
        {
            &g_default_params,
            matmul_main<Unpack>,
            matmul_main<Math>,
            matmul_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_h_sum", 
        {
            &g_default_params,
            reduce_h_sum_main<Unpack>,
            reduce_h_sum_main<Math>,
            reduce_h_sum_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
//            make_init_const_faces(IN1_ADDR, 0.1f, 0.2f, 0.3f, 0.4f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_w_sum", 
        {
            &g_default_params,
            reduce_w_sum_main<Unpack>,
            reduce_w_sum_main<Math>,
            reduce_w_sum_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
//            make_init_const_faces(IN1_ADDR, 0.1f, 0.2f, 0.3f, 0.4f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_hw_sum", 
        {
            &g_default_params,
            reduce_hw_sum_main<Unpack>,
            reduce_hw_sum_main<Math>,
            reduce_hw_sum_main<Pack>,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
//            make_init_const_faces(IN1_ADDR, 0.1f, 0.2f, 0.3f, 0.4f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_exp",
        {
            &g_default_params,
            sfpu_exp_main<Unpack>,
            sfpu_exp_main<Math>,
            sfpu_exp_main<Pack>,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_recip",
        {
            &g_default_params,
            sfpu_recip_main<Unpack>,
            sfpu_recip_main<Math>,
            sfpu_recip_main<Pack>,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.5f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_gelu",
        {
            &g_default_params,
            sfpu_gelu_main<Unpack>,
            sfpu_gelu_main<Math>,
            sfpu_gelu_main<Pack>,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sqrt",
        {
            &g_default_params,
            sfpu_sqrt_main<Unpack>,
            sfpu_sqrt_main<Math>,
            sfpu_sqrt_main<Pack>,
            make_init_const_faces(IN0_ADDR, 0.0f, 1.0f, 2.0f, 4.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sigmoid",
        {
            &g_default_params,
            sfpu_sigmoid_main<Unpack>,
            sfpu_sigmoid_main<Math>,
            sfpu_sigmoid_main<Pack>,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_log",
        {
            &g_default_params,
            sfpu_log_main<Unpack>,
            sfpu_log_main<Math>,
            sfpu_log_main<Pack>,
            make_init_const_faces(IN0_ADDR, 0.5f, 1.0f, 2.0f, 0.5f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_tanh",
        {
            &g_default_params,
            sfpu_tanh_main<Unpack>,
            sfpu_tanh_main<Math>,
            sfpu_tanh_main<Pack>,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "tilize",
        {
            &g_default_params,
            tilize_main<Unpack>,
            tilize_main<Math>,
            tilize_main<Pack>,
            make_init_range_rows(IN0_ADDR_4x4, 8 * 8 * 16, 1.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR_4x4, 1024 * 4 * 4),
            4 * 4
        }
    },
    {
        "untilize",
        {
            &g_default_params,
            untilize_main<Unpack>,
            untilize_main<Math>,
            untilize_main<Pack>,
            make_init_range_rows(IN0_ADDR_4x4, 8 * 8 * 16, 1.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR_4x4, 1024 * 4 * 4),
            4 * 4
        }
    },
    {
        "sfpu_test1",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 1>,
            sfpu_test_main<Math, 1>,
            sfpu_test_main<Pack, 1>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test2",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 2>,
            sfpu_test_main<Math, 2>,
            sfpu_test_main<Pack, 2>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test3",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 3>,
            sfpu_test_main<Math, 3>,
            sfpu_test_main<Pack, 3>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test4",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 4>,
            sfpu_test_main<Math, 4>,
            sfpu_test_main<Pack, 4>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test5",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 5>,
            sfpu_test_main<Math, 5>,
            sfpu_test_main<Pack, 5>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test6",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 6>,
            sfpu_test_main<Math, 6>,
            sfpu_test_main<Pack, 6>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test7",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 7>,
            sfpu_test_main<Math, 7>,
            sfpu_test_main<Pack, 7>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test8",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 8>,
            sfpu_test_main<Math, 8>,
            sfpu_test_main<Pack, 8>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test9",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 9>,
            sfpu_test_main<Math, 9>,
            sfpu_test_main<Pack, 9>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test10",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 10>,
            sfpu_test_main<Math, 10>,
            sfpu_test_main<Pack, 10>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test11",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 11>,
            sfpu_test_main<Math, 11>,
            sfpu_test_main<Pack, 11>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test12",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 12>,
            sfpu_test_main<Math, 12>,
            sfpu_test_main<Pack, 12>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test13",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 13>,
            sfpu_test_main<Math, 13>,
            sfpu_test_main<Pack, 13>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test14",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 14>,
            sfpu_test_main<Math, 14>,
            sfpu_test_main<Pack, 14>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test15",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 15>,
            sfpu_test_main<Math, 15>,
            sfpu_test_main<Pack, 15>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test16",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 16>,
            sfpu_test_main<Math, 16>,
            sfpu_test_main<Pack, 16>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_test17",
        {
            &g_default_params,
            sfpu_test_main<Unpack, 17>,
            sfpu_test_main<Math, 17>,
            sfpu_test_main<Pack, 17>,
            make_init_range(IN0_ADDR, 1024, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    }
};

} // namespace

// public functions

KernelSpec *map_kernel(const std::string &name) {
    auto it = g_kernel_map.find(name);
    return (it != g_kernel_map.end()) ? &it->second : nullptr;
}

} // namespace llk_basic
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

