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
- output.c: append data to file

## Implemented tests
The implemented test framework for the separated functions is GoogleTest. The src files are in LJMB/test. After compiling, the tests executables can be found in LJMB/build:
- test_comp -> ekin, force functions 
- test_utils -> azzero, pbc functions
- test_verlet -> velverlet, velverlet_prop functions

## Comparison
![Run Time](img/RunTime.png)
![Force](img/Force.png)
![Velverlet](img/Velverlet.png)
![Propagate](img/Propagate.png)
![KineticEnergy](img/KineticEnergy.png)
![Start Up](img/StartupTime.png)
