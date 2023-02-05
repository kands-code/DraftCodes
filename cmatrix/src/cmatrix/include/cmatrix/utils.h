/// @file: cmatrix/utils.h
#pragma once
#include "cmatrix/cmatrix.h"
#ifndef __CMATRIX_UTILS_H__
#define __CMATRIX_UTILS_H__

// -- INCLUDE

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
/// @param: {num} the number [ float ]
/// @return: 1 for true, 0 for false
extern _Bool isZero(float num);

#endif