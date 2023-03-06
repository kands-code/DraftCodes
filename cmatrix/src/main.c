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
  MatrixT *m = matrixMul(A, B);
  MatrixT *hM = matrixHermitianConjugate(m);
  matrixPrint(hM);

  return 0;
}