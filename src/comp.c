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

/*
  for(i = 0; i < (sys->natoms); ++i) {
    for(j = 0; j < (sys->natoms); ++j) {

      // particles have no interactions with themselves
      if (i == j) continue;

      // get distance between particle i and j
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      r = sqrt(rx*rx + ry*ry + rz*rz);

      // compute force and energy if within cutoff
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
*/

/*
for(i=0; i < (sys->natoms)-1; ++i) {
  for(j=i+1; j < (sys->natoms); ++j) {

      // get distance between particle i and j
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      r = sqrt(rx*rx + ry*ry + rz*rz);

      // compute force and energy if within cutoff 
      if (r < sys->rcut) {
        ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r, 12.0)/r
                                  + 6*pow(sys->sigma/r, 6.0)/r);

        sys->epot += 4.0*sys->epsilon*(pow(sys->sigma/r, 12.0)
                                           - pow(sys->sigma/r, 6.0));

        sys->fx[i] += rx/r*ffac;      sys->fx[j] -= rx/r*ffac;
        sys->fy[i] += ry/r*ffac;      sys->fy[j] -= ry/r*ffac;
        sys->fz[i] += rz/r*ffac;      sys->fz[j] -= rz/r*ffac;
      }
    }
  }
*/


double c12=4.0*sys->epsilon*pow(sys->sigma,12.0);
double c6 =4.0*sys->epsilon*pow(sys->sigma, 6.0);
double rcsq = sys->rcut * sys->rcut;
double rsq, r6, rinv; 
      
for(i=0; i < (sys->natoms)-1; ++i) {
  for(j=i+1; j < (sys->natoms); ++j) {

      // get distance between particle i and j
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      rsq = rx*rx + ry*ry + rz*rz;

      // compute force and energy if within cutoff 
      if (rsq < rcsq) {
        rinv=1.0/rsq; 
        r6=rinv*rinv*rinv;
        ffac = (12.0*c12*r6 - 6.0*c6)*r6*rinv;
        sys->epot += r6*(c12*r6 - c6);

        sys->fx[i] += rx*ffac;      sys->fx[j] -= rx*ffac;
        sys->fy[i] += ry*ffac;      sys->fy[j] -= ry*ffac;
        sys->fz[i] += rz*ffac;      sys->fz[j] -= rz*ffac;
      }
    }
  }


}
