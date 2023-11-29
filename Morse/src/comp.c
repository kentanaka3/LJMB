#include "structs.h"
#include "utils.h"

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
  double MM, r, ffac;
  double rx, ry, rz;
  int i, j;

  /* zero energy and forces */
  sys->epot = 0.0;
  azzero(sys->fx, sys->natoms);
  azzero(sys->fy, sys->natoms);
  azzero(sys->fz, sys->natoms);

  for(i = 0; i < (sys->natoms) - 1; ++i) {
    for(j = i + 1; j < (sys->natoms); ++j) {
      /* Get distance between particle i and j */
      rx = pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry = pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz = pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      r = sqrt(rx*rx + ry*ry + rz*rz);
//      printf("%f \n",r);
      /* Compute Force and Energy if within cutoff */
      if (r < sys->rcut) {
        MM = exp(- sys->a * (r - sys->re));
        // Mores potential
        sys->epot += sys->m + sys->De * (1-MM)*(1-MM);
        ffac = 2 * sys->a * sys->De * MM * (1-MM);

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