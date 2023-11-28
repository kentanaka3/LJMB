#include "structs.h"
#include "utils.h"
#ifdef MY_MPI
#include "myMPI.hpp"
#endif
#ifdef _OPENMP
#include <omp.h>
#endif

/* Compute Kinetic Energy */
void ekin(mdsys_t *sys) {
  sys->ekin = 0.0;
  for (int i = 0; i < sys->natoms; ++i) {
    sys->ekin += 0.5*MVSQ2E*sys->mass*(sys->vx[i]*sys->vx[i] + \
                                       sys->vy[i]*sys->vy[i] + \
                                       sys->vz[i]*sys->vz[i]);
  }
  sys->temp = 2.0*sys->ekin/(3.0*sys->natoms - 3.0)/KBOLTZ;
}

/* compute forces */
void force(mdsys_t *sys) {
  double rsq, ffac;
  double rx, ry, rz;
  int i, j, ii, tid = 0;
  /* zero energy and forces */
  double epot = 0.0;
  #ifdef MY_MPI
  MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  #endif
  double c6 = 1.0, c12, rcsq = sys->rcut*sys->rcut;
  for (i = 0; i < 6; i++) c6 *= sys->sigma;
  c12 = 4.0*sys->epsilon*c6*c6;
  c6 *= 4.0*sys->epsilon;
  #ifdef _OPENMP
  #pragma omp parallel reduction(+: epot)
  tid = omp_get_thread_num(); //thread number as thread "rank"
  sys->nthreads = omp_get_max_threads();
  #endif
  {
    double rx1, ry1, rz1;
    int fromidx, toidx;
    azzero(sys->cx, sys->natoms);
    azzero(sys->cy, sys->natoms);
    azzero(sys->cz, sys->natoms);
    for (i = 0; i < (sys->natoms) - 1; i += sys->nsize + sys->nthreads) {
      ii = i + sys->mpirank + tid;
      if (ii >= (sys->natoms - 1)) break;
      rx1 = sys->rx[ii];
      ry1 = sys->ry[ii];
      rz1 = sys->rz[ii];
      for (j = ii + 1; j < (sys->natoms); ++j) {
        /* Get distance between particle i and j */
        rx = pbc(rx1 - sys->rx[j], 0.5*sys->box);
        ry = pbc(rx1 - sys->ry[j], 0.5*sys->box);
        rz = pbc(rz1 - sys->rz[j], 0.5*sys->box);
        rsq = rx*rx + ry*ry + rz*rz;
        double rinv = 1.0;
        for (int k = 0; k < 3; k++) rinv /= rsq;
        /* Compute Force and Energy if within cutoff */
        if (rsq < rcsq) {
          ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
          epot += (c12*rinv - c6)*rinv;

          sys->cx[ii] += rx*ffac;
          sys->cy[ii] += ry*ffac;
          sys->cz[ii] += rz*ffac;

          sys->cx[j] -= rx*ffac;
          sys->cy[j] -= ry*ffac;
          sys->cz[j] -= rz*ffac;
        }
      }
    }
    
    #ifdef _OPENMP
    #pragma omp barrier
    #endif
    
    i = 1 + (sys->natoms/sys->nthreads);
    fromidx = tid*i;
    toidx = fromidx + i;
    if (toidx > sys->natoms) toidx = sys->natoms;

    for (i = 1; i < sys->nthreads; ++i) {
      int offs = i*sys->natoms;
      for (j = fromidx; j < toidx; ++j){
        sys->fx[j] += sys->fx[offs + j];
        sys->fx[j] += sys->fy[offs + j];
        sys->fz[j] += sys->fz[offs + j];
      }
    }
  }
  #ifdef MY_MPI
  MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(&epot, %sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  #else
  sys->fx = sys->cx;
  sys->fy = sys->cy;
  sys->fz = sys->cz;
  sys->epot = epot;
  #endif
}
