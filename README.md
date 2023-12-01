# LJMB
A simple Lennard-Jones Many-Body (LJMB) Simulator Optimization and Parallelization

- Andrea -> openmp
- Fathi  -> mpi
- Ken    -> optimization

## Functions
Splitted functions source files are in LJMD/src:
- main.cpp: main function
- init.c: initialize -> reading input files and restart, memory allocation
- comp.c: ekin -> kinetic energy computation; force -> forces computation
- verlet.c: velverlet -> velocity propagation by half, position by a full step; velverlet_prop -> propagate velocities by another half step 
- utils.c: azzero -> array gets all elements zeroed; helper -> apply minimum image convention; get_a_line: reads a line from a file and cuts away blank spaces and comments
- output.c: output -> append data to output file
- cleanup.c: cleanup -> close files, free allocated memory.

## Headers and auxiliary files
Headers and auxiliary files are in LJMD/inc:
- structs.c: physical constants which are used during computation
- Timer.hpp: functions for measuring and printing time results
- myMPI.hpp: MPI variables
- init.h, comp.h, verlet.h, utils.h, output.h, cleanup.h: prototypes for the respective functions.
The headers and auxiliary files are built by the compiler into a mdlib library.

## Implemented tests
The implemented test framework for the separated functions is GoogleTest. The src files used for compiling are in LJMB/test. After compiling, the tests executables can be found in LJMB/build. All tests are executed on 2 particles:
- test_comp -> testing ekin, force functions
- test_utils -> testing azzero, pbc functions 
- test_verlet -> testing velverlet, velverlet_prop functions

## Comparison
# Serial
Serial runs without optimizations.
![Run Time](img/RunTime.png)
![Force](img/Force.png)
![Velverlet](img/Velverlet.png)
![Propagate](img/Propagate.png)
![KineticEnergy](img/KineticEnergy.png)
![Start Up](img/StartupTime.png)

# Optimized
Serial runs with "-O3 -Wall -ffast-math -fexpensive-optimizations -msse3" compiler flags and code optimizations (Newton's 3rd law, expensive math operations avoided).

![RunTime Size](img/Optimized_RunTime_sz.png)
![RunTime Task](img/Optimized_Force_tk.png)
![Force Size](img/Optimized_Force_sz.png)
![Force Task](img/Optimized_Force_tk.png)
![Velverlet Size](img/Optimized_Velverlet_sz.png)
![Velverlet Task](img/Optimized_Velverlet_tk.png)
![Propagate Size](img/Optimized_Propagate_sz.png)
![Propagate Task](img/Optimized_Propagate_tk.png)
![Kinetic Energy Size](img/Optimized_KineticEnergy_sz.png)
![Kinetic Energy Task](img/Optimized_Force_tk.png)

# MPI
Parallel runs with:
- Number of Leonardo nodes: 1;
- Number of processing elements: 2, 4, 8, 16, 32;
- Number of threads: 1.

![RunTime Size](img/MPI_RunTime_sz.png)
![RunTime Task](img/MPI_Force_tk.png)
![Force Size](img/MPI_Force_sz.png)
![Force Task](img/MPI_Force_tk.png)
![Velverlet Size](img/MPI_Velverlet_sz.png)
![Velverlet Task](img/MPI_Velverlet_tk.png)
![Propagate Size](img/MPI_Propagate_sz.png)
![Propagate Task](img/MPI_Propagate_tk.png)
![Kinetic Energy Size](img/MPI_KineticEnergy_sz.png)
![Kinetic Energy Task](img/MPI_Force_tk.png)

# OpenMP
Parallel runs with:
- Number of Leonardo nodes: 1;
- Number of processing elements: 1;
- Number of threads: 2, 4, 6, 8, 16, 32.

![RunTime Size](img/OpenMP_RunTime_sz.png)
![RunTime Task](img/OpenMP_Force_tk.png)
![Force Size](img/OpenMP_Force_sz.png)
![Force Task](img/OpenMP_Force_tk.png)
![Velverlet Size](img/OpenMP_Velverlet_sz.png)
![Velverlet Task](img/OpenMP_Velverlet_tk.png)
![Propagate Size](img/OpenMP_Propagate_sz.png)
![Propagate Task](img/OpenMP_Propagate_tk.png)
![Kinetic Energy Size](img/OpenMP_KineticEnergy_sz.png)
![Kinetic Energy Task](img/OpenMP_Force_tk.png)

# MPI+OpenMP
Parallel runs with: 
- Number of Leonardo nodes: 1;
- Number of processing elements: 2, 4, 6, 8, 16, 32;
- Number of threads: 2, 4, 6, 8, 16, 32.
maintaining npes*threads < 32 (number of cores in a Leonardo node)
