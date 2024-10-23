
# Tensix ISA simulator for Wormhole B0 architecture

This section contains the source code of Tensix ISA simulator
for the Wormhole B0 architecture.

Development of the simulator has two main goals:

* Provide a foundation for a convenient toolchain aiming at designing, 
debugging, and testing of Tensix low-level kernels (LLK) on the conventional CPUs 
without using the real Tenstorrent hardware.

* Provide a verifiable formal functional specification of Tensix ISA which can serve
as a basis for the detailed architecture documentation.

The simulator shall be considered work in progress.

## Code structure

The simulator source code is contained in the subdirectory `src` and structured
in these modules:

```
core                 core functionality
demo                 demo applications
    llk_basic            demo kernels for basic LLKs
    llk_ref              demo kernels for reference LLKs
    sfpi_gen             SFPU code generation using SFPI simulator
llk                  example LLK implementations 
    api                  TT-Metal ckernels API
    basic                basic LLKs directly using core functionality
    compute              TT-Metal Compute API based on reference LLKs
    ref                  TT-Metal reference LLKs
schedule             scheduler managing concurrency and synchronization
sfpi                 SFPI simulator
```

The simulator is implemented completely in C++17 programming language. 
This source code has no external dependencies besides the standard C++ libraries.

## Building from source

The shell scripts for building the simulator libraries and examples are contained in
the subdirectory `prj`. They will place the built libraries and example applications
into subdirectories `lib` and `bin` respectively.

To build the libraries and examples, set `prj` as your current directory and run these commands:

```
./build_core.sh
./build_llk_api.sh
./build_llk_basic.sh
./build_llk_compute.sh    # optional: only to test compilation
./build_llk_ref.sh        # optional: only to test compilation
./build_schedule.sh
./build_sfpi.sh

./build_demo_llk_basic.sh
./build_demo_llk_ref.sh
./build_demo_llk_sfpi_gen.sh
```

## Running reference LLK demo application

To run the reference LLK demo application, use the command

```
./bin/demo_llk_ref kernel_name
```

where `kernel_name` is a name of one of the supported demo kernes:

```
binary_add
bcast_h_add
bcast_w_add
bcast_hw_add
matmul
reduce_h_sum
reduce_w_sum
reduce_hw_sum
tilize
untilize

sfpu_tanh
sfpu_exp
sfpu_sigmoid
sfpu_recip
sfpu_sqrt
sfpu_lrelu
sfpu_power
sfpu_square
sfpu_log
sfpu_eqz
sfpu_nez
sfpu_ltz
sfpu_gez
sfpu_gtz
sfpu_abs
sfpu_sign
sfpu_sin
sfpu_cos
sfpu_tan
sfpu_relu_max
sfpu_relu_min
sfpu_elu
sfpu_heaviside
sfpu_expm1
sfpu_asin
sfpu_acos
sfpu_atan
sfpu_erf
sfpu_erfc
sfpu_rsqrt
sfpu_logical_not_unary
sfpu_erfinv
sfpu_i0
```

## Implementation notes

Currently the simulator provides the core functionality modeling the state of 
one Tensix core shared among all threads (class `Tensix`) and the individual
state of each thread (`Thread`).

The simulator models concurrent execution of threads using the light weight
asymmetric coroutines.

The code subdirectory `llk/ref` contains a reference LLK library that uses Tensix ISS
core functionality. This library is based on the TT-Metal 0.34 release. 

