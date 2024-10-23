#!/bin/bash

NAME=core

SRC=../src
LIB=../lib

g++ -c -std=c++17 -O3 -I $SRC $SRC/$NAME/*.cpp

mkdir -p $LIB

ar rsc $LIB/$NAME.a *.o

rm *.o


