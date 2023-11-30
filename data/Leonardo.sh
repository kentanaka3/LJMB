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
#SBATCH -e %j_#_#.err
#SBATCH -o %j_#_#.out

export OMP_NUM_THREADS=#
../build/MAIN.x < $1