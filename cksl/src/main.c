#include "cksl/utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    puts("Open: REPL for KSL");
  } else if (!strcmp(argv[1], "-f")) {
    if (NULL == argv[2]) {
      fputs("Error: no file name passed!", stderr);
      return EXIT_FAILURE;
    }

    FILE *src = fopen(argv[2], "r");
    if (NULL == src) {
      fprintf(stderr, "Error: wrong file name or path: %s\n", argv[2]);
      return EXIT_FAILURE;
    }

    fseek(src, 0, SEEK_END);
    size_t size = ftell(src);
    fseek(src, 0, SEEK_SET);

    char *srcContent = calloc(size, sizeof(char));
    fread(srcContent, sizeof(char), size, src);

    char *flattenSrc = flattenSource(srcContent, size);
    puts(flattenSrc);

    free(flattenSrc);
    free(srcContent);
    fclose(src);
  } else if (!strcmp(argv[1], "-h")) {
    ckslpHelper();
  } else {
    fprintf(stderr, "Error: wrong argument: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}