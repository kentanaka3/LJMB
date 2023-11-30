set term png
set output "../img/MPI_OpenMP_".ARG2.".png"
set autoscale xfix
set autoscale yfix
set autoscale cbfix
plot "Timer/MPI_OpenMP/ matrix nonuniform with image title .ARG2.