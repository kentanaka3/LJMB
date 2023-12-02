set term png size 700,600
set xlabel "Threads"
set ylabel "Tasks"
set clabel "Time"
set xrange [-0.5:4.5]
set size square
set xtics("1" 0, "2" 1, "4" 2, "8" 3, "16" 4)
set yrange [-0.5:4.5] reverse
set palette negative
set ytics("1" 0, "2" 1, "4" 2, "8" 3, "16" 4)
do for [i=1:words(ARG1)] {
  do for [j=1:words(ARG2)] {
    do for[k=1:words(ARG3)] {
      set title word(ARG1, i).", Size: ".substr(word(ARG2, j), 2, strlen(word(ARG2, j)))
      set output "../img/MPI_OpenMP_".word(ARG1, i)."_".word(ARG3, k).word(ARG2, j).".png"
      plot "Timer/MPI_OpenMP/".word(ARG1, i)."_".word(ARG3, k).word(ARG2, j)."_tk.dat" matrix with image notitle, \
      '' matrix using 1:2:(sprintf('%.2f', $3)) with labels font ',16' notitle
    }
  }
}