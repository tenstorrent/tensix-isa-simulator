// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "machine.hpp"
#include "kernels_unpack.hpp"
#include "kernels_math.hpp"
#include "kernels_pack.hpp"
#include "kernels.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {

namespace {

// memory layout

// each stream contains two frames of 16 FP32 tiles each

static constexpr int32_t BASE_ADDR = 512 * 1024;

static constexpr uint32_t IN0_ADDR = BASE_ADDR;
static constexpr uint32_t IN1_ADDR = BASE_ADDR + 2 * 1024 * 4 * 16;
static constexpr uint32_t OUT_ADDR = BASE_ADDR + 2 * 1024 * 8 * 16;

static constexpr uint32_t IN0_ADDR_16B = IN0_ADDR / 16;
static constexpr uint32_t IN1_ADDR_16B = IN1_ADDR / 16;
static constexpr uint32_t OUT_ADDR_16B = OUT_ADDR / 16;

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

// kernel map

std::unordered_map<std::string, KernelSpec> g_kernel_map = {
    {
        "binary_add", 
        {
            unpack::binary_add_main,
            math::binary_add_main,
            pack::binary_add_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_h_add", 
        {
            unpack::bcast_h_add_main,
            math::bcast_h_add_main,
            pack::bcast_h_add_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_w_add", 
        {
            unpack::bcast_w_add_main,
            math::bcast_w_add_main,
            pack::bcast_w_add_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "bcast_hw_add", 
        {
            unpack::bcast_hw_add_main,
            math::bcast_hw_add_main,
            pack::bcast_hw_add_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "matmul", 
        {
            unpack::matmul_main,
            math::matmul_main,
            pack::matmul_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const_faces(IN1_ADDR, 100.0f, 200.0f, 300.0f, 400.0f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_h_sum", 
        {
            unpack::reduce_h_sum_main,
            math::reduce_h_sum_main,
            pack::reduce_h_sum_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_w_sum", 
        {
            unpack::reduce_w_sum_main,
            math::reduce_w_sum_main,
            pack::reduce_w_sum_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "reduce_hw_sum", 
        {
            unpack::reduce_hw_sum_main,
            math::reduce_hw_sum_main,
            pack::reduce_hw_sum_main,
            make_init_const_faces(IN0_ADDR, 10.0f, 20.0f, 30.0f, 40.0f),
            make_init_const(IN1_ADDR, 0.1f),
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "tilize",
        {
            unpack::tilize_main,
            math::tilize_main,
            pack::tilize_main,
            make_init_range_rows(IN0_ADDR, 8 * 8 * 16, 1.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024 * 4 * 4),
            4 * 4
        }
    },
    {
        "untilize",
        {
            unpack::untilize_main,
            math::untilize_main,
            pack::untilize_main,
            make_init_range_rows(IN0_ADDR, 8 * 8 * 16, 1.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024 * 4 * 4),
            4 * 4
        }
    },
    // SFPU kernels
    {
        "sfpu_tanh",
        {
            unpack::sfpu_tanh_main,
            math::sfpu_tanh_main,
            pack::sfpu_tanh_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_gelu",
        {
            unpack::sfpu_gelu_main,
            math::sfpu_gelu_main,
            pack::sfpu_gelu_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_exp",
        {
            unpack::sfpu_exponential_main,
            math::sfpu_exponential_main,
            pack::sfpu_exponential_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sigmoid",
        {
            unpack::sfpu_sigmoid_main,
            math::sfpu_sigmoid_main,
            pack::sfpu_sigmoid_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_recip",
        {
            unpack::sfpu_reciprocal_main,
            math::sfpu_reciprocal_main,
            pack::sfpu_reciprocal_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sqrt",
        {
            unpack::sfpu_sqrt_main,
            math::sfpu_sqrt_main,
            pack::sfpu_sqrt_main,
            make_init_const_faces(IN0_ADDR, 0.0f, 1.0f, 2.0f, 4.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_lrelu",
        {
            unpack::sfpu_lrelu_main,
            math::sfpu_lrelu_main,
            pack::sfpu_lrelu_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_power",
        {
            unpack::sfpu_power_main,
            math::sfpu_power_main,
            pack::sfpu_power_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_square",
        {
            unpack::sfpu_square_main,
            math::sfpu_square_main,
            pack::sfpu_square_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_log",
        {
            unpack::sfpu_log_main,
            math::sfpu_log_main,
            pack::sfpu_log_main,
            make_init_const_faces(IN0_ADDR, 1.0f, 10.0f, 100.0f, 1000.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_log_with_base",
        {
            unpack::sfpu_log_with_base_main,
            math::sfpu_log_with_base_main,
            pack::sfpu_log_with_base_main,
            make_init_const_faces(IN0_ADDR, 1.0f, 10.0f, 100.0f, 1000.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_eqz",
        {
            unpack::sfpu_equal_zero_main,
            math::sfpu_equal_zero_main,
            pack::sfpu_equal_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_nez",
        {
            unpack::sfpu_not_equal_zero_main,
            math::sfpu_not_equal_zero_main,
            pack::sfpu_not_equal_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_ltz",
        {
            unpack::sfpu_less_than_zero_main,
            math::sfpu_less_than_zero_main,
            pack::sfpu_less_than_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_gez",
        {
            unpack::sfpu_greater_than_equal_zero_main,
            math::sfpu_greater_than_equal_zero_main,
            pack::sfpu_greater_than_equal_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_lez",
        {
            unpack::sfpu_less_than_equal_zero_main,
            math::sfpu_less_than_equal_zero_main,
            pack::sfpu_less_than_equal_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_gtz",
        {
            unpack::sfpu_greater_than_zero_main,
            math::sfpu_greater_than_zero_main,
            pack::sfpu_greater_than_zero_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_abs",
        {
            unpack::sfpu_abs_main,
            math::sfpu_abs_main,
            pack::sfpu_abs_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sign",
        {
            unpack::sfpu_sign_main,
            math::sfpu_sign_main,
            pack::sfpu_sign_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_sin",
        {
            unpack::sfpu_sine_main,
            math::sfpu_sine_main,
            pack::sfpu_sine_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_cos",
        {
            unpack::sfpu_cosine_main,
            math::sfpu_cosine_main,
            pack::sfpu_cosine_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_tan",
        {
            unpack::sfpu_tan_main,
            math::sfpu_tan_main,
            pack::sfpu_tan_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_relu_max",
        {
            unpack::sfpu_relu_max_main,
            math::sfpu_relu_max_main,
            pack::sfpu_relu_max_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_relu_min",
        {
            unpack::sfpu_relu_min_main,
            math::sfpu_relu_min_main,
            pack::sfpu_relu_min_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_elu",
        {
            unpack::sfpu_elu_main,
            math::sfpu_elu_main,
            pack::sfpu_elu_main,
            make_init_const_faces(IN0_ADDR, -2.0f, -1.0f, 0.0f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_heaviside",
        {
            unpack::sfpu_heaviside_main,
            math::sfpu_heaviside_main,
            pack::sfpu_heaviside_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_expm1",
        {
            unpack::sfpu_expm1_main,
            math::sfpu_expm1_main,
            pack::sfpu_expm1_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_signbit",
        {
            unpack::sfpu_signbit_main,
            math::sfpu_signbit_main,
            pack::sfpu_signbit_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_asin",
        {
            unpack::sfpu_asin_main,
            math::sfpu_asin_main,
            pack::sfpu_asin_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 0.5f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_acos",
        {
            unpack::sfpu_acos_main,
            math::sfpu_acos_main,
            pack::sfpu_acos_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 0.5f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_atan",
        {
            unpack::sfpu_atan_main,
            math::sfpu_atan_main,
            pack::sfpu_atan_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 0.5f, 1.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_erf",
        {
            unpack::sfpu_erf_main,
            math::sfpu_erf_main,
            pack::sfpu_erf_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_erfc",
        {
            unpack::sfpu_erfc_main,
            math::sfpu_erfc_main,
            pack::sfpu_erfc_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_rsqrt",
        {
            unpack::sfpu_rsqrt_main,
            math::sfpu_rsqrt_main,
            pack::sfpu_rsqrt_main,
            make_init_const_faces(IN0_ADDR, 0.25f, 1.0f, 2.0f, 4.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    // SKIPPED: isinf / isnan
    {
        "sfpu_logical_not_unary",
        {
            unpack::sfpu_logical_not_unary_main,
            math::sfpu_logical_not_unary_main,
            pack::sfpu_logical_not_unary_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_erfinv",
        {
            unpack::sfpu_erfinv_main,
            math::sfpu_erfinv_main,
            pack::sfpu_erfinv_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
            nullptr,
            make_get_result(OUT_ADDR, 1024),
            1
        }
    },
    {
        "sfpu_i0",
        {
            unpack::sfpu_i0_main,
            math::sfpu_i0_main,
            pack::sfpu_i0_main,
            make_init_const_faces(IN0_ADDR, -1.0f, 0.0f, 1.0f, 2.0f),
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

void setup_cb(Machine *machine) {
    uint32_t fifo_num_pages = 32;  // number of tiles
    uint32_t fifo_page_size = 256; // tile size in 16B units
    uint32_t fifo_size = fifo_num_pages * fifo_page_size;
    machine->setup_cb(0, IN0_ADDR_16B, fifo_size, fifo_num_pages, fifo_page_size);
    machine->setup_cb(1, IN1_ADDR_16B, fifo_size, fifo_num_pages, fifo_page_size);
    machine->setup_cb(16, OUT_ADDR_16B, fifo_size, fifo_num_pages, fifo_page_size);
    machine->cb_push_back(0, 4 * 4);
    machine->cb_push_back(1, 4 * 4);
}

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

