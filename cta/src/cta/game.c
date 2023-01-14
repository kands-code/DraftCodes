/// @file: cta/game.c

// -- INCLUDE

#include "cta/game.h"
#include "cta/option.h"
#include "cta/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -- GLOBAL

/// @glob: CharacterState
/// @descript: the state of the character
GameState CharacterState = {NULL, 0, 0, NULL, 0, 0, 0, 1, NULL, 0, 0, 0};

/// @glob: GameTime
/// @descript: the game time [0, GameTimeRange]
unsigned char GameTime = 0;

/// @glob: GameTimeRange
/// @descript: the max time
unsigned char GameTimeRange = 50; // default is 50

// -- FUNC

/// @func: normalHelper {PRIVATE}
/// >> print help info on normal mode
static void normalHelper() {
  puts("");
  puts("'s' to get infomation about character state");
  puts("'c' to config character");
  puts("'p' to get infomation about place");
  puts("'w' to walk around");
  puts("'s' to open shop");
  puts("'b' to open bag");
  puts("'h' to print this message");
  puts("'q' to leave the game");
  puts("");
}

/// @func: gameChoice
/// >> give different choice in different place
/// @return: the quit key [ char ]
char gameChoice() {
  char opt;
  if (0 == CharacterState.state) {
    printf("what do you want to do? ");
    opt = getchar();
    while (getchar() != '\n')
      ;
    if ('h' == opt) {
      normalHelper();
    } else if ('s' == opt) {
      gameLookUpState();
    } else if ('c' == opt) {
      gameConfig();
    } else if ('q' == opt) {
      puts("quit the game");
      return opt;
    }
  } else if (1 == CharacterState.state) {
  } else if (2 == CharacterState.state) {
  } else if (3 == CharacterState.state) {
  } else {
    fprintf(stderr, "invalid state %d\n", CharacterState.state);
    exit(EXIT_FAILURE);
  }
  return 0;
}

/// @func: gameWalk
/// >> do some walk
/// @descript:
///   * when character walk, he/she have some probabilities meet monsters
///   * when character walk, he/she also have some probabilities get some buff
///   * the buff character get may be good buff, sometimes may be debuff
///   * the probability can set through config file
void gameWalk() {}

/// @func: gameLookUpState
/// >> look up character state
void gameLookUpState() {
  puts("");
  printf("name: %s\n", CharacterState.name);
  printf("HP: %u, SP: %u\n", CharacterState.hp, CharacterState.sp);
  puts("Weapon:");
  printf("  name: %s\n", CharacterState.wp.name);
  puts("");
}

/// @func: gameConfig
/// >> config character name
void gameConfig() {
  puts("press enter to keep config");
  printf("name [%s]: ", CharacterState.name);
  char *name = calloc(MaxPathLength, sizeof(char));
  scanf("%s", name);
  if ('\n' != name[0]) {
    free(CharacterState.name);
    CharacterState.name = name;
    while (getchar() != '\n')
      ;
  } else {
    free(name);
  }
}
