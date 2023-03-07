/// @file: cmatrix/typeM.c
/// @descript: types of matrix

// -- INCLUDE

#include "cmatrix/cmatrix.h"
#include "cmatrix/utils.h"

#include <stdbool.h>
#include <stddef.h>

// -- FUNC

/// matrix types

/// @func: matrixIsSymmetricMatrix
/// >> check if the matrix is symmetric
/// @param: {mat} the matrix [ MatrixT * ]
/// @return: the result [ bool ]
bool matrixIsSymmetricMatrix(MatrixT *mat) {
  IS_NULL(mat);

  if (mat->size[0] != mat->size[1]) {
    return false;
  }

  for (size_t i = 1; i < mat->size[0]; ++i) {
    for (size_t j = 1; j < mat->size[1]; ++j) {
      if (!isZero(matrixGet(i, j, mat) - matrixGet(j, i, mat))) {
        return false;
      }
    }
  }

  return true;
}