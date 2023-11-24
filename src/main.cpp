#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.hpp"
#include "structs.h"
#include "init.h"
#include "output.h"
#include "comp.h"
#include "verlet.h"
#include "structs.h"
#ifdef MY_MPI
  #include "myMPI.hpp"
#endif

int main(int argc, char *argv[]) {
  #ifdef MY_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myPE);
    MPI_Comm_size(MPI_COMM_WORLD, &nPEs);
  #endif

  int nprint, i;
  char trajfile[BLEN], ergfile[BLEN];
  FILE *traj, *erg;
  mdsys_t sys;

  printf("LJMD version %3.1f\n", LJMD_VERSION);
  {CSimpleTimer t{"Startup Time"};
  initialize(&sys, trajfile, ergfile, &nprint);

  /* Initialize Forces and Energies.*/
  sys.nfi = 0;
  force(&sys);
  ekin(&sys);
  }
  printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
  printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");

  erg = fopen(ergfile, "w");
  traj = fopen(trajfile, "w");
  output(&sys, erg, traj);

  {CSimpleTimer t{"Run Time"};
  /* Main MD loop */
  for (sys.nfi = 1; sys.nfi <= sys.nsteps; ++sys.nfi) {
    /* write output, if requested */
    if ((sys.nfi % nprint) == 0) output(&sys, erg, traj);
    /* propagate system and recompute energies */
    velverlet(&sys);
    /* compute forces and potential energy */
    force(&sys);
    /*call the second part to propagate*/
    velverlet_prop(&sys);
    ekin(&sys);
  }
  }
  /* Clean up: close files, free memory */
  printf("Simulation Done.");
  fclose(erg);
  fclose(traj);

  free(sys.rx);
  free(sys.ry);
  free(sys.rz);
  free(sys.vx);
  free(sys.vy);
  free(sys.vz);
  free(sys.fx);
  free(sys.fy);
  free(sys.fz);

  #ifdef MY_MPI
    MPI_Finalize();
  #endif
  return 0;
}
