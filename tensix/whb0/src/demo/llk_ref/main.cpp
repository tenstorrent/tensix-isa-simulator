
#include <cstdio>
#include <vector>
#include <memory>
#include <exception>

#include "machine.hpp"
#include "kernels.hpp"

using namespace ronin::iss::whb0::demo::llk_ref;

namespace {

void dump_tile(float *data) {
    int count = 1024 / 8;
#if 0
    for (int i = 0; i < 8; i++) {
        printf("[%3d]", i * 8);
        for (int k = 0; k < 8; k++) {
            printf(" %g", data[i * 8 + k]);
        }
        printf("\n");
    }
    printf("...\n");
    for (int i = count - 8; i < count; i++) {
        printf("[%3d]", i * 8);
        for (int k = 0; k < 8; k++) {
            printf(" %g", data[i * 8 + k]);
        }
        printf("\n");
    }
#else
    for (int i = 0; i < count; i++) {
        printf("[%3d]", i * 8);
        for (int k = 0; k < 8; k++) {
            printf(" %g", data[i * 8 + k]);
        }
        printf("\n");
    }
#endif
}

void dump_tiles(float *data, int count) {
    for (int i = 0; i < count; i++) {
        printf("# Tile %d\n", i);
        dump_tile(data + i * 1024);
    }
}

void dump_dest(Machine *machine) {
    float *ptr = machine->dest_row_data(1 * 64);
    dump_tile(ptr);
}

void dump_result(Machine *machine, KernelSpec *kernel_spec) {
    std::vector<float> result;
    kernel_spec->get_out(machine, result);
    dump_tiles(result.data(), kernel_spec->out_tiles);
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: demo kernel_name\n");
        return 1;
    }
    KernelSpec *kernel_spec = map_kernel(argv[1]);
    if (kernel_spec == nullptr) {
        fprintf(stderr, "Invalid kernel name '%s'\n", argv[1]);
        return 1;
    }
    try {
        std::unique_ptr<Machine> machine(new Machine());
        machine->reset_memory();
        setup_cb(machine.get());
        kernel_spec->init_in0(machine.get());
        if (kernel_spec->init_in1 != nullptr) {
            kernel_spec->init_in1(machine.get());
        }
        machine->run(
            kernel_spec->unpack_main,
            kernel_spec->math_main,
            kernel_spec->pack_main);
        dump_result(machine.get(), kernel_spec);
    } catch (std::exception &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
    return 0;
}

