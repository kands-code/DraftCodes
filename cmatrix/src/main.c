#include "cmatrix/cmatrix.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>

int main() {
  MatrixT *m1 = matrixFromInput();
  MatrixT *m2 = matrixFromInput();

  MatrixT *prod = matrixMul(m1, m2);
  matrixPrint(prod);

  matrixDrop(prod);
  matrixDrop(m2);
  matrixDrop(m1);
  return 0;
}