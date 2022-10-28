#!/bin/bash

# Load Khipu modules

# module load cmake/3.22.2
# module load gcc/9.2.0
# module load mpich/4.0

# Experiment C

make clean
make clean_data

N=1
for i in {1..4}
do
  N=$((N * (2 ** i)))
done

make ex_03_b.omp.data
for i in {1..6}
do
  t=$((2 ** i))
  filename="data_$N\_$t.csv"
  echo "Experiment B: $N $t"
  for _ in {1..10}
  do
    make omp.run N=$N T=$t FILE=$filename
  done
done

make clean

# Unload Khipu modules

# module unload cmake/3.22.2
# module unload gcc/9.2.0
# module unload mpich/4.0
