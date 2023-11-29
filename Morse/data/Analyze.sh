#!/bib/bash
if [[ ( "$1" == "Leonardo" ) || ( "$1" == "Ulysses" ) ]]; then
  echo "Analyzing for $1"
  #for j in 4 6 8 12 16 24 32; do
  #  for i in 8 16 32; do
  for j in 1; do
    for i in 32; do
      sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) # #/\1 ${j} ${i}/g" \
                -e "s/(#SBATCH --nodes=)#/\1${j}/g" \
                -e "s/(#SBATCH --tasks-per-node=)#/\1$((32/${i}))/g" \
                -e "s/(#SBATCH --cpus-per-task=)#/\1${i}/g" \
                -e "s/(export OMP_NUM_THREADS=)#/\1${i}/g" $1.sh && \
      sbatch "$1".sh $2 && \
      sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) ${j} ${i}/\1 # #/g" \
                -e "s/(#SBATCH --nodes=)${j}/\1#/g" \
                -e "s/(#SBATCH --tasks-per-node=)$((32/${i}))/\1#/g" \
                -e "s/(#SBATCH --cpus-per-task=)${i}/\1#/g" \
                -e "s/(export OMP_NUM_THREADS=)${i}/\1#/g" $1.sh
    done
  done
fi
# --nodes = 2, 4, 8
# --cpus-per-task | --tasks-per-node | OMP_NUM_THREADS
#               2                 16                 2
#               4                  8                 4
#               8                  4                 8
#              16                  2                16
#              32                  1                32