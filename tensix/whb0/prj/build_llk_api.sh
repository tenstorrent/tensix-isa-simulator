#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 -I $SRC -I $SRC/llk/api $SRC/llk/api/*.cpp

mkdir -p $LIB

ar rsc $LIB/llk_api.a *.o

rm *.o


