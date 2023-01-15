/// @file: cta/game.c

// -- INCLUDE

#include "cta/game.h"
#include "cta/option.h"
#include "cta/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -- GLOBAL

/// @glob: CharacterState
/// @descript: the state of the character
GameState CharacterState = {0};

/// @glob: GameTime
/// @descript: the game time and places
///   * 0: time
///   * 1: time range
///   * 2: place
size_t GameTP[3] = {};

// -- FUNC

/// @func: normalHelper {PRIVATE}
/// >> print help info on normal mode
static void normalHelper() {
  puts("");
  puts("'s' to get infomation about character state");
  puts("'c' to config character's name");
  puts("'p' to get infomation about place");
  puts("'w' to walk around");
  puts("'r' to take a rest");
  puts("'$' to open shop");
  puts("'b' to open bag");
  puts("'h' or '?' to print this message");
  puts("'q' to leave the game");
  puts("");
}

/// @func: fightHelper {PRIVATE}
/// >> print help info on fight mode
static void fightHelper() {
  puts("");
  puts("'a' to attack the monster");
  puts("'s' to get infomation about character state");
  puts("'p' to get infomation about place");
  puts("'m' to look up the state of monster");
  puts("'d' to drink poinson");
  puts("'h' or '?' to print this message");
  puts("");
}

