#include "structs.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* helper function: zero out an array */
void azzero(double *d, const int n) {
  for (int i = 0; i < n; ++i) d[i]=0.0;
}

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2) {
  while (x >  boxby2) x -= 2.0*boxby2;
  while (x < -boxby2) x += 2.0*boxby2;
  return x;
}

static int get_a_line(FILE *fp, char *buf) {
  char tmp[BLEN], *ptr;
  /* read a line and cut of comments and blanks */
  if (fgets(tmp, BLEN, fp)) {
    int i;

    ptr = strchr(tmp, '#');
    if (ptr) *ptr = '\0';
    i = strlen(tmp);
    --i;
    while (isspace(tmp[i])) {
      tmp[i] = '\0';
      --i;
    }
    ptr = tmp;
    while (isspace(*ptr)) {++ptr;}
    i = strlen(ptr);
    strcpy(buf, tmp);
    return 0;
  } else {
    perror("problem reading input");
    return -1;
  }
  return 0;
}