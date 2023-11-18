<<<<<<< HEAD
/* velocity verlet */
=======
#include "structs.h"
#include "comp.h"
#include <math.h>

/*velocity verlet part two*/
void velverlet_prop(mdsys_t *sys){
  /* second part: propagate velocities by another half step */
  int i;
  for (i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / MVSQ2E * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / MVSQ2E * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / MVSQ2E * sys->fz[i] / sys->mass;
  }
}

/* velocity verlet part one*/
>>>>>>> 681f1c98edf0e5ffeac8d8d6f67d228c6ee7054f
void velverlet(mdsys_t *sys) {
  int i;

  /* first part: propagate velocities by half and positions by full step */
  for (i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / MVSQ2E * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / MVSQ2E * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / MVSQ2E * sys->fz[i] / sys->mass;
    sys->rx[i] += sys->dt*sys->vx[i];
    sys->ry[i] += sys->dt*sys->vy[i];
    sys->rz[i] += sys->dt*sys->vz[i];
  }
  
  /* compute forces and potential energy */
  force(sys);
  /*call the second part to propagate*/
  velverlet_prop(sys);
}
