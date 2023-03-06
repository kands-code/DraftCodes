/// @file: cmatrix/cmatrix.h
#pragma once
#ifndef __CMATRIX_CMATRIX_H__
#define __CMATRIX_CMATRIX_H__ 1

// -- INCLUDE

#include <complex.h>
#include <stddef.h>

// -- MACRO

/// @macro: CMPLX
/// @descript:
///   * a macro to generate complex float value
///   * copied from `complex.h` because I use clang :(
#define CMPLXF(x, y) __builtin_complex((float)(x), (float)(y))

/// @macro: MIN
/// @descript: a macro to get smaller one between the two
#define MIN(x, y) (x) > (y) ? (y) : (x)

/// @macro: MAX_STR_LENGTH
/// @descript: max length of string
#define MAX_STR_LENGTH 128

/// @macro: INIT_ARR_LENGTH
/// @descript: init length of array
#define INIT_ARR_LENGTH 16

// -- TYPE

/// @type: MatrixT
/// > the type of matrix
/// @content: {data} the data of the matrix [ complex float * ]
/// @content: {size} the size of the matrix [ size_t[2] ]
/// @descript:
///   * the data default use double
///   * the size is (row, col)
///   * vector is also a matrix
typedef struct MatrixT {
  complex float *data;
  size_t size[2];
} MatrixT;

// -- FUNC

/// init matrix

/// @func: matrixZero
/// >> get a zero matrix with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the zero matrix [ MatrixT * ]
extern MatrixT *matrixZero(size_t row, size_t col);

/// @func: matrixOne
/// >> get a matrix filled with `1` with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the matrix [ MatrixT * ]
extern MatrixT *matrixOne(size_t row, size_t col);

/// @func: matrixIdentity
/// >> get a identity matrix with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the matrix [ MatrixT * ]
extern MatrixT *matrixIdentity(size_t row, size_t col);

/// @func: matrixFromInput
/// @return: the matrix [ MatrixT * ]
/// @descript:
///   * input format follow `r sign i`
///   * such as `3 - 3I` input as `3 - 3`
extern MatrixT *matrixFromInput();

/// @func: matrixFromFile
/// >> get a matrix from file
/// @param: {filePath} the file [ const char * ]
/// @param: {matCnt} the number of matrices [ size_t ]
/// @return: the matrices [ MatrixT ** ]
extern MatrixT **matrixFromFile(const char *filePath, size_t *matCnt);

/// @func: matrixToFile
/// >> save matrices to file
/// @param: {filePath} the file [ const char * ]
/// @param: {matCnt} the number of matrices [ size_t ]
/// @param: {mats} the matrices [ Matrix*[] ]
/// @descript:
///   * matrix type sample
///   *
///   * [Matrix]
///   * size = 2 2
///   * data = 1+0 2+0 3+0 4+0
///   *
///   * will save as `matFile.mdf`
///   * file will truncate
extern void matrixToFile(const char *filePath, size_t matCnt,
                         MatrixT *mats[matCnt]);

/// attribute of matrix

/// @func: matrixNegate
/// >> negate all the elements of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the negate matrix [ MatrixT * ]
extern MatrixT *matrixNegate(MatrixT *mat);

/// @func: matrixHermitianConjugate
/// >> get the hermitian conjugate of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the hermitian conjugate of original matrix [ MatrixT * ]
extern MatrixT *matrixHermitianConjugate(MatrixT *mat);

/// @func: matrixDeterminant
/// >> get the determinant of a matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the determinant [ complex float ]
extern complex float matrixDeterminant(MatrixT *mat);

/// @func: matrixRow
/// >> get the specific row of a matrix
/// @param: {row} the row emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the row of the original matrix [ MatrixT * ]
extern MatrixT *matrixRow(size_t row, MatrixT *mat);

/// @func: matrixCol
/// >> get the specific column of a matrix
/// @param: {col} the column emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the column of the original matrix [ MatrixT * ]
extern MatrixT *matrixCol(size_t col, MatrixT *mat);

/// @func: matrixSubmatrix
/// >> get the submatrix of a matrix
/// @param: {row} the row emit [ size_t ]
/// @param: {col} the column emit [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the submatrix of the original matrix [ MatrixT * ]
extern MatrixT *matrixSubmatrix(size_t row, size_t col, MatrixT *mat);

