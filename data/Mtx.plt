set term png size 600,500
set xlabel "Tasks"
set ylabel "Threads"
set clabel "Time"
set xrange [-0.5:3.5]
set size square
set xtics("2" 0, "4" 1, "8" 2, "16" 3)
set yrange [-0.5:3.5] reverse
set palette negative
set ytics("2" 0, "4" 1, "8" 2, "16" 3)
do for [i=1:words(ARG1)] {
  do for [j=1:words(ARG2)] {
    set title word(ARG1, i).", Size: ".substr(word(ARG2, j), 2, strlen(word(ARG2, j)))
    set output "../img/MPI_OpenMP".word(ARG1, i)."_".word(ARG2, j).".png"
    plot "Timer/MPI_OpenMP/".word(ARG1, i).word(ARG2, j)."_tk.dat" matrix with image notitle, \
    '' matrix using 1:2:(sprintf('%.2f', $3)) with labels font ',16' notitle
  }
}