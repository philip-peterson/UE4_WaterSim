LDFLAGS="-L/usr/local/opt/openblas/lib -L/usr/local/lib -L/Users/ironmagma/Code/UE4_WaterSim/Experiment -lblas"

CPPFLAGS="-I/usr/local/lib/ -I/usr/local/Cellar/lapack/3.5.0/include/ -I/usr/local/opt/openblas/include"


set -v
clang++ math.cpp -std=c++11 $CPPFLAGS $LDFLAGS
