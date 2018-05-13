#include <vector>
#include <iostream>

#include "display.h"
#include "region.h"

using std::vector;

int main(int argc, char* argv[]) {
  const int WIN_SIZE = 640;
  const int REG_SIZE = 64;
  const int REG_PER_SIDE = WIN_SIZE / REG_SIZE;


  const int GAME_CONTEXT_ZOOMED_OUT = 2;
  const int GAME_CONTEXT_EXIT = 10;
  int gameContext = GAME_CONTEXT_ZOOMED_OUT;
  Display d = Display(WIN_SIZE);
  d.fillBlack();
  vector<Region> regions;
  regions.reserve(REG_PER_SIDE * REG_PER_SIDE);
  for (int i = 0; i < REG_PER_SIDE; i++) {
    for (int j = 0; j < REG_PER_SIDE; j++) {
      regions.emplace_back(j*REG_SIZE, i*REG_SIZE, REG_SIZE);
    }
  }

  while (gameContext != GAME_CONTEXT_EXIT) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
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
            throw "whoops!";
          }
          regions[regY * REG_PER_SIDE + regX].drawOutline(d);
        }
      }
      d.update();
      d.fillBlack();
      d.wait(10);
    }

  }
  /*for (unsigned int i = 0; i < regions.size(); i++) {
    regions[i].drawOutline(d);
  }*/
  d.end();
}
