#!/bin/bash
# bash experiments.bash

mkdir -p Data
mkdir -p Plots
file="Data/data.csv"
rm -f $file
touch Data/data.csv

# n: 2^20 -> 2^35
# p: 2^1 -> 2^7

make clean
make e_serial e_omp e_mpi

for i in $(seq 20 30)
do
   n=$[2**$i]
   make run_e_serial N=$n FILE_NAME=$file

   for j in $(seq 1 7)
   do
      p=$[2**$j]
      
      make run_e_omp N=$n P=$p FILE_NAME=$file
      make run_e_mpi N=$n P=$p FILE_NAME=$file

      status=$?
      if [ $status -ne 0 ]; then
         make clean
         exit $status
      fi
   done
done

make clean