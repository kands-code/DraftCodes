/// @file: cmatrix/manM.c
/// @descript: manipulate matrix

// -- INCLUDE

#include "cmatrix/cmatrix.h"
#include "cmatrix/utils.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// -- FUNC

/// manipulate matrix

/// @func: matrixPrint
/// >> print matrix with 3 alt :)
/// @param: {mat} the matrix [ MatrixT * ]
void matrixPrint(MatrixT *mat) { matrixPrintAlter(mat, 3); }

/// @func: matrixPrintAlter
/// >> print matrix (alter)
/// @param: {mat} the matrix [ MatrixT * ]
/// @param: {alt} the precision [ int ]
void matrixPrintAlter(MatrixT *mat, int alt) {
  puts("<<mat>>");
  for (size_t i = 1; i <= mat->size[0]; ++i) {
    putchar('[');
    for (size_t j = 1; j < mat->size[1]; ++j) {
      complex float val = matrixGet(i, j, mat);
      printf("(%.*f, %.*f), ", alt, crealf(val), alt, cimagf(val));
    }
    printf("(%.*f, %.*f)]\n", alt, crealf(matrixGet(i, mat->size[1], mat)), alt,
           cimagf(matrixGet(i, mat->size[1], mat)));
  }
  puts("<<mat>>");
}

/// @func: matrixGet
/// >> get the value one the specific place of a matrix
/// @param: {row} the row place [ size_t ]
/// @param: {col} the col place [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the value [ complex float ]
/// @descript: row and col start from `1`
complex float matrixGet(size_t row, size_t col, MatrixT *mat) {
  if (row > mat->size[0] || col > mat->size[1]) {
    fputs("Error: out of boundary!", stderr);
    fprintf(stderr,
            "Error: you want to access (%zu, %zu) on a matrix"
            " with size (%zu, %zu)\n",
            row, col, mat->size[0], mat->size[1]);
    exit(EXIT_FAILURE);
  }
  return mat->data[(row - 1) * mat->size[1] + (col - 1)];
}

/// @func: matrixSet
/// >> set the value one the specific place of a matrix
/// @param: {row} the row place [ size_t ]
/// @param: {col} the col place [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @param: {val} the value [ complex float ]
/// @descript:
///   * if return true, the modify succeeded
///   * if return false, the modify failed
///   * row and col start from `1`
void matrixSet(size_t row, size_t col, MatrixT *mat, complex float val) {
  if (row > mat->size[0] || col > mat->size[1]) {
    fputs("Error: out of boundary!", stderr);
    fprintf(stderr,
            "Error: you want to modify (%zu, %zu) on a matrix"
            " with size (%zu, %zu)\n",
            row, col, mat->size[0], mat->size[1]);
    exit(EXIT_FAILURE);
  }
  mat->data[(row - 1) * mat->size[1] + (col - 1)] = val;
}

/// @func: matrixAdd
/// >> add two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrux [ MatrixT * ]
/// @return: the sum [ MatrixT * ]
MatrixT *matrixAdd(MatrixT *m1, MatrixT *m2) {
  IS_NULL(m1);
  IS_NULL(m2);
  if (m1->size[0] != m2->size[0] || m1->size[1] != m2->size[1]) {
    fputs("Error: can not match the size!", stderr);
    exit(EXIT_FAILURE);
  }
  MatrixT *sumM = matrixZero(m1->size[0], m2->size[1]);
  for (size_t i = 1; i <= m1->size[0]; ++i) {
    for (size_t j = 1; j <= m2->size[1]; ++j) {
      matrixSet(i, j, sumM, matrixGet(i, j, m1) + matrixGet(i, j, m2));
    }
  }
  return sumM;
}

/// @func: matrixSub
/// >> substract a matrix from another matrix
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrux [ MatrixT * ]
/// @return: the rest [ MatrixT * ]
MatrixT *matrixSub(MatrixT *m1, MatrixT *m2) {
  MatrixT *negM = matrixNegate(m2);
  MatrixT *restM = matrixAdd(m1, negM);
  matrixDrop(negM);
  return restM;
}

/// @func: matrixMul
/// >> multiply two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrux [ MatrixT * ]
/// @return: the product [ MatrixT * ]
/// @descript:
///   * a vector inner product could be <a|b>
///   * |a> is a column vector <a| is |a>^(T*)
///   * example, |a> = (1, 2)^T, <a| = (1, 2),
///   * <a|a> = 5 = norm(|a>)^2
MatrixT *matrixMul(MatrixT *m1, MatrixT *m2) {
  IS_NULL(m1);
  IS_NULL(m2);
  if (m1->size[1] != m2->size[0]) {
    fputs("Error: can not match the size!", stderr);
    exit(EXIT_FAILURE);
  }
  MatrixT *prodM = matrixZero(m1->size[0], m2->size[1]);
  for (size_t i = 1; i <= prodM->size[0]; ++i) {
    for (size_t k = 1; k <= prodM->size[1]; ++k) {
      for (size_t j = 1; j <= m1->size[1]; ++j) {
        matrixSet(i, k, prodM,
                  matrixGet(i, k, prodM) +
                      matrixGet(i, j, m1) * matrixGet(j, k, m2));
      }
    }
  }
  return prodM;
}

/// @func: matrixInnerProduct
/// >> get the inner product of two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrux [ MatrixT * ]
/// @return: the inner product [ MatrixT * ]
/// @descript:
///   * the size of the two matrices must be matched
MatrixT *matrixInnerProduct(MatrixT *m1, MatrixT *m2) {
  IS_NULL(m1);
  IS_NULL(m2);
  if (m1->size[1] != m2->size[1]) {
    fputs("Error: can not match the size!", stderr);
    exit(EXIT_FAILURE);
  }
  MatrixT *conjTransM = matrixHermitianConjugate(m1);
  MatrixT *innerProdM = matrixMul(conjTransM, m2);
  matrixDrop(conjTransM);
  return innerProdM;
}
