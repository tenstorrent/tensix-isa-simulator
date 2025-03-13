#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

NAME=sfpi

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 -I $SRC $SRC/$NAME/*.cpp

mkdir -p $LIB

ar rsc $LIB/$NAME.a *.o

rm *.o


