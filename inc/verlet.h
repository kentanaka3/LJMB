#include "structs.h"

#ifndef VERLET_H
#define VERLET_H
  #ifdef __cplusplus
  extern "C" {
   #endif
      void velverlet(mdsys_t *);
      void velverlet_prop(mdsys_t *);
  
  #ifdef __cplusplus
    }
   #endif
#endif