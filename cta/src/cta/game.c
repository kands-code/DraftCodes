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

/// @glob: GameTP
/// @descript: the game time and places
///   * 0: time
///   * 1: time range
///   * 2: place
size_t GameTP[3] = {};

/// @glob: ShopItems
/// @descript: the items in the shop
Product ShopItems[MONS_TYPE_COUNT] = {};

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
  puts("'r' to run away");
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
  puts("'l' to list bag items");
  puts("'c' to change the weapon");
  puts("'b' back to the normal mode");
  puts("'h' or '?' to print this message");
  puts("");
}

/// @func: shopHelper {PRIVATE}
/// >> print help info on shop mode
static void shopHelper() {
  puts("");
  puts("'l' to list all items");
  puts("'b' to buy items");
  puts("'s' to sold weapon");
  puts("'i' to check item info");
  puts("'n' back to the normal mode");
  puts("'h' or '?' to print this message");
  puts("");
}

/// @func: drinkPoison
/// >> drink posion
static void drinkPoison() {
  puts("");
  if (CharacterState.bag.poisonNumber > 0) {
    srand(time(NULL));
    size_t heal = CharacterState.hpBound * (rand() % 37 + 31) * 0.01;
    puts("drink poison...");
    if (CharacterState.hp + heal > CharacterState.hpBound) {
      CharacterState.hp = CharacterState.hpBound;
    } else {
      printf("? %zu\n", CharacterState.state);
      CharacterState.hp += heal;
    }
    // adjust state
    CharacterState.bag.poisonNumber -= 1;
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
}

/// @func: gameChoice
/// >> give different choice in different place
/// @return: the quit key [ char ]
char gameChoice() {
  char opt;
  if (0 == CharacterState.state) {
    printf("N> what do you want to do? ");
    opt = getchar();
    while (getchar() != '\n')
      ;
    if ('s' == opt) {
      gameLookUpState();
    } else if ('c' == opt) {
      gameConfig();
    } else if ('p' == opt) {
      gameLookUpPlace();
    } else if ('w' == opt) {
      gameWalk();
    } else if ('r' == opt) {
      gameTakeRest();
    } else if ('$' == opt) {
      gameShop();
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
        int damage = Weapons[CharacterState.wp].damage *
                     (1 + CharacterState.buff.damage);
        size_t miss = rand() % 100; // >= 80 will miss the attack
        if (miss < 90) {
          int relat = typeRelation(Weapons[CharacterState.wp].type, m.type);
          if (-1 == relat) {
            damage *= 0.7;
          } else if (1 == relat) {
            damage *= 1.2;
          }
          if (m.hp > damage) {
            m.hp -= damage;
            puts("");
            printf("monster (%s) hp: %zu\n", m.name, m.hp);
            puts("");
          } else {
            m.hp = 0;
          }
        } else {
          puts("");
          printf("the monster (%s) missed the attack\n", m.name);
          puts("");
        }
        if (m.hp > 0) {
          miss = rand() % 100;
          if (miss < 90) {
            size_t monsterDamage = m.damage / (1 + CharacterState.buff.hp);
            if (CharacterState.hp > monsterDamage) {
              CharacterState.hp -= monsterDamage;
              puts("");
              printf("you hp: %zu\n", CharacterState.hp);
              puts("");
            } else {
              CharacterState.hp = 0;
              bputs("you lost the game!");
              return 'q';
            }
          } else {
            bputs("you missed this attack");
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
      } else if ('r' == opt) {
        srand(time(NULL));
        size_t succ = rand() % 100;
        if (succ < 70) {
          bputs("you ran!");
          CharacterState.state = 0;
          break;
        } else {
          bputs("you're catched!");
          size_t monsterDamage = m.damage / (1 + CharacterState.buff.hp);
          if (CharacterState.hp > monsterDamage) {
            CharacterState.hp -= monsterDamage;
            puts("");
            printf("you hp: %zu\n", CharacterState.hp);
            puts("");
          } else {
            CharacterState.hp = 0;
            bputs("you lost the game!");
            return 'q';
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
      } else if ('d' == opt) {
        drinkPoison();
      } else {
        fightHelper();
      }
    }
    if (0 == m.hp) {
      bputs("you win");
      CharacterState.coin += m.coin;
      CharacterState.state = 0;
    }
  } else if (2 == CharacterState.state) {
    printf("B> what do you want to do? ");
    opt = getchar();
    while (getchar() != '\n')
      ;
    if ('d' == opt) {
      drinkPoison();
    } else if ('s' == opt) {
      gameLookUpState();
    } else if ('l' == opt) {
      gameLookUpBag();
    } else if ('c' == opt) {
      puts("");
      size_t wp = 0;
      gameLookUpBag();
      puts("B > use label");
      printf("B> which weapon do you want to use? ");
      scanf("%zu", &wp);
      printf("you choosed %s\n", Weapons[wp].name);
      while (getchar() != '\n')
        ;
      _Bool inSide = 0;
      for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
        if (CharacterState.bag.items[i] == wp) {
          inSide = 1;
          break;
        }
      }
      if (inSide) {
        if (wp == CharacterState.wp) {
          bputs("you already use this weapon!");
        } else {
          puts("");
          printf("now, %s is your weapon\n", Weapons[wp].name);
          CharacterState.wp = wp;
          puts("");
        }
      } else {
        bputs("invalid label!");
      }
    } else if ('b' == opt) {
      CharacterState.state = 0;
      puts("");
    } else {
      bagHelper();
    }
  } else if (3 == CharacterState.state) {
    srand(time(NULL));
    printf("$> what do you want to do? ");
    size_t hpPrice = rand() % 50 + 5;
    opt = getchar();
    while (getchar() != '\n')
      ;
    if ('l' == opt) {
      puts("");
      puts("item: random (0) -- price 20");
      printf("item: hpBounds + 5 (1) -- price: %zu\n", hpPrice);
      for (size_t i = 0; i < MONS_TYPE_COUNT; ++i) {
        printf("item: %s (%zu) -- price: %zu\n", Weapons[ShopItems[i].wp].name,
               i + 2, ShopItems[i].coin);
      }
      puts("");
    } else if ('b' == opt) {
      size_t pId = 0;
      puts("");
      puts("$ > use product id");
      printf("$> which product do you want to buy? ");
      scanf("%zu", &pId);
      while (getchar() != '\n')
        ;
      puts("");
      if (pId > MONS_TYPE_COUNT + 1) {
        puts("invalid product id!");
      } else {
        if (0 == pId) {
          if (CharacterState.coin < 20) {
            puts("you don't have enough coin, failed!");
          } else {
            size_t randP = rand() % (MONS_TYPE_COUNT + 1);
            if (0 == randP) {
              puts("you will by the hpBound!");
              puts("you hp bound up!");
              CharacterState.hpBound += 5;
              if (CharacterState.hp + 5 > CharacterState.hpBound) {
                CharacterState.hp = CharacterState.hpBound;
              } else {
                CharacterState.hp += 5;
              }
            } else {
              Product tProduct = ShopItems[randP - 1];
              printf("you get %s\n", Weapons[tProduct.wp].name);
              _Bool isHad = 0;
              for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
                if (CharacterState.bag.items[i] == tProduct.wp) {
                  isHad = 1;
                  break;
                }
              }
              if (isHad) {
                puts("you already have this weapon!");
                goto ENDSHOP;
              } else {
                CharacterState.bag.items[CharacterState.bag.itemCount++] =
                    tProduct.wp;
              }
            }
            CharacterState.coin -= 20;
          }
        } else if (1 == pId) {
          puts("you choosed hp bound!");
          if (hpPrice > CharacterState.coin) {
            puts("you don't have enough coin, failed!");
          } else {
            puts("you hp bound up!");
            CharacterState.hpBound += 5;
            if (CharacterState.hp + 5 > CharacterState.hpBound) {
              CharacterState.hp = CharacterState.hpBound;
            } else {
              CharacterState.hp += 5;
            }
            CharacterState.coin -= hpPrice;
          }
        } else {
          printf("you choosed %s\n", Weapons[ShopItems[pId - 2].wp].name);
          if (ShopItems[pId - 2].coin > CharacterState.coin) {
            puts("you don't have enough coin, failed!");
          } else {
            _Bool isHad = 0;
            for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
              if (CharacterState.bag.items[i] == ShopItems[pId - 2].wp) {
                isHad = 1;
                break;
              }
            }
            if (isHad) {
              puts("you already have this weapon!");
              goto ENDSHOP;
            } else {
              printf("you get %s\n", Weapons[ShopItems[pId - 2].wp].name);
              CharacterState.bag.items[CharacterState.bag.itemCount++] =
                  ShopItems[pId - 2].wp;
            }
          }
          CharacterState.coin -= ShopItems[pId - 2].coin;
        }
      }
    ENDSHOP:
      puts("");
    } else if ('s' == opt) {
      puts("");
      srand(time(NULL));
      // init item price
      size_t *priceL = calloc(CharacterState.bag.itemCount, sizeof(size_t));
      for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
        priceL[i] = rand() % 30 + 3;
      }
      for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
        printf("%s (%zu): %zu\n", Weapons[CharacterState.bag.items[i]].name, i,
               priceL[i]);
      }
      puts("");
      size_t wId = 0;
      puts("$> use item id");
      printf("$> which weapon do you want to sold? ");
      scanf("%zu", &wId);
      while (getchar() != '\n')
        ;
      if (wId > CharacterState.bag.itemCount - 1) {
        puts("invalid item id!");
      } else {
        printf("you sold %s\n", Weapons[CharacterState.bag.items[wId]].name);
        // move
        CharacterState.bag.itemCount -= 1;
        for (size_t i = wId; i < CharacterState.bag.itemCount; ++i) {
          CharacterState.bag.items[i] = CharacterState.bag.items[i + 1];
        }
        CharacterState.coin += priceL[wId];
      }
      free(priceL);
      puts("");
    } else if ('i' == opt) {
      size_t pId = 0;
      puts("");
      puts("$ > use product id (> 1)");
      printf("$> which product do you want to see? ");
      scanf("%zu", &pId);
      while (getchar() != '\n')
        ;
      puts("");
      if (pId < 2) {
        puts("invalid product id!");
      } else {
        puts("Weapon:");
        printf("  name: %s\n", Weapons[ShopItems[pId - 2].wp].name);
        printf("  damage: %zu\n", Weapons[ShopItems[pId - 2].wp].damage);
        printf("  type: %s\n",
               MonsterType[Weapons[ShopItems[pId - 2].wp].type]);
        printf("price: %zu\n", ShopItems[pId - 2].coin);
      }
      puts("");
    } else if ('n' == opt) {
      CharacterState.state = 0;
      puts("");
    } else {
      shopHelper();
    }
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
    CharacterState.bag.poisonNumber += 1;
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
  printf("  name: %s\n", Weapons[CharacterState.wp].name);
  printf("  damage: %zu\n", Weapons[CharacterState.wp].damage);
  printf("  type: %s\n", MonsterType[Weapons[CharacterState.wp].type]);
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
  size_t tempLen = printf("poison: %zu\n", CharacterState.bag.poisonNumber);
  for (size_t i = 0; i < tempLen; ++i) {
    putchar('-');
  }
  puts("");
  for (size_t i = 0; i < CharacterState.bag.itemCount; ++i) {
    printf("%s (%zu) ", Weapons[CharacterState.bag.items[i]].name,
           CharacterState.bag.items[i]);
    if (CharacterState.wp == CharacterState.bag.items[i]) {
      printf("*\n");
    } else {
      printf("\b\n");
    }
  }
  CharacterState.state = 2;
  puts("");
}

/// @func: gameShop
/// >> buy some weapon or hpBounds
void gameShop() {
  bputs("welcome to the game store!");
  srand(time(NULL));
  // init store
  for (size_t i = 0; i < MONS_TYPE_COUNT; ++i) {
    size_t wp = rand() % AllCount[2];
    size_t coin = rand() % 50 + Weapons[wp].damage / 2;
    ShopItems[i].coin = coin;
    ShopItems[i].wp = wp;
  }
  CharacterState.state = 3;
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
