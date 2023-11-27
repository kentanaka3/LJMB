#include "structs.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
 #include <omp.h>
#endif

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
  #ifdef _OPENMP
  #pragma omp parallel reduction(+:epot)
  #endif
  {
    double *fx,*fy,*fz; //auxiliary pointers
    double rx1,ry1,rz1;
    int fromidx,toidx;
    #ifdef _OPENMP
      sys->nthreads=omp_get_num_threads(); //total number of threads
      int tid=omp_get_thread_num(); //thread number as thread "rank"
    #else
      int tid=0; //to preserve serial behavior 
      sys->nthreads = 1; //to preserve serial behavior
    #endif
    /* zero energy and forces */
    sys->epot = 0.0;
    fx=sys->fx+(tid*sys->natoms);
    fy=sys->fy+(tid*sys->natoms);
    fz=sys->fz+(tid*sys->natoms);

    azzero(fx, sys->natoms);
    azzero(fy, sys->natoms);
    azzero(fz, sys->natoms);

    for(i = 0; i < (sys->natoms-1); i+=sys->nthreads) {
      int ii = i+tid;
      rx1=sys->rx[ii];
      ry1=sys->ry[ii];
      rz1=sys->rz[ii];
      if (ii>=(sys->natoms-1)) break;

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

          #ifdef _OPENMP
          #pragma omp barrier
          #endif
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

          sys->fx[i] += rx/r*ffac;
          sys->fy[i] += ry/r*ffac;
          sys->fz[i] += rz/r*ffac;
        }
      }
    }
  }
}
