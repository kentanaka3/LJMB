#include "structs.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

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
  int i, j;

  /* zero energy and forces */
  sys->epot = 0.0;
  azzero(sys->fx, sys->natoms);
  azzero(sys->fy, sys->natoms);
  azzero(sys->fz, sys->natoms);
  double c6 = 1, c12, rcsq, rinv = 1;
  for (i = 0; i < 6; i++) c6 *= sys->sigma;
  c12 = 4.0*sys->epsilon*c6*c6;
  c6 *= 4.0*sys->epsilon;
  rcsq = sys->rcut*sys->rcut;
  for(i = 0; i < (sys->natoms) - 1; ++i) {
    for(j = i + 1; j < (sys->natoms); ++j) {
      /* Get distance between particle i and j */
      rx = pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry = pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz = pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      rsq = rx*rx + ry*ry + rz*rz;
      for (int k = 0; k < 3; k++) rinv /= rsq;
      /* Compute Force and Energy if within cutoff */
      if (rsq < rcsq) {
        ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
        sys->epot += (c12*rinv - c6)*rinv;

        sys->fx[i] += rx*ffac;
        sys->fy[i] += ry*ffac;
        sys->fz[i] += rz*ffac;
        sys->fx[j] -= rx*ffac;
        sys->fy[j] -= ry*ffac;
        sys->fz[j] -= rz*ffac;
      }
    }
  }
}
