#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

SRC=../src
LIB=../lib
BIN=../bin

mkdir -p $BIN

g++ -o $BIN/demo_llk_r37 -std=c++17 -O3 \
    -I $SRC \
    -I $SRC/r37 \
    -I $SRC/r37/iss \
    -I $SRC/r37/common/inc \
    -I $SRC/r37/llk_lib \
    -I $SRC/r37/etc \
    -I $SRC/r37/etc/defines \
    $SRC/demo/llk_r37/*.cpp \
    $LIB/r37_iss.a \
    $LIB/core.a \
    $LIB/schedule.a \
    $LIB/sfpi.a


