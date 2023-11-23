#include "structs.h"

#ifndef CLEANUP_H
#define CLEANUP_H
  #ifdef __cplusplus
  extern "C" {
   #endif
      
    void cleanup(FILE erg,FILE traj,mdsys_t sys);
  
  #ifdef __cplusplus
    }
   #endif
#endif