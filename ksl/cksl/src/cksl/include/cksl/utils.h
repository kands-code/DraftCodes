/// @file: cksl/utils.h
/// @descript: some util functions
#pragma once
#ifndef __CKSL_UTILS_H__
#define __CKSL_UTILS_H__

// -- INCLUDE

#include <stddef.h>

// -- FUNC

/// @func: flattenSource
/// >> flatten source code to one line string
/// @param: {source} the source code [ const char * ]
/// @param: {len} the size of the source code [ size_t ]
extern char *flattenSource(const char *source, size_t len);

/// @func: ckslpHelper
/// >> print help message
extern void ckslpHelper();

#endif