#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

/* clean up: close files, free memory */
void cleanup(FILE *erg, FILE *traj, mdsys_t sys) {
  if (!sys.mpirank) {
    printf("Simulation Done.\n");
    fclose(erg);
    fclose(traj);
  }
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