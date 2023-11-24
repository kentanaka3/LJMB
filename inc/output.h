#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

#ifndef OUTPUT_H
#define OUTPUT_H
  #ifdef __cplusplus
  extern "C" {
  #endif
  void output(mdsys_t *, FILE *, FILE *);
  #ifdef __cplusplus
  }
  #endif
#endif