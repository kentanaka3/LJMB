#include <stdio.h>
#include <stdlib.h>

#ifndef UTILITIES_H
  #define UTILITIES_H

  #ifdef __cplusplus
  extern "C" {
   #endif
      
    int get_a_line(FILE *, char *);
    void azzero(double *, const int);
    double pbc(double, const double);
  
  #ifdef __cplusplus
    }
  #endif
#endif

