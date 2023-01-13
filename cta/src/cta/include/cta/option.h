/// @file: cta/cta.h
#pragma once
#ifndef __CTA_OPTION_H__
#define __CTA_OPTION_H__ 1

// -- GLOBAL

/// @glob: ConfigPath
/// @descript: the config path
extern char *ConfigPath;

/// @glob: DefaultConfigPath
/// @descript: the default config path
extern char *DefaultConfigPath;

// -- FUNC

/// @func: setConfigPath
/// >> set the path of the config directory
/// @param: <path> the directory path [ const char * ]
/// @return: the result of set [ _Bool ]
/// @descript:
///   * if path is `NULL` or not valid,
///     the program will crash
extern _Bool setConfigPath(const char *path);

/// @func: loadConfigs
/// >> load all configs
/// @return: the result of load [ _Bool ]
/// @descript:
///   * the config file must follow the syntax
extern _Bool loadConfigs();

#endif