/// @func: bagHelper {PRIVATE}
/// >> print help info on bag mode
static void bagHelper() {
  puts("");
  puts("'d' to drink poinson");
  puts("'s' to get infomation about character state");
  puts("'c' to change the weapon");
  puts("'b' back to the normal mode");
  puts("'h' or '?' to print this message");
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
    if ('h' == opt || '?' == opt) {
      normalHelper();
    } else if ('s' == opt) {
      gameLookUpState();
    } else if ('c' == opt) {
      gameConfig();
    } else if ('p' == opt) {
      gameLookUpPlace();
    } else if ('w' == opt) {
      gameWalk();
    } else if ('r' == opt) {
      gameTakeRest();
    } else if ('b' == opt) {
      gameLookUpBag();
    } else if ('q' == opt) {
      puts("quit the game");
      return opt;
    } else {
      normalHelper();
    }
  } else if (1 == CharacterState.state) {
    srand(time(NULL));
    Monster m =
        Monsters[Places[GameTP[2]]
                     .monsters[rand() % Places[GameTP[2]].monsterTypes]];
    int mpRelat = typeRelation(m.type, Places[GameTP[2]].type);
    if (-1 == mpRelat) {
      m.hp *= 0.7;
      m.damage *= 0.7;
    } else if (1 == mpRelat) {
      m.hp *= 1.2;
      m.damage *= 1.2;
    }
    while (!(0 == m.hp || 0 == CharacterState.hp)) {
      printf("F> what do you want to do? ");
      opt = getchar();
      while (getchar() != '\n')
        ;
      if ('a' == opt) {
        int damage =
            CharacterState.wp.damage * (1 + CharacterState.buff.damage);
        size_t miss = rand() % 100; // >= 80 will miss the attack
        if (miss < 90) {
          int relat = typeRelation(CharacterState.wp.type, m.type);
          if (-1 == relat) {
            damage *= 0.7;
          } else if (1 == relat) {
            damage *= 1.2;
          }
          if (m.hp > damage) {
            m.hp -= damage;
          } else {
            m.hp = 0;
          }
        } else {
          printf("the monster (%s) missed the attack\n", m.name);
        }
        if (m.hp > 0) {
          miss = rand() % 100;
          if (miss < 90) {
            size_t monsterDamage = m.damage / (1 + CharacterState.buff.hp);
            if (CharacterState.hp > monsterDamage) {
              CharacterState.hp -= monsterDamage;
            } else {
              CharacterState.hp = 0;
              puts("you lost the game!");
              return 'q';
            }
          } else {
            puts("you missed this attack");
          }
        }
        // adjust state
        GameTP[0] += 1;
        if (CharacterState.buff.time > 0) {
          CharacterState.buff.time -= 1;
        }
        if (0 == CharacterState.buff.time) {
          CharacterState.buff.hp = 0;
          CharacterState.buff.damage = 0;
        }
      } else if ('s' == opt) {
        gameLookUpState();
      } else if ('p' == opt) {
        gameLookUpPlace();
      } else if ('m' == opt) {
        gameLookUpMonster(&m);
      } else if ('d' == opt) {
        puts("");
        if (CharacterState.bag.itemNumber[0] > 0) {
          srand(time(NULL));
          size_t heal = CharacterState.hpBound * (rand() % 37 + 31) * 0.01;
          puts("drink poison...");
          if (CharacterState.hp + heal > CharacterState.hpBound) {
            CharacterState.hp = CharacterState.hpBound;
          } else {
            CharacterState.hp += heal;
          }
          // adjust state
          CharacterState.bag.itemNumber[0] -= 1;
          GameTP[0] += 1;
          if (CharacterState.buff.time > 0) {
            CharacterState.buff.time -= 1;
          }
          if (0 == CharacterState.buff.time) {
            CharacterState.buff.hp = 0;
            CharacterState.buff.damage = 0;
          }
        } else {
          puts("do not have poison...");
        }
        puts("");
      } else if ('h' == opt || '?' == opt) {
        fightHelper();
      } else {
        fightHelper();
      }
    }
    CharacterState.coin += m.coin;
    CharacterState.state = 0;
  } else if (2 == CharacterState.state) {
    CharacterState.state = 0;
  } else if (3 == CharacterState.state) {
  } else {
    fprintf(stderr, "invalid state %zu\n", CharacterState.state);
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
void gameWalk() {
  // use time as seed
  srand(time(NULL));
  puts("");
  unsigned char event = rand() % 10;
  if (event < 6) { // nothing
    puts("nothing happened...");
  } else if (9 == event) {
    puts("get a buff!");
    CharacterState.buff.hp = (rand() % 13 + 10) * 0.01f;
    CharacterState.buff.damage = (rand() % 13 + 10) * 0.01f;
    CharacterState.buff.time = rand() % 15 + 1;
    printf("hp: %.3f, damage: %.3f, time: %zu\n", CharacterState.buff.hp,
           CharacterState.buff.damage, CharacterState.buff.time);
  } else if (8 == event) {
    puts("get a poison!");
    CharacterState.bag.itemNumber[0] += 1;
  } else {
    puts("meet a monster!");
    CharacterState.state = 1;
  }
  // adjust state
  GameTP[0] += 1;
  CharacterState.step += 1;
  if (CharacterState.buff.time > 0) {
    CharacterState.buff.time -= 1;
  }
  if (0 == CharacterState.buff.time) {
    CharacterState.buff.hp = 0;
    CharacterState.buff.damage = 0;
  }
  if (CharacterState.step > Places[GameTP[2]].range) {
    size_t next = (GameTP[2] + 1) % AllCount[1];
    printf("\ngo cross the place %s, next place %s\n", Places[GameTP[2]].name,
           Places[next].name);
    GameTP[2] = next;
    CharacterState.step = 0;
  }
  puts("");
}

/// @func: gameTakeRest
/// >> take a rest
void gameTakeRest() {
  srand(time(NULL));
  puts("");
  unsigned char event = rand() % 10;
  if (event < 7) {
    if (CharacterState.hp < CharacterState.hpBound) {
      puts("heal...");
      size_t heal = 5 * (1 + CharacterState.buff.hp);
      if (CharacterState.hp + heal > CharacterState.hpBound) {
        CharacterState.hp = CharacterState.hpBound;
      } else {
        CharacterState.hp += heal;
      }
    } else {
      puts("nothing happened...");
    }
  } else {
    puts("meet a monster!");
    CharacterState.state = 1;
  }
  GameTP[0] += 1;
  if (CharacterState.buff.time > 0) {
    CharacterState.buff.time -= 1;
  }
  if (0 == CharacterState.buff.time) {
    CharacterState.buff.hp = 0;
    CharacterState.buff.damage = 0;
  }
  puts("");
}

/// @func: gameLookUpState
/// >> look up character state
void gameLookUpState() {
  puts("");
  size_t tempLen =
      printf("time: %s    place: %s\n", GameTP[0] < 30 ? "day" : "night",
             Places[GameTP[2]].name);
  for (size_t i = 0; i < tempLen; ++i) {
    printf("-");
  }
  puts("");
  printf("name: %s\n", CharacterState.name);
  printf("hp: %zu, coin: %zu\n", CharacterState.hp, CharacterState.coin);
  printf("road: %.3f place\n",
         CharacterState.step * 1.0 / Places[GameTP[2]].range);
  for (size_t i = 0; i < tempLen; ++i) {
    printf("-");
  }
  puts("");
  puts("buff:");
  printf("  hp: %.3f\n", CharacterState.buff.hp);
  printf("  damage: %.3f\n", CharacterState.buff.damage);
  printf("  time: %zu\n", CharacterState.buff.time);
  for (size_t i = 0; i < tempLen; ++i) {
    printf("-");
  }
  puts("");
  puts("Weapon:");
  printf("  name: %s\n", CharacterState.wp.name);
  printf("  damage: %zu\n", CharacterState.wp.damage);
  printf("  type: %s\n", MonsterType[CharacterState.wp.type]);
  puts("");
}

/// @func: gameLookUpPlace
/// >> look up place state
void gameLookUpPlace() {
  puts("");
  printf("name: %s\n", Places[GameTP[2]].name);
  printf("number of monster types: %zu\n", Places[GameTP[2]].monsterTypes);
  for (size_t i = 0; i < Places[GameTP[2]].monsterTypes; ++i) {
    printf("Monster (%s)\n", Monsters[Places[GameTP[2]].monsters[i]].name);
    printf("  hp: %zu\n", Monsters[Places[GameTP[2]].monsters[i]].hp);
    printf("  damage: %zu\n", Monsters[Places[GameTP[2]].monsters[i]].damage);
    printf("  type: %s\n",
           MonsterType[Monsters[Places[GameTP[2]].monsters[i]].type]);
  }
  printf("place type: %s\n", MonsterType[Places[GameTP[2]].type]);
  printf("place range: %zu\n", Places[GameTP[2]].range);
  puts("");
}

/// @func: gameLookUpMonster
/// >> look up the state of the monster
/// @param: {m} the monster [ Monster * ]
void gameLookUpMonster(Monster *m) {
  puts("");
  printf("Monster\n");
  printf("  name: %s\n", m->name);
  printf("  hp: %zu, damage: %zu\n", m->hp, m->damage);
  printf("  type: %s, coin: %zu\n", MonsterType[m->type], m->coin);
  puts("");
}

/// @func: gameLookUpBag
/// >> look up bag
void gameLookUpBag() {
  puts("");
  size_t tempLen = printf("poison: %zu\n", CharacterState.bag.itemNumber[0]);
  for (size_t i = 0; i < tempLen; ++i) {
    putchar('-');
  }
  puts("");
  for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
    printf("%s: %zu\n", Weapons[CharacterState.bag.items[i]].name,
           CharacterState.bag.itemNumber[i + 1]);
  }
  CharacterState.state = 2;
  puts("");
}

/// @func: gameConfig
/// >> config character name
void gameConfig() {
  puts("");
  char *tempName = calloc(MAX_STR_LENGTH, sizeof(char));
  printf("name [%s]: ", CharacterState.name);
  scanf("%s", tempName);
  while (getchar() != '\n')
    ;
  while (strlen(tempName) < 3) {
    puts("");
    fprintf(stderr, "name: `%s` is to short!\n", tempName);
    printf("re-type a new name [%s]: ", CharacterState.name);
    scanf("%s", tempName);
    while (getchar() != '\n')
      ;
    puts("");
  }
  printf("use %s as character name...\n", tempName);
  strncpy(CharacterState.name, tempName, strlen(tempName));
  puts("");
}
