#!/bin/bash
# bash experiments.bash
make clean
make e1_debug
rm -rf Data
mkdir -p Data

valmin=$[10]
valmax=$[1]
valstep=$[10]

# comment below
# for i in $(seq $valmin $valstep $valmax)
for i in $(seq 1 $valmax)
do
   # filename="Data/exp_$i.csv"
   filename="Data/exp_10.csv"
   for j in $(seq 1 500)
   do
      make run_e1_debug FILE_NAME=$filename

      status=$?
      if [ $status -ne 0 ]; then
         make clean
         exit $status
      fi
   done
done

make clean