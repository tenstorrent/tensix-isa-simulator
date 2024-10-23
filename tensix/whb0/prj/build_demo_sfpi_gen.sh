#!/bin/bash

SRC=../src
LIB=../lib
BIN=../bin

mkdir -p $BIN

g++ -o $BIN/demo_sfpi_gen -std=c++17 -O3 \
    -I $SRC \
    $SRC/demo/sfpi_gen/*.cpp \
    $LIB/sfpi.a


