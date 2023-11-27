#include "structs.h"
#include "utils.h"
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
  double rx, ry, rz;
  int i, j;
  int tid=0;
  double epot=0.0;
  #ifdef _OPENMP
  #pragma omp parallel reduction(+:epot)
  #endif
  {
    double *fx,*fy,*fz; //auxiliary pointers
    double rx1,ry1,rz1;
    int fromidx,toidx;
    #ifdef _OPENMP
    tid=omp_get_thread_num(); //thread number as thread "rank"
    #endif
    /* zero energy and forces */
    fx=sys->fx+(tid*sys->natoms);
    fy=sys->fy+(tid*sys->natoms);
    fz=sys->fz+(tid*sys->natoms);
    azzero(fx, sys->natoms);
    azzero(fy, sys->natoms);
    azzero(fz, sys->natoms);

    double c6 = 1.0, c12, rcsq;
    for (i = 0; i < 6; i++) c6 *= sys->sigma;
    c12 = 4.0*sys->epsilon*c6*c6;
    c6 *= 4.0*sys->epsilon;
    rcsq = sys->rcut*sys->rcut;
    for(i = 0; i < (sys->natoms) - 1; i+=sys->nthreads) {
      int ii=i+tid;
      if (ii>=(sys->natoms-1)) break; 
      rx1=sys->rx[ii];
      ry1=sys->ry[ii];
      rz1=sys->rz[ii];  
      for(j = i + 1; j < (sys->natoms); ++j) {
        /* Get distance between particle i and j */
        rx = pbc(rx1 - sys->rx[j], 0.5*sys->box);
        ry = pbc(ry1 - sys->ry[j], 0.5*sys->box);
        rz = pbc(rz1 - sys->rz[j], 0.5*sys->box);

        rsq = rx*rx + ry*ry + rz*rz;
        double rinv = 1.0;
        for (int k = 0; k < 3; k++) rinv /= rsq;
        /* Compute Force and Energy if within cutoff */
        if (rsq < rcsq) {
          ffac = 6.0*(2.0*c12*rinv - c6)*rinv/rsq;
          epot += (c12*rinv - c6)*rinv;

          sys->fx[i] += rx*ffac;
          sys->fy[i] += ry*ffac;
          sys->fz[i] += rz*ffac;
          sys->fx[j] -= rx*ffac;
          sys->fy[j] -= ry*ffac;
          sys->fz[j] -= rz*ffac;
        }
      }
    }
    #ifdef _OPENMP
    #pragma omp barrier
    #endif
    sys->epot += epot;
    i=1+(sys->natoms/sys->nthreads);
    fromidx=tid*i;
    toidx=fromidx+i;
    if (toidx>sys->natoms) toidx = sys->natoms;

    for (i=1;i<sys->nthreads;++i) {
      int offs = i*sys->natoms;
      for (int j=fromidx;j<toidx;++j){
        sys->fx[j]+=sys->fx[offs+j];
        sys->fx[j]+=sys->fy[offs+j];
        sys->fz[j]+=sys->fz[offs+j];
      }
    }
  }  
}
