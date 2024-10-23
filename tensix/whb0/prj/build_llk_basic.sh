#!/bin/bash

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 -I $SRC $SRC/llk/basic/*.cpp

mkdir -p $LIB

ar rsc $LIB/llk_basic.a *.o

rm *.o


