#include "structs.h"

#ifndef COMPUTE_H
  #define COMPUTE_H
  #ifdef __cplusplus
  extern "C" {
  #endif
  void ekin(mdsys_t *);
  void force(mdsys_t *);
  #ifdef __cplusplus
  }
  #endif
#endif