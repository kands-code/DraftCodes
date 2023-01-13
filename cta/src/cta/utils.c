/// @file: cta/utils.c

// -- INCLUDE

#include "cta/utils.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

// -- FUNC

/// @func: commandLineHelper
/// >> print help info to screen
void commandLineHelper() {
  puts("\ncta-exe");
  puts(" A Textural Advanture Game Written in C");
  puts("options:");
  puts("  -c DIRS");
  puts("     determine the config directory");
  puts("     the DIRS `MUST` be the absolute path\n");
}

/// @func: checkPathIsValid
/// >> check the path whether is valid or not
/// @param: <dirPath> the path to check [ const char * ]
/// @return: the check result [ _Bool ]
/// @descript:
///   * the fuction will get error if the path is `NULL`
_Bool checkPathIsValid(const char *dirPath) {
  if (NULL == dirPath) {
    return 0;
  }

  regex_t rgx;
  int reti = regcomp(&rgx, "^(~|[\\.]{,2})?(/[.-_a-zA-Z0-9]*)*$", REG_EXTENDED);
  if (reti) {
    fputs("wrong regex expression!\n", stderr);
    exit(EXIT_FAILURE);
  }
  reti = regexec(&rgx, dirPath, 0, NULL, 0);
  if (reti) {
    fputs("invalid path!\n", stderr);
    exit(EXIT_FAILURE);
  }

  return 1;
}