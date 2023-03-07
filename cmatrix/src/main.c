#include "cmatrix/cmatrix.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  MatrixT *mat = matrixFromInput();
  MatrixT **lu = matrixLUDecompose(mat);
  matrixPrint(lu[0]);
  matrixPrint(lu[1]);
  matrixLUDrop(lu);
  matrixDrop(mat);

  return 0;
}
