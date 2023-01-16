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
char ConfigPath[MAX_STR_LENGTH] = {};

/// @glob: DefaultConfigPath
/// @descript: the default config path
char DefaultConfigPath[MAX_STR_LENGTH] = {};

/// @glob: ConfigFileCount
/// @descript: the number of config files
const size_t ConfigFileCount = 6;

/// @glob: CharacterConfigPath
/// @descript: the config path of character
char CharacterConfigPath[MAX_STR_LENGTH] = {};

/// @glob: AllCount
/// @descript: all count value
///   * 0: monster
///   * 1: place
///   * 2: weapon
size_t AllCount[3] = {};

/// @glob: Monsters
/// @descript: all kinds of monsters
Monster Monsters[MAX_ITEM_COUNT] = {};

/// @glob: MonsterType
/// > the type of monster
/// @item: {fire} the fire type
/// @item: {water} the water type
/// @item: {grass} the grass type
/// @item: {common} the common type
/// @item: {light} the light type
/// @item: {dark} the dark type
/// @descript:
///   * fire --strong-> grass
///   * water --strong-> fire
///   * grass --strong-> water
///   * dark --strong-> light
///   * fire, water and grass weak to light
///   * common do not weak or strong to any type
char *MonsterType[MONS_TYPE_COUNT] = {"fire",   "water", "grass",
                                      "common", "light", "dark"};

/// @glob: Places
/// @descript: all the places
Place Places[MAX_ITEM_COUNT] = {};

/// @glob: Weapons
/// @descript: all the places
Weapon Weapons[MAX_ITEM_COUNT] = {};

// -- FUNC

/// @func: setConfigPath
/// >> set the path of the config directory
/// @param: {path} the directory path [ const char * ]
/// @return: the result of set [ _Bool ]
/// @descript:
///   * the program will crash if path is `NULL` or invalid
_Bool setConfigPath(const char *path) {
  initConfigPath();

  size_t defaultPathLength = strlen(DefaultConfigPath);
  const char *configPath;

  if (NULL == path) {
    fputs("the path is `NULL`!\n", stderr);
    fputs("use the default path!\n", stderr);
    fprintf(stderr, "  %s\n", DefaultConfigPath);
    configPath = DefaultConfigPath;
  } else {
    size_t pathLength = strlen(path);
    if (pathLength > MAX_STR_LENGTH) {
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
  size_t itemCount = 0;
  char **items = readDirItem(ConfigPath, &itemCount);

  if (!arrayMember("glob.gcf", sizeof(char), (void **)items, itemCount)) {
    fputs("global config file cannot find!\n", stderr);
    fputs("generate default global config file...\n", stderr);
    generateDefaultGlobal();
    loadConfigs();
  } else {
    char *globConf = calloc(MAX_STR_LENGTH, sizeof(char));
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
        char *configTag = calloc(MAX_STR_LENGTH, sizeof(char));
        sscanf(globConfigFileBuffer, "[%[^]\n] ", configTag);
        if (strcmp("Global", configTag)) {
          fprintf(stderr, "wrong content in glob config: %s\n", configTag);
          exit(EXIT_FAILURE);
        }
        free(configTag);
      } else {
        char *configKey = calloc(MAX_STR_LENGTH, sizeof(char));
        char *configVal = calloc(MAX_STR_LENGTH, sizeof(char));
        char *filePath = calloc(MAX_STR_LENGTH, sizeof(char));
        sscanf(globConfigFileBuffer, "%s = %[^\n]", configKey, configVal);
        sprintf(filePath, "%s/%s", ConfigPath, configVal);
        if (!strcmp("character", configKey)) {
          strncpy(CharacterConfigPath, filePath, strlen(filePath));
          loadCharacterConfig(filePath);
        } else if (!strcmp("monster", configKey)) {
          loadMonsterConfig(filePath);
        } else if (!strcmp("place", configKey)) {
          loadPlaceConfig(filePath);
        } else if (!strcmp("weapon", configKey)) {
          loadItemConfig(filePath);
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
    // init place and time
    srand(time(NULL));                // srand
    GameTP[1] = 50;                   // range
    GameTP[0] = rand() % GameTP[1];   // time
    GameTP[2] = rand() % AllCount[1]; // place
    fclose(globConfHandle);
    free(globConf);
  }

  return 1;
}
