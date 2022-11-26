#!/bin/bash
#SBATCH --job-name=CPD_PROJECT
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=256G
#SBATCH --output=./output/CPD_PROJECT.out
#SBATCH --error=./output/CPD_PROJECT.err

module purge
module load cmake/3.22.2
module load gcc/10.1.0

# Generate points
make omp.points_generator.test
make omp.points_generator.test.run

# TSP problem

make omp.tester.test
make omp.tester.test.run