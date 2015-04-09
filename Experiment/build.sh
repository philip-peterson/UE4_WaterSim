#!/usr/bin/env bash
LDFLAGS="-lblas"

CPPFLAGS="-I/usr/local/Cellar/lapack/3.5.0/include/ -I/usr/include/"


set -v
g++ -std=c++11 $CPPFLAGS $LDFLAGS math.cpp 
