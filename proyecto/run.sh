#!/bin/bash
#SBATCH --job-name=CPD_PROJECT
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=256G
#SBATCH --output=./output/CPD_PROJECT.out
#SBATCH --error=./output/CPD_PROJECT.err

module purge
module load gcc/10.1.0

# Generate points
make points_generator.test
make points_generator.test.run

# TSP problem

make tester.test
make tester.test.run