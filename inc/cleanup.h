#include "structs.h"
#include <stdio.h>

#ifndef CLEANUP_H
#define CLEANUP_H
  #ifdef __cplusplus
  extern "C" {
  #endif
  void cleanup(FILE *,FILE *,mdsys_t *);
  #ifdef __cplusplus
  }
  #endif
#endif