set term png size 400,400
set ylabel "Speedup"
set xlabel "# Tasks"
set print
set size square
set key top left
array Serial[words(ARG2)]
do for [i=1:words(ARG1)] {
  # Serial
  print word(ARG1, i)
  do for [j=1:words(ARG2)] {
    Serial[j] = system('grep "'.substr(word(ARG2, j), 2, strlen(word(ARG2, j))).'" Timer/Optimization/'.word(ARG1, i).'_01_01_01_sz.dat')
    Serial[j] = substr(Serial[j], strlen(word(ARG2, j)) + 1, strlen(Serial[j]))
    print Serial[j]
  }
  # MPI
  set title word(ARG1, i)
  set output "../img/MPI_".word(ARG1, i)."_sp.png"
  plot for [j=1:words(ARG2)] for [k=1:words(ARG3)] "Timer/MPI/".word(ARG1, i)."_".word(ARG3, k).word(ARG2, j)."_tk.dat" \
       u 2:(Serial[j]/($3*$1)) w lp title "Sz: ".substr(word(ARG2, j), 2, strlen(word(ARG2, j))), \
       x
  # OpenMP
  set output "../img/OpenMP_".word(ARG1, i)."_sp.png"
  plot for [j=1:words(ARG2)] "Timer/OpenMP/".word(ARG1, i)."_01".word(ARG2, j)."_tk.dat" \
       u 2:(Serial[j]/($3)) w lp title substr(word(ARG2, j), 2, strlen(word(ARG2, j))), \
       x
}