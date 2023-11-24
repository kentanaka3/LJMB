#include "structs.h"

#ifndef VERLET_H
#define VERLET_H
  #ifdef __cplusplus
  extern "C" {
   #endif
      void velverlet_part1(mdsys_t *);
      void velverlet_part2(mdsys_t *);
  
  #ifdef __cplusplus
    }
   #endif
#endif