#!/bib/bash
if [[ ( "$1" == "Leonardo" ) || ( "$1" == "Ulysses" ) ]]; then
  echo "Analyzing for $1"
<<<<<<< HEAD
<<<<<<< HEAD
  for k in 1; do
    for j in 2 4 8 16 32; do
      for i in 2 4 8 16 32; do
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
=======
  for k in 1 2 4 8; do
=======
  for k in 1; do
>>>>>>> 6273d879d491d54365f85c59da898586915ae952
    for j in 1 2 4 8 16 32; do
      for i in 1 2 4 8 16 32; do
        if [ "$(($j * $i))" -le 32 ]; then
          x=$(printf "%02d" $i)
          y=$(printf "%02d" $j)
          z=$(printf "%02d" $k)
          echo "Nodes: $z, Tasks (MPI): $y, Threads (OpenMP): $x"
          sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) # # #/\1 ${k} ${j} ${i}/g" \
                    -e "s/(#SBATCH --nodes=)#/\1${k}/g" \
                    -e "s/(#SBATCH --tasks-per-node=)#/\1${j}/g" \
                    -e "s/(#SBATCH --cpus-per-task=)#/\1${i}/g" \
                    -e "s/(#SBATCH -e %j_)#_#_#.err/\1${z}_${y}_${x}.err/g" \
                    -e "s/(#SBATCH -o %j_)#_#_#.out/\1${z}_${y}_${x}.out/g" \
<<<<<<< HEAD
                    -e "s/(export OMP_NUM_THREADS=)#/\1${i}/g" $1.sh > "${z}_${y}_${x}.dat" && \
>>>>>>> bdea9c70a40ddddd96710b1840442683711b07f7
=======
                    -e "s/(export OMP_NUM_THREADS=)#/\1${i}/g" $1.sh && \
>>>>>>> 6273d879d491d54365f85c59da898586915ae952
          sbatch "$1".sh $2 && \
          sed -i -E -e "s/(#SBATCH --job-name=\"LJMB) ${k} ${j} ${i}/\1 # # #/g" \
                    -e "s/(#SBATCH --nodes=)${k}/\1#/g" \
                    -e "s/(#SBATCH --tasks-per-node=)${j}/\1#/g" \
                    -e "s/(#SBATCH --cpus-per-task=)${i}/\1#/g" \
                    -e "s/(#SBATCH -e %j_)${z}_${y}_${x}.err/\1#_#_#.err/g" \
                    -e "s/(#SBATCH -o %j_)${z}_${y}_${x}.out/\1#_#_#.out/g" \
                    -e "s/(export OMP_NUM_THREADS=)${i}/\1#/g" $1.sh
        fi
      done
    done
  done
fi
