#!/bin/bash
# bash data_generator.bash
make clean
rm -rf Data
rm graph.png
mkdir -p Data
make e3_data

for p in {2..10}
do
   filename="Data/data$p.csv"
   for i in {1..3}
   do
      make run_e3_data P=$p FILE_NAME=$filename
   done
done

make clean
python3 data_processor.py