#!/bin/bash

#SBATCH --nodes=32
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=1
#SBATCH -A ICT23_MHPC
#SBATCH --time 0:10:00
#SBATCH --gres=gpu:4
#SBATCH --mem=490000MB
#SBATCH -p boost_usr_prod
#SBATCH -e error-%j.err
#SBATCH -o output-%j.out


module load nvhpc/23.1  openmpi/4.1.4--nvhpc--23.1-cuda-11.8

mpic++ -o laplace.x -O3 ./src/laplace.cpp -I./include/ -L/leonardo/prod/spack/03/install/0.19/linux-rhel8-icelake/gcc-8.5.0/nvhpc-23.1-x5lw6edfmfuot2ipna3wseallzl4oolm/Linux_x86_64/23.1/math_libs/lib64 -acc --std=c++17

mpirun ./laplace.x 80000 100 0.5 10 1

