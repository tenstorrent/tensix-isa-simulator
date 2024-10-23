#!/bin/bash

SRC=../src
LIB=../lib
BIN=../bin

mkdir -p $BIN

g++ -o $BIN/demo_llk_basic -std=c++17 -O3 \
    -I $SRC \
    $SRC/demo/llk_basic/*.cpp \
    $LIB/llk_basic.a \
    $LIB/core.a \
    $LIB/schedule.a


