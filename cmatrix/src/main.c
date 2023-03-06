#include "cmatrix/cmatrix.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  MatrixT *m = matrixFromInput();
  complex float det = matrixDeterminant(m);
  printf("%.3f%+.3fI\n", crealf(det), cimag(det));
  matrixDrop(m);

  return 0;
}