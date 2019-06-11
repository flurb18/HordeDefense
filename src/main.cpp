#include <cstdlib>
#include <ctime>

#include "display.h"
#include "game.h"


int main(int argc, char* argv[]) {

  /* Set the random number generator seed */
  srand(time(0));

  const int WIN_SIZE = 512;

  Display d = Display(WIN_SIZE);
  Game g = Game(&d);
  g.mainLoop();
  d.end();
}
