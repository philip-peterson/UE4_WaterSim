#!/usr/bin/env bash
LDFLAGS="-lcblas"

CPPFLAGS="-I/usr/local/Cellar/lapack/3.5.0/include/"


set -v
clang++ math.cpp -std=c++11 $CPPFLAGS $LDFLAGS
