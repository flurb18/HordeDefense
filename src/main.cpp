#include <vector>
#include <iostream>

#include "display.h"
#include "region.h"
#include "game.h"

using std::vector;

int main(int argc, char* argv[]) {
  const int REG_SIZE = 35;
  const int REG_PER_SIDE = 25;
  const int WIN_SIZE = REG_SIZE * REG_PER_SIDE;

  int gameContext = GAME_CONTEXT_ZOOMED_OUT;

  Display d = Display(WIN_SIZE);
  d.fillBlack();
  vector<Region*> regions;
  regions.reserve(REG_PER_SIDE * REG_PER_SIDE);
  /* Regions are created in the x direction; it fills the top row with regions
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  for (int i = 0; i < REG_PER_SIDE; i++) {
    for (int j = 0; j < REG_PER_SIDE; j++) {
      regions.push_back(new Region(j*REG_SIZE, i*REG_SIZE, REG_SIZE, &d));
    }
  }

  while (gameContext != GAME_CONTEXT_EXIT) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      d.fillBlack();
      if (e.type == SDL_QUIT) {
        gameContext = GAME_CONTEXT_EXIT;
      }
      if (e.type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (gameContext == GAME_CONTEXT_ZOOMED_OUT) {
          int regX = x / REG_SIZE;
          int regY = y / REG_SIZE;
          unsigned int regIndex = regY * REG_PER_SIDE + regX;
          if (regIndex < 0 || regIndex >= regions.size()) {
            throw "Mouse is out of bounds or a region got deleted!";
          }
          regions[regIndex]->drawOutline();
        }
      }
      d.update();
      d.wait(10);
    }

  }
  d.end();
}
