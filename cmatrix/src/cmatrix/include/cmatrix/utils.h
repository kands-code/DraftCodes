/// @file: cmatrix/utils.h
#pragma once
#ifndef __CMATRIX_UTILS_H__
#define __CMATRIX_UTILS_H__

// -- INCLUDE

#include "cmatrix/cmatrix.h"

#include <complex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// -- MACRO

/// @macro: IS_NULL
/// @descript: check the object is whether null or some
#define IS_NULL(obj)                                                           \
  do {                                                                         \
    if (NULL == obj) {                                                         \
      fprintf(stderr, "Error: (%s) null pointer error!", __func__);            \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

// -- FUNC

/// @func: isValidFilePath
/// >> check is the file path valid
/// @param: {filePath} the path [ const char * ]
extern void isValidFilePath(const char *filePath);

/// @func: isZero
/// >> check the float number is whether zero or not
/// @param: {num} the number [ complex float ]
/// @return: the result [ bool ]
extern bool isZero(complex float num);

/// @func: matrixCopy
/// >> copy a matrix
/// @param: {mat} the matrix [ Matrix * ]
/// @return: the copy of original matrix [ Matrix * ]
extern MatrixT *matrixCopy(MatrixT *mat);

#endif