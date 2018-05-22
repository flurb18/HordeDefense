#include "display.h"
#include "game.h"


int main(int argc, char* argv[]) {

  const int WIN_SIZE = 640;

  Display d = Display(WIN_SIZE);
  Game g = Game(&d);
  g.mainLoop();
  d.end();
}
