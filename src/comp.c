#include "structs.h"
#include "utils.h"
#ifdef MY_MPI
#include <mpi.h>
#endif
#if defined (_OPENMP)
#include <omp.h>
#endif

#ifdef MORSE

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
  double epot = 0.0;
  /* zero energy and forces */
  #ifdef MY_MPI
  MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  #endif
  #if defined (_OPENMP)
  #pragma omp parallel reduction(+:epot)
  #endif
  {
  #if defined (_OPENMP)
  int tid = omp_get_thread_num(); //thread number as thread "rank"
  #else
  sys->nthreads = 1;
  int tid = 0;
  #endif
  double *fx, *fy, *fz;
  fx = sys->cx + (tid*sys->natoms);
  fy = sys->cy + (tid*sys->natoms);
  fz = sys->cz + (tid*sys->natoms);
  azzero(fx, sys->natoms);
  azzero(fy, sys->natoms);
  azzero(fz, sys->natoms);
  double c6, c12, rcsq = sys->rcut*sys->rcut;
  c6 = sys->sigma*sys->sigma*sys->sigma*sys->sigma*sys->sigma*sys->sigma;
  c12 = 4.0*sys->epsilon*c6*c6;
  c6 *= 4.0*sys->epsilon;
  for (int i = 0; i < (sys->natoms) - 1; i += sys->nsize * sys->nthreads) {
    int ii = i + sys->mpirank * sys->nthreads + tid;
    if (ii >= (sys->natoms - 1)) break;
    double rx1 = sys->rx[ii];
    double ry1 = sys->ry[ii];
    double rz1 = sys->rz[ii];
    for (int jj = ii + 1; jj < (sys->natoms); ++jj) {
      /* Get distance between particle i and jj */
      double rx = pbc(rx1 - sys->rx[jj], 0.5*sys->box);
      double ry = pbc(ry1 - sys->ry[jj], 0.5*sys->box);
      double rz = pbc(rz1 - sys->rz[jj], 0.5*sys->box);
      double rsq = rx*rx + ry*ry + rz*rz;
      /* Compute Force and Energy if within cutoff */
      if (rsq < rcsq) {
        /* Morse Potential
        MM = exp(- sys->a * (r - sys->re));
        // Mores potential
        sys->epot += sys->m + sys->De * (1-MM)*(1-MM);
        ffac = 2 * sys->a * sys->De * MM * (1-MM);
         */
        double rinv = ((1.0 / rsq) / rsq) / rsq;
        double ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
        epot += (c12*rinv - c6)*rinv;

        fx[ii] += rx*ffac;
        fy[ii] += ry*ffac;
        fz[ii] += rz*ffac;

        fx[jj] -= rx*ffac;
        fy[jj] -= ry*ffac;
        fz[jj] -= rz*ffac;
      }
    }
  }
  #if defined (_OPENMP)
  #pragma omp barrier
  #endif
  int i = (sys->natoms/sys->nthreads) + 1;
  int fromidx = tid * i;
  int toidx = fromidx + i;
  if (toidx > sys->natoms) toidx = sys->natoms;
  for (int i = 1; i < sys->nthreads; ++i) {
    int offs = i*sys->natoms;
    for (int j = fromidx; j < toidx; ++j) sys->cx[j] += sys->cx[offs + j];
    for (int j = fromidx; j < toidx; ++j) sys->cy[j] += sys->cy[offs + j];
    for (int j = fromidx; j < toidx; ++j) sys->cz[j] += sys->cz[offs + j];
  }
  }
  #ifdef MY_MPI
  MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Reduce(&epot, &(sys->epot), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  #else
  for (int i = 0; i < sys->natoms; i++) sys->fx[i] = sys->cx[i];
  for (int i = 0; i < sys->natoms; i++) sys->fy[i] = sys->cy[i];
  for (int i = 0; i < sys->natoms; i++) sys->fz[i] = sys->cz[i];
  sys->epot = epot;
  #endif
}
