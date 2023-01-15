/// @file: cta/monster.c
/// @descript: the monsters

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

/// @func: loadMonsterConfig
/// >> load monster config
/// @param: {path} the path of the file
void loadMonsterConfig(const char *path) {
  checkPathIsValid(path);
  FILE *fileHandle = fopen(path, "r");

  if (NULL == fileHandle) {
    fprintf(stderr, "monster config file cannot find!\n");
    fprintf(stderr, "generate default monster config...\n");
    generateDefaultMonster(path);

    // reopen file
    fileHandle = fopen(path, "r");
    if (NULL == fileHandle) {
      perror("open monster config ");
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
  size_t monsterCount = -1;
  while ((fscanf(fileHandle, "%[^\n] ", fileBuffer) != EOF)) {
    if ('[' == fileBuffer[0]) {
      char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "[%[^]\n] ", configTag);
      if (strcmp("Monster", configTag)) {
        fprintf(stderr, "wrong content in glob config: %s\n", configTag);
        exit(EXIT_FAILURE);
      } else if (monsterCount + 1 > MAX_ITEM_COUNT) {
        fputs("too many monsters!", stderr);
        exit(EXIT_FAILURE);
      }
      monsterCount++;
      free(configTag);
    } else {
      char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
      char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
      sscanf(fileBuffer, "%s = %[^\n]", configKey, configVal);

      if (!strcmp("name", configKey)) {
        size_t len = strlen(configVal);
        strncpy(Monsters[monsterCount].name, configVal, len);
        Monsters[monsterCount].name[len] = '\0';
      } else if (!strcmp("hp", configKey)) {
        Monsters[monsterCount].hp = atoi(configVal);
      } else if (!strcmp("damage", configKey)) {
        Monsters[monsterCount].damage = atoi(configVal);
      } else if (!strcmp("coin", configKey)) {
        Monsters[monsterCount].coin = atoi(configVal);
      } else if (!strcmp("type", configKey)) {
        Monsters[monsterCount].type = atoi(configVal);
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(configKey);
      free(configVal);
    }
  }
  AllCount[0] = monsterCount + 1;
  free(fileBuffer);
  fclose(fileHandle);
}

/// @func: generateDefaultMonster
/// >> generate default monster config file
/// @param: {path} the path of the file
void generateDefaultMonster(const char *path) {
  FILE *fileHandle = fopen(path, "ax+");
  if (NULL == fileHandle) {
    perror("create default monster config file ");
    exit(EXIT_FAILURE);
  }
  // default all type have a slime
  char *monsterName = calloc(16, sizeof(char));
  for (size_t i = 0; i < MONS_TYPE_COUNT; ++i) {
    fputs("[Monster]\n", fileHandle);
    sprintf(monsterName, "slime-%s", MonsterType[i]);
    fprintf(fileHandle, "name = %s\n", monsterName);
    fputs("hp = 10\n", fileHandle);    // default 10
    fputs("damage = 2\n", fileHandle); // default 2
    fputs("coin = 2\n", fileHandle);   // default 2
    fprintf(fileHandle, "type = %zu\n", i);
  }
  free(monsterName);
  fclose(fileHandle);
}
