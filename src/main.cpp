
#include "display.h"
#include "context.h"
#include "game.h"

int main(int argc, char* argv[]) {
  const int REG_SIZE = 35;
  const int REG_PER_SIDE = 25;
  const int WIN_SIZE = REG_SIZE * REG_PER_SIDE;

  Display d = Display(WIN_SIZE);
  Context c = Context();
  Game g = Game(&c, &d, REG_SIZE, REG_PER_SIDE);
  g.mainLoop();
  d.end();
}
