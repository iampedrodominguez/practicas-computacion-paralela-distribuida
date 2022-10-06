#!/bin/bash
# bash data_generator.bash
make clean
make e3_data
rm -rf Data
mkdir -p Data

n=$[10**3]
times=$[3]
max_exp_p=$[7]
for e in $(seq 0 $max_exp_p)
do
   p=$[2**$e]
   filename="Data/data$p.csv"
   for i in $(seq 1 $times)
   do
      make run_e3_data N=$n P=$p FILE_NAME=$filename
      status=$?
      if [ $status -ne 0 ]; then
         make clean
         exit $status
      fi
   done
done

make clean