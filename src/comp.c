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
  int i, j, ii, tid = 0;
  /* zero energy and forces */
  #ifdef MY_MPI
  MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  #endif
  double epot = 0.0;
  #ifdef _OPENMP
  //sys->nthreads = omp_get_num_threads();
  sys->nthreads = omp_get_max_threads();
  #pragma omp parallel reduction(+:epot)
  {
  tid = omp_get_thread_num(); //thread number as thread "rank"
  #else
  sys->nthreads = 1;
  tid = 0;
  #endif
  double *fx, *fy, *fz;
  fx = sys->cx + (tid*sys->natoms);
  fy = sys->cy + (tid*sys->natoms);
  fz = sys->cz + (tid*sys->natoms);
  azzero(fx, sys->natoms);
  azzero(fy, sys->natoms);
  azzero(fz, sys->natoms);
  double c6 = 1.0, c12, rcsq = sys->rcut*sys->rcut;
  for (k = 0; k < 6; k++) c6 *= sys->sigma;
  c12 = 4.0*sys->epsilon*c6*c6;
  c6 *= 4.0*sys->epsilon;
  for (i = 0; i < (sys->natoms) - 1; i += sys->nsize * sys->nthreads) {
    ii = i + sys->mpirank * sys->nthreads + tid;
    if (ii >= (sys->natoms - 1)) break;
    double rx1 = sys->rx[ii], ry1 = sys->ry[ii], rz1 = sys->rz[ii];
    for (j = ii + 1; j < (sys->natoms); ++j) {
      /* Get distance between particle i and j */
      double rx, ry, rz;
      rx = pbc(rx1 - sys->rx[j], 0.5*sys->box);
      ry = pbc(ry1 - sys->ry[j], 0.5*sys->box);
      rz = pbc(rz1 - sys->rz[j], 0.5*sys->box);
      rsq = rx*rx + ry*ry + rz*rz;
      /* Compute Force and Energy if within cutoff */
      if (rsq < rcsq) {
        double rinv = ((1.0 / rsq) / rsq) / rsq;
        ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
        epot += (c12*rinv - c6)*rinv;

        fx[ii] += rx*ffac;
        fy[ii] += ry*ffac;
        fz[ii] += rz*ffac;

        fx[j] -= rx*ffac;
        fy[j] -= ry*ffac;
        fz[j] -= rz*ffac;
      }
    }
  }
  #ifdef _OPENMP
  #pragma omp barrier
  #endif
  i = (sys->natoms/sys->nthreads) + 1;
  int fromidx = tid * i, toidx = fromidx + i;
  if (toidx > sys->natoms) toidx = sys->natoms;
  for (i = 1; i < sys->nthreads; ++i) {
    int offs = i*sys->natoms;
    for (j = fromidx; j < toidx; ++j) sys->cx[j] += sys->cx[offs + j];
    for (j = fromidx; j < toidx; ++j) sys->cy[j] += sys->cy[offs + j];
    for (j = fromidx; j < toidx; ++j) sys->cz[j] += sys->cz[offs + j];
  }
  #ifdef _OPENMP
  }
  #endif
  #ifdef MY_MPI
  MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(&epot, %sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  #else
  for (i = 0; i < sys->natoms; i++) sys->fx[i] = sys->cx[i];
  for (i = 0; i < sys->natoms; i++) sys->fy[i] = sys->cy[i];
  for (i = 0; i < sys->natoms; i++) sys->fz[i] = sys->cz[i];
  sys->epot = epot;
  #endif
}
