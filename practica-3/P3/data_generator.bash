#!/bin/bash
# bash data_generator.bash
make clean
make e3_data
rm -rf Data
mkdir -p Data

for ee in {7..10}
do
   n=$[2**$ee]
   for e in {0..6}
   do
      p=$[2**$e]
      filename="Data/data$p.N$n.csv"
      for i in {1..10}
      do
         make run_e3_data P=$p FILE_NAME=$filename N=$n
         status=$?
         if [ $status -ne 0 ]; then
            make clean
            exit $status
         fi
      done
   done
done

make clean
python data_processor.py
