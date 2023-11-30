#include "structs.h"
#include "comp.h"
#ifdef _OPENMP
#include <omp.h>
#endif

/*velocity verlet part two*/
void velverlet_prop(mdsys_t *sys){
  /* Propagate velocities by another half step */
  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for (int i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / MVSQ2E * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / MVSQ2E * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / MVSQ2E * sys->fz[i] / sys->mass;
  }
}

/* Velocity verlet part one */
void velverlet(mdsys_t *sys) {
  /* Propagate velocities by half and positions by full step */
  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for (int i = 0; i < sys->natoms; ++i) {
    sys->vx[i] += 0.5*sys->dt / MVSQ2E * sys->fx[i] / sys->mass;
    sys->vy[i] += 0.5*sys->dt / MVSQ2E * sys->fy[i] / sys->mass;
    sys->vz[i] += 0.5*sys->dt / MVSQ2E * sys->fz[i] / sys->mass;
    sys->rx[i] += sys->dt*sys->vx[i];
    sys->ry[i] += sys->dt*sys->vy[i];
    sys->rz[i] += sys->dt*sys->vz[i];
  }
}
