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
  parseArgs(argc, argv);

  mainGame();

  endGame();

  puts("Bye!");

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
}

/// @func: endGame
/// >> some clean process to end the game
static void endGame() {}