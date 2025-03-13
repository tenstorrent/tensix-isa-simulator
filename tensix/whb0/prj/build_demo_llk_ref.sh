#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

SRC=../src
LIB=../lib
BIN=../bin

mkdir -p $BIN

g++ -o $BIN/demo_llk_ref -std=c++17 -O3 \
    -I $SRC \
    -I $SRC/system \
    -I $SRC/llk \
    -I $SRC/llk/api \
    -I $SRC/llk/ref \
    $SRC/demo/llk_ref/*.cpp \
    $LIB/llk_api.a \
    $LIB/core.a \
    $LIB/schedule.a \
    $LIB/sfpi.a


