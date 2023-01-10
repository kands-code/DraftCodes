/// @file: cmatrix/cmatrix.h
#pragma once
#ifndef __CMATRIX_H__
#define __CMATRIX_H__ 1

// -- INCLUDE

#include <stddef.h>

/// @type: MatrixT
/// > the type of matrix
/// @content: {data} the data of the matrix [ double * ]
/// @content: {size} the size of the matrix [ size_t[2] ]
/// @descript:
///   * the data default use double
///   * the size is (row, col)
typedef struct MatrixT {
  double *data;
  size_t size[2];
} MatrixT;

#endif