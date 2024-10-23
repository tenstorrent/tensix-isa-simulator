#!/bin/bash

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 \
    -I $SRC \
    -I $SRC/r37 \
    -I $SRC/r37/iss \
    -I $SRC/r37/common/inc \
    -I $SRC/r37/llk_lib \
    -I $SRC/r37/etc \
    -I $SRC/r37/etc/defines \
    $SRC/r37/compute/*.cpp

mkdir -p $LIB

ar rsc $LIB/r37_compute.a *.o

rm *.o


