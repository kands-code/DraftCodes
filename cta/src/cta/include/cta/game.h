/// @file: cta/game.h
#pragma once
#include "cta/utils.h"
#ifndef __CTA_GAME_H__
#define __CTA_GAME_H__ 1

// -- INClUDE

#include "cta/option.h"
#include <stddef.h>

// -- YTPE

/// @type: GameState
/// > character state
/// @content: {name} the name of the character [ char[] ]
/// @content: {hp} the health point of character [ size_t ]
/// @content: {hpBound} the max health point [ size_t ]
/// @content: {wp} the weapon the character use [ size_t ]
/// @content: {bag} the bag of the character [ Bag ]
/// @content: {buff} the buff the character have [ Buff ]
/// @content: {coin} the coin the character have [ size_t ]
/// @content: {step} the step of character in the place [ size_t ]
/// @content: {state} the state of the character [ size_t ]
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
  char name[MAX_STR_LENGTH];
  size_t hp;
  size_t hpBound;
  size_t wp;
  Bag bag;
  Buff buff;
  size_t coin;
  size_t step;
  size_t state;
} GameState;

typedef struct Product {
  size_t wp;
  size_t coin;
} Product;

// -- GLOBAL

/// @glob: CharacterState
/// @descript: the state of the character
extern GameState CharacterState;

/// @glob: GameTP
/// @descript: the game time and places
///   * 0: time
///   * 1: time range
///   * 2: place
extern size_t GameTP[3];

/// @glob: ShopItems
/// @descript: the items in the shop
extern Product ShopItems[6];

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
extern void gameWalk();

/// @func: gameTakeRest
/// >> take a rest
extern void gameTakeRest();

/// @func: gameLookUpState
/// >> look up character state
extern void gameLookUpState();

/// @func: gameLookUpState
/// >> look up place state
extern void gameLookUpPlace();

/// @func: gameLookUpMonster
/// >> look up the state of the monster
/// @param: {m} the monster [ Monster * ]
extern void gameLookUpMonster(Monster *m);

/// @func: gameLookUpBag
/// >> look up bag
extern void gameLookUpBag();

/// @func: gameShop
/// >> buy some weapon or hpBounds
extern void gameShop();

/// @func: gameConfig
/// >> config character
extern void gameConfig();

#endif