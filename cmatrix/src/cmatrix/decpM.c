/// @file: cmatrix/decpM.c
/// @descript: decompose of matrix

// -- INCLUDE

#include "cmatrix/cmatrix.h"
#include "cmatrix/utils.h"

#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// -- FUNC

/// matrix decompose

/// @func: matrixLUDecompose
/// >> decompose a matrix into LU
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the result of decompose [ MatrixT ** ]
/// @descript: the result is L^{-1} U, not L U
MatrixT **matrixLUDecompose(MatrixT *mat) {
  IS_NULL(mat);
  if (mat->size[0] != mat->size[1]) {
    fputs("Error: non-square matrix can not do decompose!\n", stderr);
    exit(EXIT_FAILURE);
  }

  MatrixT **lu = calloc(2, sizeof(MatrixT *));
  MatrixT *transM = matrixIdentity(mat->size[0], mat->size[1]);
  MatrixT *copyM = matrixCopy(mat);

  for (size_t i = 1; i < copyM->size[0]; ++i) {
    // check pivot
    if (isZero(matrixGet(i, i, copyM))) {
      size_t targetRow = i + 1;
      // find the non-zero row
      for (size_t row = targetRow; row <= copyM->size[0]; ++row) {
        if (!isZero(matrixGet(row, i, copyM))) {
          targetRow = row;
          break;
        } else if (row == copyM->size[0]) {
          fputs("Error: strange matrix can not do\n", stderr);
          exit(EXIT_FAILURE);
        }
      }
      // create the exchange matrix
      MatrixT *exchangeRow = matrixIdentity(mat->size[0], mat->size[1]);
      matrixSet(i, i, exchangeRow, CMPLXF(0.0f, 0.0f));
      matrixSet(i, targetRow, exchangeRow, CMPLXF(1.0f, 0.0f));
      matrixSet(targetRow, targetRow, exchangeRow, CMPLXF(0.0f, 0.0f));
      matrixSet(targetRow, i, exchangeRow, CMPLXF(1.0f, 0.0f));
      // apply exchange matrix
      MatrixT *newTransM = matrixMul(exchangeRow, transM);
      matrixDrop(transM);
      transM = newTransM;
      MatrixT *newCopyM = matrixMul(exchangeRow, copyM);
      matrixDrop(copyM);
      copyM = newCopyM;
      matrixDrop(exchangeRow);
    }
    // do elimilation
    MatrixT *elimM = matrixIdentity(mat->size[0], mat->size[1]);
    for (size_t row = i + 1; row <= copyM->size[0]; ++row) {
      complex float divisor =
          -matrixGet(row, i, copyM) / matrixGet(i, i, copyM);
      matrixSet(row, i, elimM, divisor);
    }
    // apply elimilation
    MatrixT *newTransM = matrixMul(elimM, transM);
    matrixDrop(transM);
    transM = newTransM;
    MatrixT *newCopyM = matrixMul(elimM, copyM);
    matrixDrop(copyM);
    copyM = newCopyM;
    matrixDrop(elimM);
  }

  lu[0] = transM;
  lu[1] = copyM;
  return lu;
}

/// @func: matrixLDUDecompose
/// >> decompose a matrix into LDU
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the result of decompose [ MatrixT ** ]
/// @descript: the result is L^{-1} D U, not L D U
MatrixT **matrixLDUDecompose(MatrixT *mat) {
  MatrixT **lu = matrixLUDecompose(mat);
  MatrixT **ldu = calloc(3, sizeof(MatrixT *));
  ldu[0] = matrixCopy(lu[0]);
  MatrixT *diagM = matrixIdentity(mat->size[0], mat->size[1]);
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    complex float val = matrixGet(i, i, lu[1]);
    if (!isZero(val)) {
      matrixSet(i, i, diagM, val);
    }
  }
  ldu[1] = diagM;
  MatrixT *toOneM = matrixIdentity(mat->size[0], mat->size[1]);
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    complex float val = matrixGet(i, i, lu[1]);
    if (!isZero(val)) {
      matrixSet(i, i, toOneM, 1 / val);
    }
  }
  MatrixT *uM = matrixMul(toOneM, lu[1]);
  ldu[2] = uM;

  matrixDrop(toOneM);
  matrixLUDrop(lu);

  return ldu;
}