# CTA

The Textual Advantual Game written by C.

## Basic Element

### Character

The most important element of this game is the main character,
someone may call him/her the hero/heroine, whatever.

All state of a character list after

```c
typedef struct GameState {
  char name[MAX_STR_LENGTH];
  size_t hp;
  size_t hpBound;
  Weapon wp;
  Bag bag;
  Buff buff;
  size_t coin;
  size_t step;
  size_t state;
} GameState;
```

- the name will input by user at first time, then will load from config
- the weapon by default is `punch (0)`, can be modified in config
- the buff will not work when the character not in the fight
- while the buff do not work, the time of the buff will still go on
- the state can be 0, 1, 2 and 3
  - 0: the normal mode
  - 1: the fight mode
  - 2: the bag mode
  - 3: the shop mode

### Monsters

#### Types

monster have 6 types

- fire
- grass
- water
- common
- light
- dark

the relation of these types is

```mermaid
flowchart TD
Fire -->|strong| Grass
Grass -->|weak| Fire
Grass -->|strong| Water
Water -->|weak| Grass
Water -->|strong| Fire
Fire -->|weak| Water

Fire -->|weak| Light
Grass -->|week| Light
Water -->|week| Light
Dark -->|strong| Light

Common
```

### Place

Every time you enter this game,
your time and place will be setted randomly.

```c
srand(time(NULL));                // srand
GameTP[0] = rand() % 50;          // time
GameTP[2] = rand() % AllCount[1]; // place
```

#### Time

A day, include day and night, can be counted as 50 prs.

That means you do 50 processes, a day passed.

30 prs is day, and 20 prs is night.

#### Envrionment

<mark>**_TODO_**</mark>

### Shop

#### Poison

#### Weapon
