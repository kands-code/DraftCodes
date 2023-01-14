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
        char *configTag = calloc(MaxPathLength, sizeof(char));
        sscanf(fileBuffer, "[%[^]\n] ", configTag);
        if (strcmp("Character", configTag)) {
          fprintf(stderr, "wrong content in glob config: %s\n", configTag);
          exit(EXIT_FAILURE);
        }
        onlyOneCharacter = !onlyOneCharacter;
        free(configTag);
      }
    } else {
      char *configKey = calloc(MaxPathLength, sizeof(char));
      char *configVal = calloc(MaxPathLength, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);

      if (!strcmp("name", configKey)) {
        size_t len = strlen(configVal);
        CharacterState.name = calloc(len + 1, sizeof(char));
        strncpy(CharacterState.name, configVal, len);
        CharacterState.name[len] = '\0';
      } else if (!strcmp("hp", configKey)) {
        CharacterState.hp = atoi(configVal);
      } else if (!strcmp("sp", configKey)) {
        CharacterState.sp = atoi(configVal);
      } else if (!strcmp("weapon.name", configKey)) {
        size_t len = strlen(configVal);
        CharacterState.wp.name = calloc(len + 1, sizeof(char));
        strncpy(CharacterState.wp.name, configVal, len);
        CharacterState.wp.name[len] = '\0';
      } else if (!strcmp("weapon.damage", configKey)) {
        CharacterState.wp.damage = atoi(configVal);
      } else if (!strcmp("weapon.skill", configKey)) {
        sscanf(configVal, "%d %d", &CharacterState.wp.skill[0],
               &CharacterState.wp.skill[1]);
      } else if (strcmp("bag.itemcnt", configKey)) {
        CharacterState.bag.itemCount = atoi(configVal);
      } else if (strcmp("bag.items", configKey)) {
        size_t cnt = 0;
        size_t size = 16;
        CharacterState.bag.items = calloc(size, sizeof(unsigned char));
        char *elem = calloc(4, sizeof(char));
        while (sscanf(configVal, "%[^\n ] ", elem) != EOF) {
          if (size < cnt - 2) {
            size *= 2;
            CharacterState.bag.items = reallocarray(
                CharacterState.bag.items, size, sizeof(unsigned char));
          }
          CharacterState.bag.items[cnt++] = atoi(elem);
          configVal = configVal + strlen(elem) + 1;
        }
        free(elem);
      } else if (strcmp("buff.hp", configKey)) {
        CharacterState.buff.hp = atof(configVal);
        puts("her");
      } else if (strcmp("buff.damage", configKey)) {
        CharacterState.buff.damage = atof(configVal);
      } else if (strcmp("buff.time", configKey)) {
        CharacterState.buff.time = atoi(configVal);
      } else if (strcmp("state", configKey)) {
        CharacterState.state = atoi(configVal);
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
    }
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
    perror("create glob config file ");
    exit(EXIT_FAILURE);
  }
  fputs("[Character]\n", fileHandle);
  fputs("name = Hero\n", fileHandle);
  fputs("hp = 23\n", fileHandle);
  fputs("sp = 11\n", fileHandle);
  fputs("weapon.name = punch\n", fileHandle);
  fputs("weapon.damage = 3\n", fileHandle);
  fputs("weapon.skill = 0 1\n", fileHandle);
  fputs("bag.itemcnt = 1\n", fileHandle);
  fputs("bag.items = 0\n", fileHandle);
  fputs("buff.hp = 0\n", fileHandle);
  fputs("buff.damage = 0\n", fileHandle);
  fputs("buff.time = 0\n", fileHandle);
  fputs("state = 0\n", fileHandle);
  fclose(fileHandle);
}

/// @func: saveCharacter
/// >> generate default character config file
/// @param: {path} the path of the file
void saveCharacter(const char *path) {
  FILE *fileHandle = fopen(path, "w");
  if (NULL == fileHandle) {
    perror("create glob config file ");
    exit(EXIT_FAILURE);
  }
  fputs("[Character]\n", fileHandle);
  fprintf(fileHandle, "name = %s\n", CharacterState.name);
  fprintf(fileHandle, "hp = %u\n", CharacterState.hp);
  fprintf(fileHandle, "sp = %u\n", CharacterState.sp);
  fprintf(fileHandle, "weapon.name = %s\n", CharacterState.wp.name);
  fprintf(fileHandle, "weapon.damage = %u\n", CharacterState.wp.damage);
  fprintf(fileHandle, "weapon.skill = %u %u\n", CharacterState.wp.skill[0],
          CharacterState.wp.skill[1]);
  fprintf(fileHandle, "bag.itemcnt = %d\n", CharacterState.bag.itemCount);
  fprintf(fileHandle, "bag.items =");
  for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
    fprintf(fileHandle, " %u", CharacterState.bag.items[i]);
  }
  fprintf(fileHandle, "\n");
  fprintf(fileHandle, "buff.hp = %f\n", CharacterState.buff.hp);
  fprintf(fileHandle, "buff.damage = %f\n", CharacterState.buff.damage);
  fprintf(fileHandle, "buff.time = %u\n", CharacterState.buff.time);
  fprintf(fileHandle, "state = %u\n", CharacterState.state);
  fclose(fileHandle);
}