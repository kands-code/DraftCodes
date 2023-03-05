/// @file: cmatrix/attrM.c
/// @descript: attribute of matrix

// -- INCLUDE

#include "cmatrix/cmatrix.h"
#include "cmatrix/utils.h"
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// -- FUNC

/// attribute of matrix

/// @func: matrixNegate
/// >> negate all the elements of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the negate matrix [ MatrixT * ]
MatrixT *matrixNegate(MatrixT *mat) {
  IS_NULL(mat);
  MatrixT *negMat = matrixZero(mat->size[0], mat->size[1]);
  for (size_t i = 0; i < negMat->size[0] * negMat->size[1]; ++i) {
    negMat->data[i] = -mat->data[i];
  }
  return negMat;
}

/// @func: matrixTranspose
/// >> transpose a matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the transposed matrix [ MatrixT * ]
MatrixT *matrixTranspose(MatrixT *mat) {
  IS_NULL(mat);

  MatrixT *transMat = matrixZero(mat->size[1], mat->size[0]);
  for (size_t i = 1; i <= transMat->size[0]; ++i) {
    for (size_t j = 1; j <= transMat->size[1]; ++j) {
      matrixSet(i, j, transMat, matrixGet(j, i, mat));
    }
  }
  return transMat;
}

/// @func: matrixDeterminant
/// >> get the determinant of a matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the determinant [ complex float ]
complex float matrixDeterminant(MatrixT *mat) {
  IS_NULL(mat);
  if (mat->size[0] != mat->size[1]) {
    fputs("Error: non-square matrix do not have normal determinant!\n", stderr);
    exit(EXIT_FAILURE);
  }
  MatrixT **lu = matrixLUDecompose(mat);
  complex float det = CMPLXF(1.0f, 0.0f);
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    det *= matrixGet(i, i, lu[1]);
  }
  matrixLUDrop(lu);
  return det;
}

/// @func: matrixRow
/// >> get the specific row of a matrix
/// @param: {row} the row emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the row of the original matrix [ MatrixT * ]
MatrixT *matrixRow(size_t row, MatrixT *mat) {
  IS_NULL(mat);
  if (row == 0 || row > mat->size[0]) {
    fputs("Error: out of boundary!\n", stderr);
    fprintf(stderr,
            "Error: you want to access the %zu row on a matrix"
            " with size (%zu, %zu)\n",
            row, mat->size[0], mat->size[1]);
    exit(EXIT_FAILURE);
  }
  MatrixT *matRow = matrixZero(1, mat->size[1]);
  for (size_t i = 1; i <= mat->size[1]; ++i) {
    matrixSet(1, i, matRow, matrixGet(row, i, mat));
  }
  return matRow;
}

/// @func: matrixCol
/// >> get the specific column of a matrix
/// @param: {col} the column emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the column of the original matrix [ MatrixT * ]
MatrixT *matrixCol(size_t col, MatrixT *mat) {
  IS_NULL(mat);
  if (col == 0 || col > mat->size[1]) {
    fputs("Error: out of boundary!\n", stderr);
    fprintf(stderr,
            "Error: you want to access the %zu column on a matrix"
            " with size (%zu, %zu)\n",
            col, mat->size[0], mat->size[1]);
    exit(EXIT_FAILURE);
  }
  MatrixT *matCol = matrixZero(mat->size[0], 1);
  for (size_t i = 1; i <= mat->size[1]; ++i) {
    matrixSet(i, 1, matCol, matrixGet(i, col, mat));
  }
  return matCol;
}

