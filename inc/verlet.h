#include "structs.h"

#ifndef VERLET_H
#define VERLET_H
  #ifdef __cplusplus
  extern "C" {
   #endif
      void verlet_part1(mdsys_t *);
      void verlet_part2(mdsys_t *);
  
  #ifdef __cplusplus
    }
   #endif
#endif