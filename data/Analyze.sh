#!/bib/bash
if [[ ( "$1" == "Leonardo" ) || ( "$1" == "Ulysses" ) ]]
then
  echo "Analyzing for $1"
  for k in 1; do
    for j in 1 2 4 8 16 32; do
      for i in 1; do
        if [ "$(($j * $i))" -le 32 ]
        then
          echo "Nodes: $k, Tasks (MPI): $j, Threads (OpenMP): $i"
          sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) # # #/\1 ${k} ${j} ${i}/g" \
                    -e "s/(#SBATCH --nodes=)#/\1${k}/g" \
                    -e "s/(#SBATCH --tasks-per-node=)#/\1${j}/g" \
                    -e "s/(#SBATCH --cpus-per-task=)#/\1${i}/g" \
                    -e "s/(#SBATCH -e %j_)#_#_#.err/\1${k}_${j}_${i}.err/g" \
                    -e "s/(#SBATCH -o %j_)#_#_#.out/\1${k}_${j}_${i}.out/g" \
                    -e "s/(export OMP_NUM_THREADS=)#/\1${i}/g" $1.sh && \
          sbatch "$1".sh $2 && \
          sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) ${k} ${j} ${i}/\1 # # #/g" \
                    -e "s/(#SBATCH --nodes=)${k}/\1#/g" \
                    -e "s/(#SBATCH --tasks-per-node=)${j}/\1#/g" \
                    -e "s/(#SBATCH --cpus-per-task=)${i}/\1#/g" \
                    -e "s/(#SBATCH -e %j_)${k}_${j}_${i}.err/\1#_#_#.err/g" \
                    -e "s/(#SBATCH -o %j_)${k}_${j}_${i}.out/\1#_#_#.out/g" \
                    -e "s/(export OMP_NUM_THREADS=)${i}/\1#/g" $1.sh
        fi
      done
    done
  done
fi
