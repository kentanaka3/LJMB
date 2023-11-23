/*
*header for data structures and prototypes
*/

/* structure to hold the complete information
 * about the MD system */
struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct _mdsys mdsys_t;

#ifndef protos
#ifdef __cplusplus
extern "C" {
    #endif
    #define protos
    int get_a_line(FILE *fp, char *buf);
    double wallclock();
    void azzero(double *d, const int n);
    double pbc(double x, const double boxby2);
    void ekin(mdsys_t *sys);
    void force(mdsys_t *sys);
    void velverlet(mdsys_t *sys);
    void init();
    void output(mdsys_t *sys, FILE *erg, FILE *traj)
    #ifdef __cplusplus
    }
    #endif
#endif
