/// @file: cta/utils.h
#pragma once
#ifndef __CTA_UTILS_H__
#define __CTA_UTILS_H__ 1

// -- FUNC

/// @func: commandLineHelper
/// >> print help info to screen
extern void commandLineHelper();

/// @func: checkPathIsValid
/// >> check the path whether is valid or not
/// @param: <dirPath> the path to check [ const char * ]
/// @return: the check result [ _Bool ]
/// @descript:
///   * the fuction will get error if the path is `NULL`
extern _Bool checkPathIsValid(const char *dirPath);

#endif