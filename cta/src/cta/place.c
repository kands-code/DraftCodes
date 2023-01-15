/// @file: cta/place.c

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

/// @func: loadPlaceConfig
/// >> load place config
/// @param: {path} the path of the file
void loadPlaceConfig(const char *path) {
  checkPathIsValid(path);
  FILE *fileHandle = fopen(path, "r");

  if (NULL == fileHandle) {
    fprintf(stderr, "place config file cannot find!\n");
    fprintf(stderr, "generate default place config...\n");
    generateDefaultPlace(path);

    // reopen file
    fileHandle = fopen(path, "r");
    if (NULL == fileHandle) {
      perror("open place config ");
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
  size_t placesCount = -1; // use wrap around
  while ((fscanf(fileHandle, "%[^\n] ", fileBuffer) != EOF)) {
    if ('[' == fileBuffer[0]) {
      char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "[%[^]\n] ", configTag);
      if (strcmp("Place", configTag)) {
        fprintf(stderr, "wrong content in glob config: %s\n", configTag);
        exit(EXIT_FAILURE);
      } else if (placesCount + 1 > MAX_ITEM_COUNT) {
        fputs("too many places!", stderr);
        exit(EXIT_FAILURE);
      }
      placesCount++;
      free(configTag);
    } else {
      char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
      char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);
      if (!strcmp("name", configKey)) {
        size_t len = strlen(configVal);
        strncpy(Places[placesCount].name, configVal, len);
        Places[placesCount].name[len] = '\0';
      } else if (!strcmp("monstertypes", configKey)) {
        Places[placesCount].monsterTypes = atoi(configVal);
      } else if (!strcmp("monsters", configKey)) {
        char *valTemp = configVal;
        size_t cnt = 0;
        char *elem = calloc(4, sizeof(char));
        while (sscanf(valTemp, "%[^\n ] ", elem) != EOF) {
          if (cnt > MAX_ITEM_COUNT - 1) {
            fprintf(stderr, "too many items in bag!\n");
            exit(EXIT_FAILURE);
          }
          Places[placesCount].monsters[cnt++] = atoi(elem);
          valTemp = valTemp + strlen(elem) + 1;
        }
        free(elem);
      } else if (!strcmp("type", configKey)) {
        Places[placesCount].type = atoi(configVal);
      } else if (!strcmp("range", configKey)) {
        Places[placesCount].range = atoi(configVal);
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(configKey);
      free(configVal);
    }
  }
  AllCount[1] = placesCount + 1;
  free(fileBuffer);
  fclose(fileHandle);
}

/// @func: generateDefaultPlace
/// >> generate default place config file
/// @param: {path} the path of the file
void generateDefaultPlace(const char *path) {
  FILE *fileHandle = fopen(path, "ax+");
  if (NULL == fileHandle) {
    perror("create default place config file ");
    exit(EXIT_FAILURE);
  }
  // default all type have a place
  const char *placeNames[] = {"Volcano", "Ocean", "Forest",
                              "Plane",   "Pole",  "Cave"};
  for (size_t i = 0; i < MONS_TYPE_COUNT; ++i) {
    fputs("[Place]\n", fileHandle);
    fprintf(fileHandle, "name = %s\n", placeNames[i]);
    fputs("monstertypes = 1\n", fileHandle);
    fprintf(fileHandle, "monsters = %zu\n", i);
    fprintf(fileHandle, "type = %zu\n", i);
    fputs("range = 31\n", fileHandle);
  }
  fclose(fileHandle);
}
