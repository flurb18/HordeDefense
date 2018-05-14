#include "game.h"

#include <SDL2/SDL_events.h>
#include <iostream>

#include "context.h"
#include "display.h"
#include "region.h"
#include "spawner.h"
#include "team.h"

Game::Game(Context* c, Display* d, const int& s, const int& r):\
           regionSize(s), regionsPerSide(r), disp(d), gameContext(c) {
  /*gameContext = c;
  disp = d;
  regionSize = REG_SIZE;
  regionsPerSide = REG_PER_SIDE;*/
  // Regions is a vector
  regions.reserve(regionsPerSide * regionsPerSide);
  /* Regions are created in the x direction; it fills the top row with regions
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  for (unsigned int i = 0; i < regionsPerSide; i++) {
    for (unsigned int j = 0; j < regionsPerSide; j++) {
      regions.push_back(new Region(disp, j*regionSize, i*regionSize, regionSize));
    }
  }
  // do something with this
  Region* centerRegion = regions[winCoordsToIndex(disp->getRadius(), disp->getRadius())];
  spawn = new Spawner(gameContext, centerRegion, &WHITE_TEAM, regionSize / 4);
}

unsigned int Game::regCoordsToIndex(int regX, int regY) {
  unsigned int regIndex = regY * regionsPerSide + regX;
  if (regIndex >= regions.size()) {
    std::cerr << "Invalid argument to regionIndexFromRegionCoords()" << std::endl;
    std::cerr << regIndex << std::endl;
    throw 1;
  }
  return regIndex;
}

unsigned int Game::winCoordsToIndex(int x, int y) {
  return regCoordsToIndex(x / regionSize, y / regionSize);
}

void Game::mouseMoved(int x, int y) {
  if (gameContext->type == GAME_CONTEXT_ZOOMED_OUT) {
    gameContext->setOutlinedRegionIndex(winCoordsToIndex(x, y));
  }
}

void Game::draw() {
  disp->fillBlack();
  if (gameContext->type == GAME_CONTEXT_ZOOMED_OUT) {
    for (unsigned int i = 0; i < regions.size(); i++) {
      regions[i]->drawAgents();
      if (i == gameContext->getOutlinedRegionIndex()) {
        disp->setDrawColor(255, 255, 255);
        regions[i]->drawOutline();
      }
    }
  }
  if (gameContext->isPaused()) {
    disp->drawText("PAUSED", 0, 0);
  }
}

void Game::updateRegions() {
  for (unsigned int i = 0; i < regions.size(); i++) {
    regions[i]->update();
  }
}

void Game::mainLoop() {
  SDL_Event e;
  while (gameContext->type != GAME_CONTEXT_EXIT) {
    while (SDL_PollEvent(&e) != 0) {
      /* SDL_QUIT is outside switch statement, so we can break the event
      polling loop*/
      if (e.type == SDL_QUIT) {
        gameContext->type = GAME_CONTEXT_EXIT;
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
              gameContext->togglePause();
          }
      }
    }
    if (!gameContext->isPaused()) {
      updateRegions();
    }
    draw();
    disp->update();
    disp->wait(10);
  }
}
