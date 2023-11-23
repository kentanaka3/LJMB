#ifndef protos
#ifdef __cplusplus
extern "C" {
    #endif
    #define protos  
    int get_a_line(FILE *fp, char *buf);
    void azzero(double *d, const int n);
    double pbc(double x, const double boxby2);
    #ifdef __cplusplus
}
#endif
#endif

