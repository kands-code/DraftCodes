/// @file: cta/utils.h
#pragma once
#ifndef __CTA_UTILS_H__
#define __CTA_UTILS_H__ 1

// -- INCLUDE

#include <stddef.h>

// -- MACRO

/// @macro: MAX_STR_LENGTH
/// @descript: the max length of string
#define MAX_STR_LENGTH 128

/// @macro: MAX_ITEM_COUNT
/// @descript: the max number of items, include monsters
#define MAX_ITEM_COUNT 32

/// @macro: MONS_TYPE_COUNT
/// @descript: the number of the type kind of monster
#define MONS_TYPE_COUNT 6

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

/// @func: typeRelation
/// >> test the releation of two types
/// @param: {type1} the first type [ size_t ]
/// @param: {type2} the second type [ size_t ]
/// @return: 0, 1 or -1
/// @descript:
///   * if return 0, the two type have no releation
///   * if return 1, the type1 is strong to type2
///   * if return -1, the type1 is weak to type2
extern int typeRelation(size_t type1, size_t type2);

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

/// @func: generateDefaultPlace
/// >> generate default place config file
/// @param: {path} the path of the file
extern void generateDefaultPlace(const char *path);

/// @func: generateDefaultItem
/// >> generate default ite, config file
/// @param: {path} the path of the file
extern void generateDefaultItem(const char *path);

/// @func: generateDefaultShop
/// >> generate default shop config file
/// @param: {path} the path of the file
extern void generateDefaultShop(const char *path);

/// @func: bputs
/// >> puts sentence inside two blank line
/// @param: {s} the sentence
extern void bputs(const char *s);

#endif