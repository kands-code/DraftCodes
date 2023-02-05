/// @file: cmatrix/utils.c

// -- INCLUDE

#include "cmatrix/utils.h"
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

// -- FUNC

/// @func: isValidFilePath
/// >> check is the file path valid
/// @param: {filePath} the path [ const char * ]
void isValidFilePath(const char *filePath) {
  IS_NULL(filePath);
  regex_t rgx;
  int reti = regcomp(&rgx, "^(~|[\\.]{,2})?(/[.-_a-zA-Z0-9]*)*$", REG_EXTENDED);
  if (reti) {
    fputs("Error: wrong regex expression!\n", stderr);
    exit(EXIT_FAILURE);
  }
  reti = regexec(&rgx, filePath, 0, NULL, 0);
  if (reti) {
    fputs("Error: invalid path!\n", stderr);
    exit(EXIT_FAILURE);
  }
}

/// @func: isZero
/// >> check the float number is whether zero or not
/// @param: {num} the number [ float ]
/// @return: 1 for true, 0 for false
_Bool isZero(float num) {
  const float ESP = 1e-4;
  if (fabsf(num) < ESP) {
    return 1;
  }
  return 0;
}