/// @func: matrixSubmatrix
/// >> get the submatrix of a matrix
/// @param: {row} the row emit [ size_t ]
/// @param: {col} the column emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the submatrix of the original matrix [ MatrixT * ]
MatrixT *matrixSubmatrix(size_t row, size_t col, MatrixT *mat) {
  if (1 == mat->size[0] || 1 == mat->size[1]) {
    fprintf(stderr, "Error: matrix of size (%zu, %zu) has no submatrix!\n",
            mat->size[0], mat->size[1]);
    exit(EXIT_FAILURE);
  }
  MatrixT *subM = matrixZero(mat->size[0] - 1, mat->size[1] - 1);
  size_t cntRow = 1;
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    if (i == row) {
      continue;
    }
    size_t cntCol = 1;
    for (size_t j = 1; j <= mat->size[1]; ++j) {
      if (j == col) {
        continue;
      }
      matrixSet(cntRow, cntCol, subM, matrixGet(i, j, mat));
      cntCol++;
    }
    cntRow++;
  }
  return subM;
}

/// @func: matrixCofactorMatrix
/// >> get the cofactor matrix of a matrix
/// @param: {mat} the matrix
/// @return: the cofactor matrix of the original matrix [ MatrixT * ]
MatrixT *matrixCofactorMatrix(MatrixT *mat) {
  MatrixT *coM = matrixZero(mat->size[0], mat->size[1]);

  for (size_t i = 1; i <= coM->size[0]; ++i) {
    for (size_t j = 1; j <= coM->size[1]; ++j) {
      MatrixT *subM = matrixSubmatrix(i, j, mat);
      complex float det = matrixDeterminant(subM);
      matrixSet(i, j, coM, (i + j) & 1 ? -det : det);
      matrixDrop(subM);
    }
  }

  return coM;
}

/// @func: matrixAdjugate
/// >> get the adjugate matrix of a matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the adjugate matrix [ MatrixT * ]
MatrixT *matrixAdjugate(MatrixT *mat) {
  MatrixT *coM = matrixCofactorMatrix(mat);
  MatrixT *adjM = matrixTranspose(coM);
  matrixDrop(coM);
  return adjM;
}

/// @func: matrixInverse
/// >> get the inverse of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the inverse matrix [ MatrixT * ]
MatrixT *matrixInverse(MatrixT *mat) {
  complex float det = matrixDeterminant(mat);
  if (isZero(det)) {
    fputs("Error: matrix with determinant which is zero have no inverse!\n",
          stderr);
    exit(EXIT_FAILURE);
  }
  MatrixT *adjM = matrixAdjugate(mat);
  MatrixT *invM = matrixScaleMul(CMPLXF(1.0f, 0.0f) / det, adjM);
  // matrixDrop(adjM);
  return invM;
}

/// @func: matrixHermitianConjugate
/// >> get the hermitian conjugate of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the hermitian conjugate of original matrix [ MatrixT * ]
MatrixT *matrixHermitianConjugate(MatrixT *mat) {
  IS_NULL(mat);
  MatrixT *hermMat = matrixTranspose(mat);
  for (size_t i = 1; i <= hermMat->size[0]; ++i) {
    for (size_t j = 1; j <= hermMat->size[1]; ++j) {
      complex float val = matrixGet(i, j, hermMat);
      // conjugate
      matrixSet(i, j, hermMat, CMPLXF(crealf(val), -cimagf(val)));
    }
  }
  return hermMat;
}

/// @func: matrixDrop
/// >> destory a matrix
/// @param: {mat} the matrix [ Matrix * ]
void matrixDrop(MatrixT *mat) {
  if (NULL == mat) {
    return;
  }
  free(mat->data);
  free(mat);
}

/// @func: matrixLUDrop
/// >> destory LU matrices
/// @param: {lu} the LU matrices [ Matrix ** ]
void matrixLUDrop(MatrixT **lu) {
  if (NULL == lu) {
    return;
  }
  matrixDrop(lu[0]);
  matrixDrop(lu[1]);
  free(lu);
}

/// @func: matrixLDUDrop
/// >> destory LDU matrices
/// @param: {ldu} the LDU matrices [ Matrix ** ]
void matrixLDUDrop(MatrixT **ldu) {
  if (NULL == ldu) {
    return;
  }
  matrixDrop(ldu[0]);
  matrixDrop(ldu[1]);
  matrixDrop(ldu[2]);
  free(ldu);
}