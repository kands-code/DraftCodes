#include "cmatrix/cmatrix.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t mCnt = 0;
  MatrixT **mats = matrixFromFile("./data/read.mdf", &mCnt);
  MatrixT *A = mats[0];
  MatrixT *B = mats[1];
  MatrixT *mulM = matrixMul(A, B);
  matrixPrint(mulM);

  return 0;
}