#include "cmatrix/cmatrix.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>

int main() {
  size_t mCnt = 0;
  MatrixT **mats = matrixFromFile("./data/read.mdf", &mCnt);
  printf("%f %f\n", crealf(matrixGet(1, 1, mats[0])),
         cimagf(matrixGet(1, 1, mats[0])));
  matrixPrint(mats[0]);

  MatrixT *subM = matrixSubmatrix(1, 1, mats[0]);
  matrixPrint(subM);

  matrixDrop(subM);
  for (size_t i = 0; i < mCnt; ++i) {
    matrixDrop(mats[i]);
  }

  return 0;
}