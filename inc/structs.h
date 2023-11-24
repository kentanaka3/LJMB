/* Header for Data Structures and Constants */

/* generic file- or pathname buffer length */
#define BLEN 200
#define LJMD_VERSION 1.0

/* a few physical constants */
const double kboltz = 0.0019872067;     /* Boltzman constant in kcal/mol/K */
const double mvsq2e = 2390.05736153349; /* m*v^2 in kcal/mol */

/* structure to hold the complete information about the MD system */
struct _mdsys {
  int natoms,nfi,nsteps;
  double dt, mass, epsilon, sigma, box, rcut;
  double ekin, epot, temp;
  double *rx, *ry, *rz;
  double *vx, *vy, *vz;
  double *fx, *fy, *fz;
};
typedef struct _mdsys mdsys_t;
