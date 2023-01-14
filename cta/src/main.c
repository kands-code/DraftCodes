#include "cta/game.h"
#include "cta/option.h"
#include "cta/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @func: parseArgs
/// >> parse the args form commandline
/// @param: <argc> the number of arguments [ int ]
/// @param: <argv> all the arguments [ char * argv[] ]
static void parseArgs(int argc, char *argv[]);

/// @func: mainGame
/// >> the main process of the game
static void mainGame();

/// @func: endGame
/// >> some clean process to end the game
static void endGame();

int main(int argc, char *argv[]) {
  // init basic config
  initConfigPath();
  parseArgs(argc, argv);
  // load config and launch main loop
  mainGame();
  // some clear process
  endGame();
  exit(EXIT_SUCCESS);
}

/// @func: parseArgs
/// >> parse the args form commandline
/// @param: <argc> the number of arguments [ int ]
/// @param: <argv> all the arguments [ char * argv[] ]
void parseArgs(int argc, char *argv[]) {
  if (argc > 1) {
    if (!strcmp("-c", argv[1])) {
      if (checkPathIsValid(argv[2])) {
        setConfigPath(argv[2]);
      } else {
      }
    } else {
      fprintf(stderr, "only support `-c` to determine the config directory\n");
      commandLineHelper();
      exit(EXIT_FAILURE);
    }
  } else {
    setConfigPath(DefaultConfigPath);
  }
}

/// @func: mainGame
/// >> the main process of the game
static void mainGame() {
  puts("try load configurations...");
  loadConfigs();
  // main loop
  char opt = '\0';
  while (opt != 'q') {
    opt = gameChoice();
  }
}

/// @func: endGame
/// >> some clean process to end the game
static void endGame() { saveConfigs(); }