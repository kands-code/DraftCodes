/// @file: cta/game.h
#pragma once
#ifndef __CTA_GAME_H__
#define __CTA_GAME_H__ 1

// -- INClUDE

#include "cta/option.h"
#include <stddef.h>

// -- YTPE

/// @type: GameState
/// > character state
/// @content: {name} the name of the character [ char * ]
/// @content: {hp} the health point of character [ unsigned char ]
/// @content: {wp} the weapon the character use [ Weapon * ]
/// @content: {bag} the bag of the character [ Bag * ]
/// @content: {buff} the buff the character have [ Buff * ]
/// @content: {state} the state of the character [ unsigned char ]
/// @descript:
///   * the name will input by user at first time, then will load from config
///   * the weapon by default is `punch (0)`, can be modified in config
///   * the buff will not work when the character not in the fight
///   * while the buff do not work, the time of the buff will still go on
///   * the state can be 0, 1, 2 and 3
///     ** 0: the normal mode
///     ** 1: the fight mode
///     ** 2: the bag mode
///     ** 3: the shop mode
typedef struct GameState {
  char *name;
  unsigned char hp;
  unsigned char sp;
  Weapon wp;
  Bag bag;
  Buff buff;
  unsigned char state;
} GameState;

// -- GLOBAL

/// @glob: CharacterState
/// @descript: the state of the character
extern GameState CharacterState;

/// @glob: GameTime
/// @descript: the game time [0, 49]
extern unsigned char GameTime;

/// @glob: GameTimeRange
/// @descript: the max time
extern unsigned char GameTimeRange;

// -- FUNC

/// @func: gameChoice
/// >> give different choice in different place
/// @return: the quit key [ char ]
extern char gameChoice();

/// @func: gameWalk
/// >> do some walk
/// @descript:
///   * when character walk, he/she have some probabilities meet monsters
///   * when character walk, he/she also have some probabilities get some buff
///   * the buff character get may be good buff, sometimes may be debuff
///   * the probability can set through config file
extern void gameWalk();

extern void gameTakeRest();

extern void gameSkill();

/// @func: gameLookUpState
/// >> look up character state
extern void gameLookUpState();

extern void gameLookUpPlace();

extern void gameLookUpMonster();

extern void gameLookUpBag();

extern void gameShop();

/// @func: gameConfig
/// >> config character
extern void gameConfig();

#endif