#!/bin/bash
# bash debug.bash
n=$[100000000]
p=$[8]
make clean

make e_serial e_omp e_mpi
echo
echo "---------------SERIAL---------------"
echo
make run_e_serial N=$n
echo
echo "---------------OMP---------------"
echo
make run_e_omp N=$n P=$p
echo
echo "---------------MPI---------------"
echo
make run_e_mpi N=$n P=$p
make clean

