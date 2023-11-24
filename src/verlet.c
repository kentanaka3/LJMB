#include "structs.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/* velocity verlet part one*/
void velverlet(mdsys_t *sys) {
  int i;

  /* first part: propagate velocities by half and positions by full step */
  for (i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
    sys->rx[i] += sys->dt*sys->vx[i];
    sys->ry[i] += sys->dt*sys->vy[i];
    sys->rz[i] += sys->dt*sys->vz[i];
  }
  
  /* compute forces and potential energy */
  force(sys);
}

/*velocity verlet part two*/
void velverlet_prop(mdsys_t *sys){
  /* second part: propagate velocities by another half step */
  int i;
  for (i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
  }
}
