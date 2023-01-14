/// @file: cta/option.c

// -- INCLUDE

#include "cta/option.h"
#include "cta/game.h"
#include "cta/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// -- GLOBAL

/// @glob: ConfigPath
/// @descript: the config path
char *ConfigPath = NULL;

/// @glob: DefaultConfigPath
/// @descript: the default config path
char *DefaultConfigPath = NULL;

/// @glob: ConfigFileCount
/// @descript: the number of config files
size_t ConfigFileCount = 16;

/// @glob: Buffs
/// @descript: all kinds of buff
Buff Buffs[] = {};

/// @glob: Skills
/// @descript: all kinds of buff
Skill Skills[] = {};

/// @glob: Monsters
/// @descript: all kinds of monsters
Monster Monsters[] = {};

/// @glob: MonsterCount
/// @descript: the number of the kind of monster
size_t MonsterCount = 0;

/// @glob: CharacterPlace
/// @descript: the place the character in
unsigned char CharacterPlace = 0;

/// @glob: PlaceCount
/// @descript: the number of places
unsigned char PlaceCount = 5; // 5 monster types, so use 5 as init

/// @glob: Places
/// @descript: all the places
Place Places[] = {};

/// @glob: Weapons
/// @descript: all the places
Weapon Weapons[] = {};

// -- FUNC

/// @func: setConfigPath
/// >> set the path of the config directory
/// @param: {path} the directory path [ const char * ]
/// @return: the result of set [ _Bool ]
/// @descript:
///   * the program will crash if path is `NULL` or invalid
_Bool setConfigPath(const char *path) {
  if (NULL == ConfigPath) {
    initConfigPath();
  }

  size_t defaultPathLength = strlen(DefaultConfigPath);
  const char *configPath;

  if (NULL == path) {
    fputs("the path is `NULL`!\n", stderr);
    fputs("use the default path!\n", stderr);
    fprintf(stderr, "  %s\n", DefaultConfigPath);
    configPath = DefaultConfigPath;
  } else {
    size_t pathLength = strlen(path);
    if (pathLength > MaxPathLength + 1) {
      fputs("the path is too long!\n", stderr);
      fputs("use the default path!\n", stderr);
      fprintf(stderr, "  %s\n", DefaultConfigPath);
      configPath = DefaultConfigPath;
    } else {
      configPath = path;
    }
  }

  strncpy(ConfigPath, configPath, strlen(configPath));
  ConfigPath[strlen(configPath)] = '\0';
  fprintf(stdout, "use %s as config path!\n", ConfigPath);

  struct stat confStat = {0};
  if (stat(ConfigPath, &confStat) == -1) {
    mkdir(ConfigPath, 0733);
  }

  return 1;
}

/// @func: loadConfigs
/// >> load all configs
/// @return: the result of load [ _Bool ]
/// @descript:
///   * the config file must follow the syntax
///   * the program will crash if the config path is invalid
_Bool loadConfigs() {
  // init place and time
  srand(time(NULL));
  CharacterPlace = rand() % PlaceCount;
  GameTime = rand() % 50;

  size_t itemCount = 0;
  char **items = readDirItem(ConfigPath, &itemCount);

  if (!arrayMember("glob.gcf", sizeof(char), (void **)items, itemCount)) {
    fputs("global config file cannot find!\n", stderr);
    fputs("generate default global config file...\n", stderr);
    generateDefaultGlobal();
    loadConfigs();
  } else {
    char *globConf = calloc(MaxPathLength, sizeof(char));
    sprintf(globConf, "%s/glob.gcf", ConfigPath);
    FILE *globConfHandle = fopen(globConf, "r");
    if (NULL == globConfHandle) {
      perror("open glob config file ");
      exit(EXIT_FAILURE);
    }
    struct stat buffer = {0};
    if (stat(globConf, &buffer) == -1) {
      perror("create buffer ");
      exit(EXIT_FAILURE);
    }
    // read line by line
    char *globConfigFileBuffer = malloc(buffer.st_size);
    while ((fscanf(globConfHandle, "%[^\n] ", globConfigFileBuffer) != EOF)) {
      if ('[' == globConfigFileBuffer[0]) {
        char *configTag = calloc(MaxPathLength, sizeof(char));
        sscanf(globConfigFileBuffer, "[%[^]\n] ", configTag);
        if (strcmp("Global", configTag)) {
          fprintf(stderr, "wrong content in glob config: %s\n", configTag);
          exit(EXIT_FAILURE);
        }
        free(configTag);
      } else {
        char *configKey = calloc(MaxPathLength, sizeof(char));
        char *configVal = calloc(MaxPathLength, sizeof(char));
        char *filePath = calloc(MaxPathLength, sizeof(char));
        sscanf(globConfigFileBuffer, "%s = %[^\n]", configKey, configVal);
        sprintf(filePath, "%s/%s", ConfigPath, configVal);
        if (!strcmp("character", configKey)) {
          loadCharacterConfig(filePath);
        } else if (!strcmp("monster", configKey)) {
        } else if (!strcmp("place", configKey)) {
        } else if (!strcmp("item", configKey)) {
        } else if (!strcmp("skill", configKey)) {
        } else if (!strcmp("shop", configKey)) {
        } else {
          fprintf(stderr, "wrong config key found: %s\n", configKey);
          exit(EXIT_FAILURE);
        }
        free(filePath);
        free(configVal);
        free(configKey);
      }
    }
    fclose(globConfHandle);
    free(globConf);
  }

  return 1;
}

/// @func: saveConfigs
/// >>  save all configs
/// @return: the result of free [ _Bool ]
_Bool saveConfigs() {
  char *globConf = calloc(MaxPathLength, sizeof(char));
  sprintf(globConf, "%s/glob.gcf", ConfigPath);
  FILE *globConfHandle = fopen(globConf, "r");
  if (NULL == globConfHandle) {
    perror("open glob config file ");
    exit(EXIT_FAILURE);
  }
  struct stat buffer = {0};
  if (stat(globConf, &buffer) == -1) {
    perror("create buffer ");
    exit(EXIT_FAILURE);
  }
  char *globConfigFileBuffer = malloc(buffer.st_size);
  while ((fscanf(globConfHandle, "%[^\n] ", globConfigFileBuffer) != EOF)) {
    if ('[' == globConfigFileBuffer[0]) {
      char *configTag = calloc(MaxPathLength, sizeof(char));
      sscanf(globConfigFileBuffer, "[%[^]\n] ", configTag);
      if (strcmp("Global", configTag)) {
        fprintf(stderr, "wrong content in glob config: %s\n", configTag);
        exit(EXIT_FAILURE);
      }
      free(configTag);
    } else {
      char *configKey = calloc(MaxPathLength, sizeof(char));
      char *configVal = calloc(MaxPathLength, sizeof(char));
      char *filePath = calloc(MaxPathLength, sizeof(char));
      sscanf(globConfigFileBuffer, "%s = %[^\n]", configKey, configVal);
      sprintf(filePath, "%s/%s", ConfigPath, configVal);
      if (!strcmp("character", configKey)) {
        saveCharacter(filePath);
      } else if (!strcmp("monster", configKey)) {
      } else if (!strcmp("place", configKey)) {
      } else if (!strcmp("item", configKey)) {
      } else if (!strcmp("skill", configKey)) {
      } else if (!strcmp("shop", configKey)) {
      } else {
        fprintf(stderr, "wrong config key found: %s\n", configKey);
        exit(EXIT_FAILURE);
      }
      free(filePath);
      free(configVal);
      free(configKey);
    }
  }
  fclose(globConfHandle);
  free(globConf);
  return 1;
}