#! /bin/bash

#SBATCH --job-name="LJMB # #"
#SBATCH --nodes=#
#SBATCH --tasks-per-node=#
#SBATCH --cpus-per-task=#
#SBATCH  -A ICT23_MHPC
#SBATCH --time 1:00:00
#SBATCH --gres=gpu:1
#SBATCH --mem=49000MB
#SBATCH  -p boost_usr_prod
#SBATCH -e error-%j.err
#SBATCH -o output-%j.out

export OMP_NUM_THREADS=#
#module load openmpi/4.1.4--nvhpc--23.1-cuda-11.8 openblas/0.3.21--nvhpc--23.1
../build/MAIN.x < $1