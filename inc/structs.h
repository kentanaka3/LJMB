/* Header for Data Structures and Constants */
#ifndef STRUCTS_H
  #define STRUCTS_H

  /* generic file- or pathname buffer length */
  #define BLEN 200
  #ifndef LJMD_VERSION
    #define LJMD_VERSION 1.0
  #endif
  extern const double KBOLTZ;
  extern const double MVSQ2E;
  /* structure to hold the complete information about the MD system */
  struct _mdsys {
<<<<<<< HEAD
    int natoms, nfi, nsteps, nsize, mpirank;
=======
    int natoms,nfi,nsteps,nthreads;
>>>>>>> openmp
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
    double *cx, *cy, *cz;
  };
  typedef struct _mdsys mdsys_t;
#endif
