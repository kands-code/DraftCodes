/// @file: cta/utils.h
#pragma once
#ifndef __CTA_UTILS_H__
#define __CTA_UTILS_H__ 1

// -- INCLUDE

#include <stddef.h>

// -- GLOBAL

/// @glob: MaxPathLength
/// @descript: the max length of path
extern size_t MaxPathLength;

// -- FUNC

/// @func: initConfigPath
/// >> initialize the config path variable
void initConfigPath();

/// @func: commandLineHelper
/// >> print help info to screen
extern void commandLineHelper();

/// @func: checkPathIsValid
/// >> check the path whether is valid or not
/// @param: {dirPath} the path to check [ const char * ]
/// @return: the check result [ _Bool ]
/// @descript:
///   * the fuction will get error if the path is `NULL`
extern _Bool checkPathIsValid(const char *dirPath);

/// @func: readDirItem
/// >> get all item name inside the directory
/// >> exclude the current directory `.` and up directory `..`
/// @param: {dirName} the directory [ const char * ]
/// @param: {itemCount} the number of items [ size_t * ]
/// @return: the items [ char ** ]
/// @descript: if the return less than 0, the open is failed
extern char **readDirItem(const char *dirName, size_t *itemCount);

/// @func: arrayMember
/// >> test is the element in the array
/// @param: {elem} the element [ void * ]
/// @param: {size} the size of the element [ size_t ]
/// @param: {arr} the array [ void ** ]
/// @param: {len} the size of the array [ size_t ]
/// @return: the result of the test [ _Bool ]
extern _Bool arrayMember(void *elem, size_t size, void **arr, size_t len);

/// @func: generateDefaultGlobal
/// >> generate default global config file
extern void generateDefaultGlobal();

/// @func: generateDefaultCharacter
/// >> generate default character config file
/// @param: {path} the path of the file
extern void generateDefaultCharacter(const char *path);

/// @func: generateDefaultMonster
/// >> generate default monster config file
/// @param: {path} the path of the file
extern void generateDefaultMonster(const char *path);

/// @func: generateDefaultItem
/// >> generate default ite, config file
/// @param: {path} the path of the file
extern void generateDefaultItem(const char *path);

/// @func: generateDefaultPlace
/// >> generate default place config file
/// @param: {path} the path of the file
extern void generateDefaultPlace(const char *path);

/// @func: generateDefaultSkill
/// >> generate default skill config file
/// @param: {path} the path of the file
extern void generateDefaultSkill(const char *path);

/// @func: generateDefaultShop
/// >> generate default shop config file
/// @param: {path} the path of the file
extern void generateDefaultShop(const char *path);

#endif