/// @func: matrixCofactorMatrix
/// >> get the cofactor matrix of a matrix
/// @param: {mat} the matrix
/// @return: the cofactor matrix of the original matrix [ MatrixT * ]
extern MatrixT *matrixCofactorMatrix(MatrixT *mat);

/// @func: matrixAdjugate
/// >> get the adjugate matrix of a matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the adjugate matrix [ MatrixT * ]
extern MatrixT *matrixAdjugate(MatrixT *mat);

/// @func: matrixInverse
/// >> get the inverse of the matrix
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the inverse matrix [ MatrixT * ]
extern MatrixT *matrixInverse(MatrixT *mat);

/// @func: matrixDrop
/// >> destory a matrix
/// @param: {mat} the matrix [ Matrix * ]
extern void matrixDrop(MatrixT *mat);

/// @func: matrixLUDrop
/// >> destory LU matrices
/// @param: {lu} the LU matrices [ Matrix ** ]
extern void matrixLUDrop(MatrixT **lu);

/// @func: matrixLDUDrop
/// >> destory LDU matrices
/// @param: {ldu} the LDU matrices [ Matrix ** ]
extern void matrixLDUDrop(MatrixT **ldu);

/// manipulate matrix

/// @func: matrixPrint
/// >> print matrix with 3 alt :)
/// @param: {mat} the matrix [ MatrixT * ]
extern void matrixPrint(MatrixT *mat);

/// @func: matrixPrintAlter
/// >> print matrix (alter)
/// @param: {mat} the matrix [ MatrixT * ]
/// @param: {alt} the precision [ int ]
extern void matrixPrintAlter(MatrixT *mat, int alt);

/// @func: matrixGet
/// >> get the value one the specific place of a matrix
/// @param: {row} the row place [ size_t ]
/// @param: {col} the col place [ size_t ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the value [ complex float ]
/// @descript: row and col start from `1`
extern complex float matrixGet(size_t row, size_t col, MatrixT *mat);

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
extern void matrixSet(size_t row, size_t col, MatrixT *mat, complex float val);

/// @func: matrixAdd
/// >> add two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrix [ MatrixT * ]
/// @return: the sum [ MatrixT * ]
extern MatrixT *matrixAdd(MatrixT *m1, MatrixT *m2);

/// @func: matrixSub
/// >> substract a matrix from another matrix
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrix [ MatrixT * ]
/// @return: the rest [ MatrixT * ]
extern MatrixT *matrixSub(MatrixT *m1, MatrixT *m2);

/// @func: matrixCommonInnerProduct
/// >> get the common inner product of two vectors
/// @param: {v1} the first vector [ MatrixT * ]
/// @param: {v2} the second vector [ MatrixT * ]
/// @return: the inner product [ complex float ]
/// @descript:
///   * v1 is a row vector and v2 is a column vector
extern complex float matrixCommonInnerProduct(MatrixT *v1, MatrixT *v2);

/// @func: matrixScaleMul
/// >> multiply a scale and a matrix
/// @param: {scale} the number [ complex float ]
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the product [ MatrixT * ]
extern MatrixT *matrixScaleMul(complex float scale, MatrixT *mat);

/// @func: matrixMul
/// >> multiply two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrix [ MatrixT * ]
/// @return: the product [ MatrixT * ]
/// @descript:
///   * a vector inner product could be <a|b>
///   * |a> is a column vector <a| is |a>^(T*)
///   * example, |a> = (1, 2)^T, <a| = (1, 2),
///   * <a|a> = 5 = norm(|a>)^2
extern MatrixT *matrixMul(MatrixT *m1, MatrixT *m2);

/// @func: matrixInnerProduct
/// >> get the inner product of two matrices
/// @param: {m1} the first matrix [ MatrixT * ]
/// @param: {m2} the second matrix [ MatrixT * ]
/// @return: the inner product [ MatrixT * ]
/// @descript:
///   * the size of the two matrices must be matched
extern MatrixT *matrixInnerProduct(MatrixT *m1, MatrixT *m2);

/// matrix decompose

/// @func: matrixLUDecompose
/// >> decompose a matrix into LU
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the result of decompose [ MatrixT ** ]
/// @descript: the result is L^{-1} U, not L U
extern MatrixT **matrixLUDecompose(MatrixT *mat);

/// @func: matrixLDUDecompose
/// >> decompose a matrix into LDU
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the result of decompose [ MatrixT ** ]
/// @descript: the result is L^{-1} D U, not L D U
extern MatrixT **matrixLDUDecompose(MatrixT *mat);

#endif