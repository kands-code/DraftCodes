/// @file: cmatrix/initM.c
/// @descript: init matrix

// -- INCLUDE

#include "cmatrix/cmatrix.h"
#include "cmatrix/utils.h"
#include <complex.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// -- FUNC

/// init matrix

/// @func: matrixZero
/// >> get a zero matrix with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the zero matrix [ MatrixT * ]
MatrixT *matrixZero(size_t row, size_t col) {
  MatrixT *mat = malloc(sizeof(MatrixT));
  mat->data = calloc(row * col, sizeof(complex float));
  mat->size[0] = row;
  mat->size[1] = col;
  for (size_t i = 0; i < row * col; ++i) {
    mat->data[i] = CMPLXF(0.0f, 0.0f);
  }
  return mat;
}

/// @func: matrixOne
/// >> get a matrix filled with `1` with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the matrix [ MatrixT * ]
MatrixT *matrixOne(size_t row, size_t col) {
  MatrixT *mat = malloc(sizeof(MatrixT));
  mat->data = calloc(row * col, sizeof(complex float));
  mat->size[0] = row;
  mat->size[1] = col;
  for (size_t i = 0; i < row * col; ++i) {
    mat->data[i] = CMPLXF(1.0f, 0.0f);
  }
  return mat;
}

/// @func: matrixIdentity
/// >> get a identity matrix with specific size
/// @param: {row} the row size [ size_t ]
/// @param: {col} the col size [ size_t ]
/// @return: the matrix [ MatrixT * ]
MatrixT *matrixIdentity(size_t row, size_t col) {
  size_t min = MIN(row, col);
  MatrixT *mat = matrixZero(row, col);
  for (size_t i = 1; i <= min; ++i) {
    matrixSet(i, i, mat, CMPLXF(1.0f, 0.0f));
  }
  return mat;
}

/// @func: matrixFromInput
/// @return: the matrix [ MatrixT * ]
/// @descript:
///   * input format follow `r + i`
///   * must have the `+` sign
///   * such as `3 - 3I` input as `3 + -3`
MatrixT *matrixFromInput() {
  size_t row, col;
  char tempChar = '\0';
  printf("matrix size: ");
  scanf("%zu %zu", &row, &col);
  fflush(stdin);
  printf("size will be %zu, %zu\n", row, col);
  MatrixT *mat = matrixZero(row, col);
  puts("input data:");
  for (size_t i = 0; i < row * col; ++i) {
    float real, imag;
    scanf("%f + %f", &real, &imag);
    mat->data[i] = CMPLXF(real, imag);
  }
  return mat;
}

/// @func: matrixFromFile
/// >> get a matrix from file
/// @param: {filePath} the file [ const char * ]
/// @param: {matCnt} the number of matrices [ size_t ]
/// @return: the matrices [ MatrixT ** ]
MatrixT **matrixFromFile(const char *filePath, size_t *matCnt) {
  // check path
  IS_NULL(filePath);
  isValidFilePath(filePath);
  // init matrices
  size_t arrLen = INIT_ARR_LENGTH;
  MatrixT **mats = calloc(arrLen, sizeof(MatrixT *));
  // open file
  FILE *fileHandle = fopen(filePath, "r");
  if (NULL == fileHandle) {
    fprintf(stderr, "Error: can not open %s\n", filePath);
    exit(EXIT_FAILURE);
  }

  struct stat buffer = {0};
  if (stat(filePath, &buffer) == -1) {
    perror("create buffer ");
    exit(EXIT_FAILURE);
  }
  // read line by line
  char *fileBuffer = malloc(buffer.st_size);
  size_t matNumber = -1;
  while ((fscanf(fileHandle, "%[^\n] ", fileBuffer) != EOF)) {
    if ('[' == fileBuffer[0]) {
      // check tag
      char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "[%[^]\n] ", configTag);
      if (strcmp("Matrix", configTag)) {
        fprintf(stderr, "Error: wrong content in matrix data file: %s\n",
                configTag);
        exit(EXIT_FAILURE);
      }
      // auto resize
      if (matNumber + 2 > arrLen) {
        arrLen *= 2;
        mats = reallocarray(mats, arrLen, sizeof(MatrixT *));
      }
      matNumber++;
      free(configTag);
    } else {
      // get key and value
      char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
      char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);
      // get value
      if (!strcmp("size", configKey)) {
        mats[matNumber] = malloc(sizeof(MatrixT));
        if (sscanf(configVal, "%zu %zu", &mats[matNumber]->size[0],
                   &mats[matNumber]->size[1]) != EOF) {
          mats[matNumber]->data =
              calloc(mats[matNumber]->size[0] * mats[matNumber]->size[1],
                     sizeof(complex float));
        } else {
          fprintf(stderr, "Error: failed to parse matrix data\n");
        }

      } else if (!strcmp("data", configKey)) {
        char *valTemp = configVal;
        size_t cnt = 0;
        char *value = calloc(MAX_STR_LENGTH, sizeof(char));
        while (sscanf(valTemp, "%[^\n ] ", value) != EOF) {
          if (cnt > mats[matNumber]->size[0] * mats[matNumber]->size[1]) {
            fprintf(stderr, "Error: too many values here!\n");
            exit(EXIT_FAILURE);
          }
          float real = 0.0f;
          float imag = 0.0f;
          sscanf(value, "%f+%f", &real, &imag);
          mats[matNumber]->data[cnt++] = CMPLXF(real, imag);
          valTemp = valTemp + strlen(value) + 1;
        }
        free(value);
      } else {
        fprintf(stderr, "Error: wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(configKey);
      free(configVal);
    }
  }
  // save state
  *matCnt = matNumber + 1;
  // some clear
  free(fileBuffer);
  fclose(fileHandle);
  return mats;
}

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
///   * data = 1 2 3 4
///   *
///   * will save as `matFile.mdf`
///   * file will truncate
void matrixToFile(const char *filePath, size_t matCnt, MatrixT *mats[matCnt]) {
  IS_NULL(filePath);
  isValidFilePath(filePath);
  FILE *fileHandle = fopen(filePath, "w");
  for (size_t i = 0; i < matCnt; ++i) {
    fputs("[Matrix]\n", fileHandle);
    fprintf(fileHandle, "size = %zu %zu\n", mats[i]->size[0], mats[i]->size[1]);
    fputs("data =", fileHandle);
    for (size_t j = 0; j < mats[i]->size[0] * mats[i]->size[1]; ++j) {
      fprintf(fileHandle, " %.3f+%.3f", crealf(mats[i]->data[j]),
              cimagf(mats[i]->data[j]));
    }
    fputs("\n\n", fileHandle);
  }
  fclose(fileHandle);
}
