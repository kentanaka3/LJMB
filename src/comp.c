#include "structs.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* Compute Kinetic Energy */
void ekin(mdsys_t *sys) {
  sys->ekin=0.0;
  for (int i = 0; i < sys->natoms; ++i) {
    sys->ekin += 0.5*MVSQ2E*sys->mass*(sys->vx[i]*sys->vx[i] + \
                                       sys->vy[i]*sys->vy[i] + \
                                       sys->vz[i]*sys->vz[i]);
  }
  sys->temp = 2.0*sys->ekin/(3.0*sys->natoms - 3.0)/KBOLTZ;
}

/* compute forces */
void force(mdsys_t *sys) {
  double r, ffac;
  double rx, ry, rz;
  int i, j;

  /* zero energy and forces */
  sys->epot = 0.0;
  azzero(sys->fx, sys->natoms);
  azzero(sys->fy, sys->natoms);
  azzero(sys->fz, sys->natoms);

  for(i = 0; i < (sys->natoms); ++i) {
    for(j = 0; j < (sys->natoms); ++j) {

      /* particles have no interactions with themselves */
      if (i == j) continue;

      /* get distance between particle i and j */
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      r = sqrt(rx*rx + ry*ry + rz*rz);

      /* compute force and energy if within cutoff */
      if (r < sys->rcut) {
        ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r, 12.0)/r
                                  + 6*pow(sys->sigma/r, 6.0)/r);

        sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r, 12.0)
                                           - pow(sys->sigma/r, 6.0));

        sys->fx[i] += rx/r*ffac;
        sys->fy[i] += ry/r*ffac;
        sys->fz[i] += rz/r*ffac;
      }
    }
  }
}