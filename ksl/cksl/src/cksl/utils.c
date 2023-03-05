/// @file: cksl/utils.c
/// @descript: some util functions

// -- INCLUDE

#include "cksl/utils.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -- FUNC

/// @func: flattenSource
/// >> flatten source code to one line string
/// @param: {source} the source code [ const char * ]
/// @param: {len} the size of the source code [ size_t ]
char *flattenSource(const char *source, size_t len) {
  char *flattenSrc = calloc(len, sizeof(char));
  size_t srcLen = 0;

  for (size_t i = 0; i < strlen(source); ++i) {
    if (isspace(source[i])) {
      continue;
    } else {
      flattenSrc[srcLen++] = source[i];
    }
  }

  return flattenSrc;
}

/// @func: ckslpHelper
/// >> print help message
void ckslpHelper() {
  puts("ckslp: the intepreter for KSL");
  puts("-------------------------------");
  puts("  -f: determine the source file");
  puts("  example:");
  puts("    ckslp -f example/simple.ks");
  puts("  -h: print this message");
  puts("  example:");
  puts("    ckslp -h");
  puts("to open repl, pass no argument!");
}