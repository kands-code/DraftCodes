/// @file: cta/utils.c

// -- INCLUDE

#include "cta/utils.h"
#include "cta/option.h"
#include <dirent.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// -- FUNC

/// @func: initConfigPath
/// >> initialize the config path variable
void initConfigPath() {
  sprintf(DefaultConfigPath, "%s/.config/cta", getenv("HOME"));
}

/// @func: commandLineHelper
/// >> print help info to screen
void commandLineHelper() {
  puts("");
  puts("cta-exe");
  puts(" A Textural Advanture Game Written in C");
  puts("options:");
  puts("  -c DIRS");
  puts("     determine the config directory");
  puts("     the DIRS `MUST` be the absolute path");
  puts("");
}

/// @func: checkPathIsValid
/// >> check the path whether is valid or not
/// @param: {dirPath} the path to check [ const char * ]
/// @return: the check result [ _Bool ]
/// @descript:
///   * the fuction will get error if the path is `NULL`
_Bool checkPathIsValid(const char *dirPath) {
  if (NULL == dirPath) {
    return 0;
  }

  regex_t rgx;
  int reti = regcomp(&rgx, "^(~|[\\.]{,2})?(/[.-_a-zA-Z0-9]*)*$", REG_EXTENDED);
  if (reti) {
    fputs("wrong regex expression!\n", stderr);
    exit(EXIT_FAILURE);
  }
  reti = regexec(&rgx, dirPath, 0, NULL, 0);
  if (reti) {
    fputs("invalid path!\n", stderr);
    exit(EXIT_FAILURE);
  }

  return 1;
}

/// @func: readDirItem
/// >> get all item name inside the directory
/// >> exclude the current directory `.` and up directory `..`
/// @param: {dirName} the directory [ const char * ]
/// @param: {itemCount} the number of items [ size_t * ]
/// @return: the items [ char ** ]
/// @descript: if the return less than 0, the open is failed
char **readDirItem(const char *dirName, size_t *itemCount) {
  *itemCount = 0;
  size_t size = ConfigFileCount;
  char **items = calloc(size, sizeof(char *));
  if (checkPathIsValid(dirName)) {
    DIR *dir = opendir(dirName);

    if (NULL == dir) {
      fprintf(stderr, "open dir %s failed!\n", dirName);
      return NULL;
    }
    struct dirent *dirEnt;
    while ((dirEnt = readdir(dir)) != NULL) {
      if (strcmp(dirEnt->d_name, "..") && strcmp(dirEnt->d_name, ".")) {
        if (*itemCount > size - 2) {
          // resize the array
          size *= 2;
          items = reallocarray(items, size, sizeof(char *));
        }
        items[*itemCount] = calloc(strlen(dirEnt->d_name) + 1, sizeof(char));
        strncpy(items[*itemCount], dirEnt->d_name, strlen(dirEnt->d_name));
        items[*itemCount][strlen(dirEnt->d_name)] = '\0';
        *itemCount += 1;
      }
    }
    closedir(dir);
  } else {
    fprintf(stderr, "%s is an invalid dir name!\n", dirName);
    return NULL;
  }
  return items;
}

/// @func: arrayMember
/// >> test is the element in the array
/// @param: {elem} the element [ void * ]
/// @param: {size} the size of the element [ size_t ]
/// @param: {arr} the array [ void ** ]
/// @param: {len} the size of the array [ size_t ]
/// @return: the result of the test [ _Bool ]
_Bool arrayMember(void *elem, size_t size, void **arr, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (!memcmp(elem, arr[i], size)) {
      return 1;
    }
  }
  return 0;
}

/// @func: typeRelation
/// >> test the releation of two types
/// @param: {type1} the first type [ size_t ]
/// @param: {type2} the second type [ size_t ]
/// @return: 0, 1 or -1
/// @descript:
///   * if return 0, the two type have no releation
///   * if return 1, the type1 is strong to type2
///   * if return -1, the type1 is weak to type2
int typeRelation(size_t type1, size_t type2) {
  if (0 == type1 && 1 == type2) {
    return -1;
  } else if (0 == type1 && 2 == type2) {
    return 1;
  } else if (0 == type1 && 4 == type2) {
    return -1;
  } else if (1 == type1 && 0 == type2) {
    return 1;
  } else if (1 == type1 && 2 == type2) {
    return -1;
  } else if (1 == type1 && 4 == type2) {
    return -1;
  } else if (2 == type1 && 0 == type2) {
    return -1;
  } else if (2 == type1 && 1 == type2) {
    return 1;
  } else if (2 == type1 && 4 == type2) {
    return -1;
  } else if (5 == type1 && 4 == type2) {
    return 1;
  } else {
    return 0;
  }
}

/// @func: generateDefaultGlobal
/// >> generate default global config file
void generateDefaultGlobal() {
  char *globConf = calloc(MAX_STR_LENGTH, sizeof(char));
  sprintf(globConf, "%s/glob.gcf", ConfigPath);
  FILE *globConfHandle = fopen(globConf, "ax+");
  if (NULL == globConfHandle) {
    perror("create glob config file ");
    exit(EXIT_FAILURE);
  }
  fputs("[Global]\n", globConfHandle);
  const size_t ConfigKinds = 4;
  const char *AllKindOfConfig[4] = {"character", "monster", "place", "weapon"};
  for (size_t i = 0; i < ConfigKinds; ++i) {
    fprintf(globConfHandle, "%s = %s.gcf\n", AllKindOfConfig[i],
            AllKindOfConfig[i]);
  }
  fclose(globConfHandle);
  free(globConf);
}

/// @func: bputs
/// >> puts sentence inside two blank line
/// @param: {s} the sentence
void bputs(const char *s) {
  puts("");
  puts(s);
  puts("");
}