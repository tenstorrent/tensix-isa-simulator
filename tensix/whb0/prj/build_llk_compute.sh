#!/bin/bash

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 \
    -I $SRC \
    -I $SRC/system \
    -I $SRC/llk \
    -I $SRC/llk/api \
    -I $SRC/llk/ref \
    $SRC/llk/compute/*.cpp

mkdir -p $LIB

ar rsc $LIB/llk_compute.a *.o

rm *.o


