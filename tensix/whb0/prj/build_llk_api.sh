#!/bin/bash

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 -I $SRC -I $SRC/llk/api $SRC/llk/api/*.cpp

mkdir -p $LIB

ar rsc $LIB/llk_api.a *.o

rm *.o


