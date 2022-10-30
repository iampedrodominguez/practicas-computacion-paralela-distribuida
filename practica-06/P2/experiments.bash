#!/bin/bash
# bash experiments.bash
make clean
#make e1_debug
make e1
rm -rf Data
mkdir -p Data

valmin=$[200]
valmax=$[1000]
valstep=$[200]

# comment below
for i in $(seq $valmin $valstep $valmax)
do
   filename="Data/exp_$i.csv"
   for j in $(seq 1 $i)
   do
      #make run_e1_debug FILE_NAME=$filename
      make run_e1 FILE_NAME=$filename

      status=$?
      if [ $status -ne 0 ]; then
         make clean
         exit $status
      fi
   done
done

make clean