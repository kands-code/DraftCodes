/// @file: cta/cta.h
#pragma once
#ifndef __CTA_OPTION_H__
#define __CTA_OPTION_H__ 1

// -- INCLUDE

#include "cta/utils.h"
#include <stddef.h>

// -- TYPE

/// @type: Place
/// > the environment
/// @content: {monsterTypes} number of kinds of monsters [ size_t ]
/// @content: {monsters} the monsters in this place [ size_t[] ]
/// @content: {type} the type of the place [ size_t ]
/// @content: {range} the size of the place [ size_t ]
/// @descript:
///   * the `type` is same as the type of monster without
typedef struct Place {
  char name[MAX_STR_LENGTH];
  size_t monsterTypes;
  size_t monsters[MAX_ITEM_COUNT];
  size_t type;
  size_t range;
} Place;

/// @type: Buff
/// > the buff to the character
/// @content: {hp} make character hp to `hp * (1 + buff.hp)`
/// @content: {damage} make character damage to `damage * (1 + buff.damage)`
/// @content: {time} the time the buff continue
typedef struct Buff {
  float hp;
  float damage;
  size_t time;
} Buff;

typedef struct Weapon {
  char name[MAX_STR_LENGTH];
  size_t damage;
  size_t type;
} Weapon;

/// @type: Bag
/// > the character's bag
/// @content: {itemCount} the number of item types [ size_t ]
/// @content: {items} all the items [ size_t[] ]
/// @content: {itemNumber} the number of items [ size_t[] ]
/// @descript:
///   * the first item must be poison
typedef struct Bag {
  size_t itemCount;
  size_t items[MAX_ITEM_COUNT];
  size_t itemNumber[MAX_ITEM_COUNT];
} Bag;

/// @type: Monster
/// > monster
/// @content: {name} the name of the monster [ char[] ]
/// @content: {hp} the health point of the monster [ size_t ]
/// @content: {damage} the damage of the monster [ size_t ]
/// @content: {coin} the value of the monster [ size_t ]
/// @content: {type} the type of the monster [ size_t ]
typedef struct Monster {
  char name[MAX_STR_LENGTH];
  size_t hp;
  size_t damage;
  size_t coin;
  size_t type;
} Monster;

// -- GLOBAL

/// @glob: ConfigPath
/// @descript: the config path
extern char ConfigPath[MAX_STR_LENGTH];

/// @glob: DefaultConfigPath
/// @descript: the default config path
extern char DefaultConfigPath[MAX_STR_LENGTH];

/// @glob: ConfigFileCount
/// @descript: the number of config files
extern const size_t ConfigFileCount;

/// @glob: CharacterConfigPath
/// @descript: the config path of character
extern char CharacterConfigPath[MAX_STR_LENGTH];

/// @glob: AllCount
/// @descript: all count value
///   * 0: monster
///   * 1: place
///   * 2: weapon
extern size_t AllCount[3];

/// @glob: Monsters
/// @descript: all kinds of monsters
extern Monster Monsters[MAX_ITEM_COUNT];

/// @glob: MonsterType
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
extern char *MonsterType[MONS_TYPE_COUNT];

/// @glob: Places
/// @descript: all the places
extern Place Places[MAX_ITEM_COUNT];

/// @glob: Weapons
/// @descript: all the places
extern Weapon Weapons[MAX_ITEM_COUNT];

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

/// @func: saveCharacter
/// >> generate default character config file
/// @param: {path} the path of the file
extern void saveCharacter(const char *path);

#endif