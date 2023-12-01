set term png size 300,300
set ylabel "Speedup"
set xlabel "# Tasks"
set print
set size square
set key top left
array Serial[words(ARG2)]
set xrange [1:35]
set yrange [1:35]
do for [i=1:words(ARG1)] {
  print word(ARG1, i)
  do for [j=1:words(ARG2)] {
    Serial[j] = system('grep "'.substr(word(ARG2, j), 2, strlen(word(ARG2, j))).'" Timer/Optimization/'.word(ARG1, i).'_01_01_01_sz.dat')
    Serial[j] = substr(Serial[j], strlen(word(ARG2, j)) + 1, strlen(Serial[j]))
    print Serial[j]
  }
  set title word(ARG1, i)
  set output "../img/MPI_".word(ARG1, i)."_sp.png"
  plot for [j=1:words(ARG2)] "Timer/MPI/".word(ARG1, i).word(ARG2, j)."_tk.dat" \
       u 1:(Serial[j]/$2) w lp title substr(word(ARG2, j), 2, strlen(word(ARG2, j))), \
       x
  set output "../img/OpenMP_".word(ARG1, i)."_sp.png"
  plot for [j=1:words(ARG2)] "Timer/OpenMP/".word(ARG1, i).word(ARG2, j)."_tk.dat" \
       u 1:(Serial[j]/$2) w lp title substr(word(ARG2, j), 2, strlen(word(ARG2, j))), \
       x
}