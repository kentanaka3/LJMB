#ifndef VERLET_H
#define VERLET_H

#include "structs.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

    #ifdef __cplusplus
    extern "C" {
     #endif
  
      void velverlet(mdsys_t *sys);
  
    #ifdef __cplusplus
      }
    #endif
#endif