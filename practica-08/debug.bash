#!/bin/bash
# bash debug.bash

mkdir -p Data
file="Data/debug.csv"
cd Data
touch debug.csv
cd ..

n=$[100000000]
p=$[32]
make clean

make e_serial e_omp e_mpi
echo
echo "---------------SERIAL---------------"
echo
make run_e_serial N=$n FILE_NAME=$file
echo
echo "---------------OMP---------------"
echo
make run_e_omp N=$n P=$p FILE_NAME=$file
echo
echo "---------------MPI---------------"
echo
make run_e_mpi N=$n P=$p FILE_NAME=$file
make clean

