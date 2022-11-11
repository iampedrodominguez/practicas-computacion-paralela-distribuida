#!/bin/bash
# bash debug.bash
make clean
mkdir -p Data

# max 2^25
n=$[2**4]
p=$[4]
filename="Data/data"$n".txt"

# Generate ramdom data
make e2_data 
make run_e2_data N=$n

# Compile and run
make e2_debug
make run_e2_debug P=$p FILE_NAME=$filename
#make e2
#make run_e2 P=$p FILE_NAME=$filename

make clean