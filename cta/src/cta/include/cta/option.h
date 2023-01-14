/// @file: cta/cta.h
#pragma once
#ifndef __CTA_OPTION_H__
#define __CTA_OPTION_H__ 1

// -- INCLUDE

#include <stddef.h>

// -- TYPE

/// @type: Buff
/// > the buff
/// @content: {hp} the percent of change on hp [ float ]
/// @content: {damage} the percent of change on damage [ float ]
/// @content: {time} the time the buff will work
typedef struct Buff {
  float hp;
  float damage;
  unsigned int time;
} Buff;

/// @type: Place
/// > the environment
typedef struct Place {
  char *name;
  unsigned char *monsters;
} Place;

typedef struct Skill {
  char *name;
  unsigned char damage;
  unsigned char sp;
  Buff buff;
} Skill;

typedef struct Weapon {
  char *name;
  unsigned int damage;
  unsigned int skill[2];
} Weapon;

typedef struct Item {
  char *name;

} Item;

typedef struct Bag {
  unsigned int itemCount;
  unsigned int *items;
} Bag;

/// @enum: MonsterType
/// > the type of monster
/// @item: {FIRE} the fire type
/// @item: {WATER} the water type
/// @item: {GRASS} the grass type
/// @item: {COMMON} the common type
/// @item: {LIGHT} the light type
/// @item: {DARK} the dark type
/// @descript:
///   * fire --strong-> grass
///   * water --strong-> fire
///   * grass --strong-> water
///   * dark --strong-> light
///   * fire, water and grass weak to light
///   * common do not weak or strong to any type
enum MonsterType { FIRE, WATER, GRASS, COMMON, LIGHT, DARK };

typedef struct Monster {
  char *name;

} Monster;

// -- GLOBAL

/// @glob: ConfigPath
/// @descript: the config path
extern char *ConfigPath;

/// @glob: DefaultConfigPath
/// @descript: the default config path
extern char *DefaultConfigPath;

/// @glob: ConfigFileCount
/// @descript: the number of config files
extern size_t ConfigFileCount;

/// @glob: Buffs
/// @descript: all kinds of buff
extern Buff Buffs[];

/// @glob: Skills
/// @descript: all kinds of buff
extern Skill Skills[];

/// @glob: Monsters
/// @descript: all kinds of monsters
extern Monster Monsters[];

/// @glob: MonsterCount
/// @descript: the number of the kind of monster
extern size_t MonsterCount;

/// @glob: CharacterPlace
/// @descript: the place the character in
extern unsigned char CharacterPlace;

/// @glob: PlaceCount
/// @descript: the number of places
extern unsigned char PlaceCount;

/// @glob: Places
/// @descript: all the places
extern Place Places[];

/// @glob: Weapons
/// @descript: all the places
extern Weapon Weapons[];

// -- FUNC

/// @func: setConfigPath
/// >> set the path of the config directory
/// @param: {path} the directory path [ const char * ]
/// @return: the result of set [ _Bool ]
/// @descript:
///   * the program will crash if path is `NULL` or invalid
extern _Bool setConfigPath(const char *path);

/// @func: loadConfigs
/// >> load all configs
/// @return: the result of load [ _Bool ]
/// @descript:
///   * the config file must follow the syntax
///   * the program will crash if the config path is invalid
extern _Bool loadConfigs();

/// @func: loadCharacterConfig
/// >> load character config
/// @param: {path} the path of the file
extern void loadCharacterConfig(const char *path);

/// @func: loadCharacterConfig
/// >> load character config
/// @param: {path} the path of the file
extern void loadCharacterConfig(const char *path);

/// @func: loadMonsterConfig
/// >> load monster config
/// @param: {path} the path of the file
extern void loadMonsterConfig(const char *path);

/// @func: loadPlaceConfig
/// >> load place config
/// @param: {path} the path of the file
extern void loadPlaceConfig(const char *path);

/// @func: loadItemConfig
/// >> load item config
/// @param: {path} the path of the file
extern void loadItemConfig(const char *path);

/// @func: loadShopConfig
/// >> load shop config
/// @param: {path} the path of the file
extern void loadShopConfig(const char *path);

/// @func: saveConfigs
/// >> save all configs
/// @return: the result of free [ _Bool ]
extern _Bool saveConfigs();

/// @func: saveCharacter
/// >> generate default character config file
/// @param: {path} the path of the file
extern void saveCharacter(const char *path);

#endif