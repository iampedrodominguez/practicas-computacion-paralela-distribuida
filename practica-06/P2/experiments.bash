#!/bin/bash
# bash experiments.bash
make clean
mkdir -p Data
rm "Data/times.csv"

# max 2^25
n=$[2**4]
p=$[4]
filename="Data/data"$n".txt"

# falta de 2^19 y 2^5 proccessors, 2^20 demora mucho

#for i in $(seq 10 20)
for i in $(seq 20 25)
do
   n=$[2**$i]
   filename="Data/data"$n".txt"

   # Generate ramdom data
   make e2_data 
   make run_e2_data N=$n

   for j in $(seq 2 7)
   do
      p=$[2**$j]

      make e2
      make run_e2 P=$p FILE_NAME=$filename
      
      status=$?
      if [ $status -ne 0 ]; then
         make clean
         exit $status
      fi
   done
done

make clean