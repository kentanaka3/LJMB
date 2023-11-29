#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

/* clean up: close files, free memory */
void cleanup(FILE *erg, FILE *traj, mdsys_t sys) {
  #ifdef MY_MPI
  if (!sys.mpirank) {
  #endif
  printf("Simulation Done.\n");
  fclose(erg);
  fclose(traj);
  #ifdef MY_MPI
  }
  #endif
  free(sys.rx);
  free(sys.ry);
  free(sys.rz);

  free(sys.vx);
  free(sys.vy);
  free(sys.vz);

  free(sys.fx);
  free(sys.fy);
  free(sys.fz);

  free(sys.cx);
  free(sys.cy);
  free(sys.cz);
}