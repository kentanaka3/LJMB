#include "structs.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

void initialize(mdsys_t * sys, char trajfile[], char ergfile[], int *nprint){
  char restfile[BLEN], line[BLEN];
  FILE *fp;
  /* read input file */
  if (get_a_line(stdin, line)) exit(1);
  sys->natoms = atoi(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->mass = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->m = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->De = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->a = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->re = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->rcut = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->box = atof(line);
  if (get_a_line(stdin, restfile)) exit(1);
  if (get_a_line(stdin, trajfile)) exit(1);
  if (get_a_line(stdin, ergfile)) exit(1);
  if (get_a_line(stdin, line)) exit(1);
  sys->nsteps = atoi(line);
  if (get_a_line(stdin, line)) exit(1);
  sys->dt = atof(line);
  if (get_a_line(stdin, line)) exit(1);
  *nprint = atoi(line);

  /* allocate memory */
  sys->rx = (double *)malloc(sys->natoms*sizeof(double));
  sys->ry = (double *)malloc(sys->natoms*sizeof(double));
  sys->rz = (double *)malloc(sys->natoms*sizeof(double));
  sys->vx = (double *)malloc(sys->natoms*sizeof(double));
  sys->vy = (double *)malloc(sys->natoms*sizeof(double));
  sys->vz = (double *)malloc(sys->natoms*sizeof(double));
  sys->fx = (double *)malloc(sys->natoms*sizeof(double));
  sys->fy = (double *)malloc(sys->natoms*sizeof(double));
  sys->fz = (double *)malloc(sys->natoms*sizeof(double));

  /* read restart */
  fp = fopen(restfile, "r");
  if (fp) {
    for (int i = 0; i < sys->natoms; ++i) {
      fscanf(fp, "%lf%lf%lf", sys->rx + i, sys->ry + i, sys->rz + i);
    }
    for (int i = 0; i < sys->natoms; ++i) {
      fscanf(fp, "%lf%lf%lf", sys->vx + i, sys->vy + i, sys->vz + i);
    }
    fclose(fp);
    //
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);
  } else {
    perror("cannot read restart file");
    exit(3);
  }
}
