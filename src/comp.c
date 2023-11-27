#include "structs.h"
#include "utils.h"
#ifdef MY_MPI
#include "myMPI.hpp"
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
  double rsq, ffac, epot;
  double rx, ry, rz;
  int i, j, ii;

  /* zero energy and forces */
  epot = 0.0;
  azzero(sys->cx, sys->natoms);
  azzero(sys->cy, sys->natoms);
  azzero(sys->cz, sys->natoms);
  #ifdef MY_MPI
  MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
  #endif
  double c6 = 1.0, c12, rcsq;
  for (i = 0; i < 6; i++) c6 *= sys->sigma;
  c12 = 4.0*sys->epsilon*c6*c6;
  c6 *= 4.0*sys->epsilon;
  rcsq = sys->rcut*sys->rcut;
  for (i = 0; i < (sys->natoms) - 1; i += sys->nsize) {
    ii = i + sys->mpirank;
    if (ii >= (sys->natoms - 1)) break;
    for (j = i + 1; j < (sys->natoms); ++j) {
      /* Get distance between particle i and j */
      rx = pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry = pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz = pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      rsq = rx*rx + ry*ry + rz*rz;
      double rinv = 1.0;
      for (int k = 0; k < 3; k++) rinv /= rsq;
      /* Compute Force and Energy if within cutoff */
      if (rsq < rcsq) {
        ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
        sys->epot += (c12*rinv - c6)*rinv;

        sys->fx[i] += rx*ffac;
        sys->fy[i] += ry*ffac;
        sys->fz[i] += rz*ffac;
        sys->cx[j] -= rx*ffac;
        sys->cy[j] -= ry*ffac;
        sys->cz[j] -= rz*ffac;
      }
    }
  }
  #ifdef MY_MPI
  MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             sys->mpicomm);
  MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             sys->mpicomm);
  MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0,
             sys->mpicomm);
  MPI_Reduce(&epot, %sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
  #endif
}
