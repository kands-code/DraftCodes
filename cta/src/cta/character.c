/// @file: cta/character.c

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

/// @func: loadCharacterConfig
/// >> load character config
/// @param: {path} the path of the file
void loadCharacterConfig(const char *path) {
  checkPathIsValid(path);
  FILE *fileHandle = fopen(path, "r");

  if (NULL == fileHandle) {
    fprintf(stderr, "character config file cannot find!\n");
    fprintf(stderr, "generate default character config...\n");
    generateDefaultCharacter(path);

    // reopen file
    fileHandle = fopen(path, "r");
    if (NULL == fileHandle) {
      perror("open character config ");
      exit(EXIT_FAILURE);
    }
  }

  struct stat buffer = {0};
  if (stat(path, &buffer) == -1) {
    perror("create buffer ");
    exit(EXIT_FAILURE);
  }
  // flags
  _Bool onlyOneCharacter = 1;
  // read line by line
  char *fileBuffer = malloc(buffer.st_size);
  while ((fscanf(fileHandle, "%[^\n] ", fileBuffer) != EOF)) {
    if ('[' == fileBuffer[0]) {
      if (!onlyOneCharacter) {
        fprintf(stderr, "duplicate character config!\n");
        exit(EXIT_FAILURE);
      } else {
        char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
        sscanf(fileBuffer, "[%[^]\n] ", configTag);
        if (strcmp("Character", configTag)) {
          fprintf(stderr, "wrong content in glob config: %s\n", configTag);
          exit(EXIT_FAILURE);
        }
        onlyOneCharacter = !onlyOneCharacter;
        free(configTag);
      }
    } else {
      char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
      char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);
      if (!strcmp("name", configKey)) {
        strncpy(CharacterState.name, configVal, strlen(configVal));
      } else if (!strcmp("hp", configKey)) {
        CharacterState.hp = atoi(configVal);
      } else if (!strcmp("hpbound", configKey)) {
        CharacterState.hpBound = atoi(configVal);
      } else if (!strcmp("weapon.name", configKey)) {
        strncpy(CharacterState.wp.name, configVal, strlen(configVal));
      } else if (!strcmp("weapon.damage", configKey)) {
        CharacterState.wp.damage = atoi(configVal);
      } else if (!strcmp("weapon.type", configKey)) {
        CharacterState.wp.type = atoi(configVal);
      } else if (!strcmp("bag.itemcnt", configKey)) {
        CharacterState.bag.itemCount = atoi(configVal);
      } else if (!strcmp("bag.items", configKey)) {
        char *valTemp = configVal;
        size_t cnt = 0;
        char *elem = calloc(4, sizeof(char));
        while (sscanf(valTemp, "%[^\n ] ", elem) != EOF) {
          if (cnt > MAX_ITEM_COUNT - 1) {
            fprintf(stderr, "too many items in bag!\n");
            exit(EXIT_FAILURE);
          }
          CharacterState.bag.items[cnt++] = atoi(elem);
          valTemp = valTemp + strlen(elem) + 1;
        }
        free(elem);
      } else if (!strcmp("bag.itemnumber", configKey)) {
        char *valTemp = configVal;
        size_t cnt = 0; // start from 0
        char *elem = calloc(4, sizeof(char));
        while (sscanf(valTemp, "%[^\n ] ", elem) != EOF) {
          if (cnt > MAX_ITEM_COUNT - 1) {
            fprintf(stderr, "too many items in bag!\n");
            exit(EXIT_FAILURE);
          }
          CharacterState.bag.itemNumber[cnt++] = atoi(elem);
          valTemp = valTemp + strlen(elem) + 1;
        }
        free(elem);
      } else if (!strcmp("buff.hp", configKey)) {
        CharacterState.buff.hp = atof(configVal);
      } else if (!strcmp("buff.damage", configKey)) {
        CharacterState.buff.damage = atof(configVal);
      } else if (!strcmp("buff.time", configKey)) {
        CharacterState.buff.time = atoi(configVal);
      } else if (!strcmp("coin", configKey)) {
        CharacterState.coin = atoi(configVal);
      } else if (!strcmp("state", configKey)) {
        CharacterState.state = atoi(configVal);
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(configKey);
      free(configVal);
    }
  }
  CharacterState.step = 0;
  if (0 == CharacterState.hp) {
    puts("your hp is zero, reborn!\n");
    CharacterState.hp = CharacterState.hpBound;
    CharacterState.state = 0;
  }
  free(fileBuffer);
  fclose(fileHandle);
}

/// @func: generateDefaultCharacter
/// >> generate default character config file
/// @param: {path} the path of the file
void generateDefaultCharacter(const char *path) {
  FILE *fileHandle = fopen(path, "ax+");
  if (NULL == fileHandle) {
    perror("create default character config file ");
    exit(EXIT_FAILURE);
  }
  fputs("[Character]\n", fileHandle);
  fputs("name = Hero\n", fileHandle);
  fputs("hp = 23\n", fileHandle);
  fputs("hpbound = 23\n", fileHandle);
  fputs("weapon.name = punch\n", fileHandle);
  fputs("weapon.damage = 3\n", fileHandle);
  fputs("weapon.type = 3\n", fileHandle);
  fputs("bag.itemcnt = 1\n", fileHandle);
  fputs("bag.items = 0\n", fileHandle);
  fputs("bag.itemnumber = 0 1\n", fileHandle);
  fputs("buff.hp = 0\n", fileHandle);
  fputs("buff.damage = 0\n", fileHandle);
  fputs("buff.time = 0\n", fileHandle);
  fputs("coin = 5\n", fileHandle);
  fputs("state = 0\n", fileHandle);
  fclose(fileHandle);
}

/// @func: saveCharacter
/// >> save character config file
/// @param: {path} the path of the file
void saveCharacter(const char *path) {
  FILE *fileHandle = fopen(path, "w");
  if (NULL == fileHandle) {
    perror("create glob config file ");
    exit(EXIT_FAILURE);
  }
  fputs("[Character]\n", fileHandle);
  fprintf(fileHandle, "name = %s\n", CharacterState.name);
  fprintf(fileHandle, "hp = %zu\n", CharacterState.hp);
  fprintf(fileHandle, "hpbound = %zu\n", CharacterState.hpBound);
  fprintf(fileHandle, "weapon.name = %s\n", CharacterState.wp.name);
  fprintf(fileHandle, "weapon.damage = %zu\n", CharacterState.wp.damage);
  fprintf(fileHandle, "weapon.type = %zu\n", CharacterState.wp.type);
  fprintf(fileHandle, "bag.itemcnt = %zu\n", CharacterState.bag.itemCount);
  fprintf(fileHandle, "bag.items =");
  for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
    fprintf(fileHandle, " %zu", CharacterState.bag.items[i]);
  }
  fprintf(fileHandle, "\n");
  fprintf(fileHandle, "bag.itemnumber =");
  for (size_t i = 0; i < CharacterState.bag.itemCount + 1; ++i) {
    fprintf(fileHandle, " %zu", CharacterState.bag.itemNumber[i]);
  }
  fprintf(fileHandle, "\n");
  fprintf(fileHandle, "buff.hp = %.3f\n", CharacterState.buff.hp);
  fprintf(fileHandle, "buff.damage = %.3f\n", CharacterState.buff.damage);
  fprintf(fileHandle, "buff.hp = %zu\n", CharacterState.buff.time);
  fprintf(fileHandle, "state = %zu\n", CharacterState.state);
  fclose(fileHandle);
}