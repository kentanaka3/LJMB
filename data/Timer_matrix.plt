set term png
set output "../img/MPI_OpenMP_".ARG1.".png"
set autoscale xfix
set autoscale yfix
set autoscale cbfix
set xlabel "npes"
set ylabel "
plot "Timer/MPI_OpenMP/".ARG1.".dat" matrix nonuniform with image title .ARG1.