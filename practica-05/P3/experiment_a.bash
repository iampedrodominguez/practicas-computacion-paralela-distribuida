#!/bin/bash

# Load Khipu modules

module load cmake/3.22.2
module load gcc/9.2.0
module load mpich/4.0

# Experiment C

make clean
make clean_data

N=1
for i in {1..4}
do
  N=$((N * (2 ** i)))
done

make ex_03_a.mpi.data
for j in {1..5}
do
  p=$((2 ** j))
  filename="data_$N\_\_$p.csv"
  echo "Experiment A: $N $p"
  for _ in {1..10}
  do
    make mpi.run N=$N P=$p FILE=$filename
  done
done

make clean

# Unload Khipu modules

module unload cmake/3.22.2
module unload gcc/9.2.0
module unload mpich/4.0
