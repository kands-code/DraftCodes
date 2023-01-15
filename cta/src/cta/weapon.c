/// @file: cta/weapon.c

// -- INCLUDE

#include "cta/game.h"
#include "cta/option.h"
#include "cta/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// -- FUNC

/// @func: loadItemConfig
/// >> load item config
/// @param: {path} the path of the file
void loadItemConfig(const char *path) {
  checkPathIsValid(path);
  FILE *fileHandle = fopen(path, "r");

  if (NULL == fileHandle) {
    fprintf(stderr, "item config file cannot find!\n");
    fprintf(stderr, "generate default item config...\n");
    generateDefaultItem(path);

    // reopen file
    fileHandle = fopen(path, "r");
    if (NULL == fileHandle) {
      perror("open item config ");
      exit(EXIT_FAILURE);
    }
  }

  struct stat buffer = {0};
  if (stat(path, &buffer) == -1) {
    perror("create buffer ");
    exit(EXIT_FAILURE);
  }
  // read line by line
  char *fileBuffer = malloc(buffer.st_size);
  size_t wpCount = -1; // use wrap around
  while ((fscanf(fileHandle, "%[^\n] ", fileBuffer) != EOF)) {
    if ('[' == fileBuffer[0]) {
      char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "[%[^]\n] ", configTag);
      if (strcmp("Weapon", configTag)) {
        fprintf(stderr, "wrong content in glob config: %s\n", configTag);
        exit(EXIT_FAILURE);
      } else if (wpCount + 1 > MAX_ITEM_COUNT) {
        fputs("too many weapons!", stderr);
        exit(EXIT_FAILURE);
      }
      wpCount++;
      free(configTag);
    } else {
      char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
      char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);
      if (!strcmp("name", configKey)) {
        strncpy(Weapons[wpCount].name, configVal, strlen(configVal));
      } else if (!strcmp("damage", configKey)) {
        Weapons[wpCount].damage = atoi(configVal);
      } else if (!strcmp("type", configKey)) {
        Weapons[wpCount].type = atoi(configVal);
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(configKey);
      free(configVal);
    }
  }
  AllCount[2] = wpCount + 1;
  free(fileBuffer);
  fclose(fileHandle);
}

/// @func: generateDefaultItem
/// >> generate default item config file
/// @param: {path} the path of the file
void generateDefaultItem(const char *path) {
  FILE *fileHandle = fopen(path, "ax+");
  if (NULL == fileHandle) {
    perror("create default place config file ");
    exit(EXIT_FAILURE);
  }
  // punch
  fputs("[Weapon]\n", fileHandle);
  fputs("name = punch\n", fileHandle);
  fputs("damage = 3\n", fileHandle);
  fputs("type = 3\n", fileHandle);
  // fire broadsword
  fputs("[Weapon]\n", fileHandle);
  fputs("name = firebroadsword\n", fileHandle);
  fputs("damage = 5\n", fileHandle);
  fputs("type = 0\n", fileHandle);
  // water whip
  fputs("[Weapon]\n", fileHandle);
  fputs("name = waterwhip\n", fileHandle);
  fputs("damage = 6\n", fileHandle);
  fputs("type = 0\n", fileHandle);
  // grass knife
  fputs("[Weapon]\n", fileHandle);
  fputs("name = grassknife\n", fileHandle);
  fputs("damage = 4\n", fileHandle);
  fputs("type = 0\n", fileHandle);
  // light sword
  fputs("[Weapon]\n", fileHandle);
  fputs("name = lightsword\n", fileHandle);
  fputs("damage = 5\n", fileHandle);
  fputs("type = 0\n", fileHandle);
  // dark dagger
  fputs("[Weapon]\n", fileHandle);
  fputs("name = darkdagger\n", fileHandle);
  fputs("damage = 7\n", fileHandle);
  fputs("type = 0\n", fileHandle);

  fclose(fileHandle);
}
