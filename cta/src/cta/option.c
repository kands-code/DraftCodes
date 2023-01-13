/// @file: cta/option.c

// -- INCLUDE

#include "cta/option.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// -- GLOBAL

/// @glob: ConfigPath
/// @descript: the config path
char *ConfigPath;

/// @glob: DefaultConfigPath
/// @descript: the default config path
char *DefaultConfigPath;

/// @glob: MaxPathLength
/// @descript: the max length of path
static size_t MaxPathLength = 128;

// -- FUNC

/// @func: initConfigPath
/// >> initialize the config path variable
static void initConfigPath() {
  if (NULL == ConfigPath) {
    ConfigPath = calloc(MaxPathLength, sizeof(char));
  } else {
    fputs("the path is already initialized!\n", stderr);
  }

  if (NULL == DefaultConfigPath) {
    DefaultConfigPath = calloc(MaxPathLength, sizeof(char));
    sprintf(DefaultConfigPath, "%s/.config/cta", getenv("HOME"));
  }
}

/// @func: setConfigPath
/// >> set the path of the config directory
/// @param: <path> the directory path [ const char * ]
/// @return: the result of set [ _Bool ]
/// @descript:
///   * if path is `NULL` or not valid,
///     the program will use the default path
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

  strncpy(ConfigPath, configPath, MaxPathLength);
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
extern _Bool loadConfigs() { return 1; }