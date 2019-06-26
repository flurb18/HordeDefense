#include <cstdlib>
#include <ctime>

#include "display.h"
#include "game.h"


int main(int argc, char* argv[]) {

  /* Set the random number generator seed */
  srand(time(0));

  const int GAME_SIZE = 600;

  Game g = Game(GAME_SIZE);
  g.mainLoop();
}
