#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 \
    -I $SRC \
    -I $SRC/system \
    -I $SRC/llk/api \
    -I $SRC/llk/ref \
    $SRC/llk/api/*.cpp

mkdir -p $LIB

ar rsc $LIB/llk_ref.a *.o

rm *.o


