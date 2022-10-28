#!/bin/bash

make clean
make clean_data

N=1
for i in {1..4}
do
  N=$((N * (2 ** i)))
done

make ex_03.mpi.data
for i in {1..4}
do
  t=$((2 ** i))
  for j in {1..4}
  do
    p=$((2 ** j))
    filename="data_$N\_$t\_$p.csv"
    echo "Experiment C: $N $t $p"
    for _ in {1..10}
    do
      make mpi.run N=$N T=$t P=$p FILE=$filename
    done
  done
done
