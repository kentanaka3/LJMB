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
#include "cleanup.h"
#ifdef MY_MPI
  #include "myMPI.hpp"
#endif

int main(int argc, char *argv[]) {
  #ifdef MY_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mype);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
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

  {CSimpleTimer t{"RunTime"};
  /* Main MD loop */
  for (sys.nfi = 1; sys.nfi <= sys.nsteps; ++sys.nfi) {
    /* write output, if requested */
    if ((sys.nfi % nprint) == 0) output(&sys, erg, traj);
    /* propagate system and recompute energies */
    {CSimpleTimer t{"Velverlet"};
    velverlet(&sys);
    }
    /* compute forces and potential energy */
    {CSimpleTimer t{"Force"};
    force(&sys);
    }
    /*call the second part to propagate*/
    {CSimpleTimer t{"Propagate"};
    velverlet_prop(&sys);
    }
    {CSimpleTimer t{"KineticEnergy"};
    ekin(&sys);
    }
  }
  }
  cleanup(erg, traj, sys);
  print_timing_results();
  #ifdef MY_MPI
    MPI_Finalize();
  #endif
  return 0;
}
