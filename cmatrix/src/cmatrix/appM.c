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

  MatrixT **lu = calloc(2, sizeof(MatrixT *));
  MatrixT *transM = matrixIdentity(mat->size[0], mat->size[0]);
  MatrixT *copyM = matrixCopy(mat);
  for (size_t i = 1, indent = 0;
       i < copyM->size[0] && i + indent <= copyM->size[1]; ++i) {
    // check pivot
    while (i + indent <= copyM->size[1] &&
           isZero(matrixGet(i, i + indent, copyM))) {
      size_t targetRow = i + 1;
      // find the non-zero row
      for (size_t row = targetRow; row <= copyM->size[0]; ++row) {
        if (!isZero(matrixGet(row, i, copyM))) {
          targetRow = row;
          break;
        } else if (row == copyM->size[0]) {
          fputs("Warn: this is a strange matrix\n", stderr);
          indent++;
        }
      }
      // create the exchange matrix
      MatrixT *exchangeRow = matrixIdentity(mat->size[0], mat->size[0]);
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
    MatrixT *elimM = matrixIdentity(mat->size[0], mat->size[0]);
    for (size_t row = i + 1; row <= copyM->size[0]; ++row) {
      complex float divisor =
          -matrixGet(row, i + indent, copyM) / matrixGet(i, i + indent, copyM);
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
  MatrixT *diagM = matrixIdentity(mat->size[0], mat->size[0]);
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    complex float val = matrixGet(i, i, lu[1]);
    if (isZero(val)) {
      fputs("Error: strange matrix cannot do LDU decompose!\n", stderr);
      exit(EXIT_FAILURE);
    }
    matrixSet(i, i, diagM, val);
  }
  ldu[1] = diagM;
  MatrixT *toOneM = matrixIdentity(mat->size[0], mat->size[0]);
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    complex float val = matrixGet(i, i, lu[1]);
    matrixSet(i, i, toOneM, 1 / val);
  }
  MatrixT *uM = matrixMul(toOneM, lu[1]);
  ldu[2] = uM;

  matrixDrop(toOneM);
  matrixLUDrop(lu);

  return ldu;
}

/// @func: matrixSolveLinearEquations
/// >> solve the basic linear equations
/// @param: {A} the coefficient matrix [ MatrixT * ]
/// @param: {b} the value matrix [ MatrixT * ]
/// @return: the solution of the linear equations [ MatrixT * ]
MatrixT *matrixSolveLinearEquations(MatrixT *A, MatrixT *b) {
  IS_NULL(A);
  IS_NULL(b);

  if (A->size[0] != A->size[1]) {
    fputs("Error: can only solve basic linear equations!\n", stderr);
    exit(EXIT_FAILURE);
  } else if (A->size[0] != b->size[0]) {
    fprintf(stderr,
            "Error: the number of equations %zu can not match the number of "
            "values %zu!\n",
            A->size[0], b->size[0]);
    exit(EXIT_FAILURE);
  }

  MatrixT *invA = matrixInverse(A);
  MatrixT *solM = matrixMul(invA, b);
  matrixDrop(invA);

  return solM;
}
