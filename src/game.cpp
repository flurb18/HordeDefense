#include "game.h"

#include <SDL2/SDL_events.h>

#include "display.h"
#include "region.h"
#include "context.h"

Game::Game(Display* d, const int& REG_SIZE, const int& REG_PER_SIDE) {
  gameContext = Context();
  disp = d;
  regionSize = REG_SIZE;
  regionsPerSide = REG_PER_SIDE;
  regions.reserve(regionsPerSide * regionsPerSide);
  /* Regions are created in the x direction; it fills the top row with regions
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  for (int i = 0; i < regionsPerSide; i++) {
    for (int j = 0; j < regionsPerSide; j++) {
      regions.push_back(new Region(j*regionSize, i*regionSize, regionSize, disp));
    }
  }
}

void Game::mouseMoved(int x, int y) {
  if (gameContext.type == GAME_CONTEXT_ZOOMED_OUT) {
    int regX = x / regionSize;
    int regY = y / regionSize;
    unsigned int regIndex = regY * regionsPerSide + regX;
    if (regIndex < 0 || regIndex >= regions.size()) {
      throw "Mouse is out of bounds or a region got deleted!";
    }
    gameContext.setOutlinedRegionIndex(regIndex);
  }
}

void Game::draw() {
  disp->fillBlack();
  if (gameContext.type == GAME_CONTEXT_ZOOMED_OUT) {
    for (unsigned int i = 0; i < regions.size(); i++) {
      regions[i]->drawAgents();
      if (i == gameContext.getOutlinedRegionIndex()) {
        regions[i]->drawOutline();
      }
    }
  }
  if (gameContext.isPaused()) {
    disp->drawText("PAUSED", 0, 0);
  }
}

void Game::mainLoop() {
  SDL_Event e;
  while (gameContext.type != GAME_CONTEXT_EXIT) {
    while (SDL_PollEvent(&e) != 0) {
      /* SDL_QUIT is outside switch statement, so we can break the event
      polling loop*/
      if (e.type == SDL_QUIT) {
        gameContext.type = GAME_CONTEXT_EXIT;
        break;
      }
      switch(e.type) {
        case SDL_MOUSEMOTION:
          int x, y;
          SDL_GetMouseState(&x, &y);
          mouseMoved(x, y);
          break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_SPACE:
              gameContext.togglePause();
          }
      }
    }
    draw();
    disp->update();
    disp->wait(10);
  }
}
