#!/bin/bash
#SBATCH --job-name=CPD_PROJECT
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=256G
#SBATCH --output=./output/CPD_PROJECT.out
#SBATCH --error=./output/CPD_PROJECT.err

# Load Khipu modules

module purge
module load cmake/3.22.2
module load gcc/10.1.0

points_file="points_random_19.txt"
times_file="experiments_times.csv"

make clean

# Experiment A: Sequential

make run.tester_sec PFILE=$points_file TFILE=$times_file

# Experiment B: Parallel OMP

for t in {2..16}
do
    echo "Running experiment B with $t threads"
    for _ in {1..3}
    do
        make run.tester.omp T=$t PFILE=$points_file TFILE=$times_file
    done
done

# Unload Khipu modules

module unload cmake/3.22.2
module unload gcc/10.1.0