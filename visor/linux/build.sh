#!/bin/bash

rm *.o
rm unitTest

export LIBRARY_PATH=~/witFromGithub/CMMPPT/wit/linux/:~/witFromGithub/CMMPPT/mcl/linux:/opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic
export C_INCLUDE_PATH="../../wit/src"


gcc -I$C_INCLUDE_PATH -c -o scoFile.o         ../src/scoFile.cpp
gcc -I$C_INCLUDE_PATH -c -o unitTest.o        ../src/unitTest.cpp
gcc -I$C_INCLUDE_PATH -c -o CoinFinite.o      ../src/CoinFinite.cpp
gcc -I$C_INCLUDE_PATH -c -o visorProblem.o    ../src/visorProblem.cpp

gcc -o unitTest unitTest.o scoFile.o visorProblem.o CoinFinite.o -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm
