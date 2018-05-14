#include "game.h"

#include <SDL2/SDL_events.h>
#include <iostream>
#include <string>

#include "context.h"
#include "display.h"
#include "region.h"
#include "spawner.h"
#include "team.h"

Game::Game(Context* c, Display* d, const int& s, const int& r):\
           regionSize(s), regionsPerSide(r), gameContext(c), disp(d){
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
  spawn = new Spawner(gameContext, centerRegion, &WHITE_TEAM, regionSize / 4, 100);
}

unsigned int Game::regCoordsToIndex(int regX, int regY) {
  unsigned int regIndex = regY * regionsPerSide + regX;
  if (regIndex >= regions.size()) {
    std::cerr << "Invalid argument to regCoordsToIndex()" << std::endl;
    std::cerr << regIndex << std::endl;
    throw 1;
  }
  return regIndex;
}

void Game::indexToRegCoords(int index, int *x, int *y) {
  *x = index % regionsPerSide;
  *y = index / regionsPerSide;
}

unsigned int Game::winCoordsToIndex(int x, int y) {
  return regCoordsToIndex(x / regionSize, y / regionSize);
}

void Game::mouseMoved(int x, int y) {
  if (gameContext->type == GAME_CONTEXT_ZOOMED_OUT) {
    gameContext->setCurrentRegionIndex(winCoordsToIndex(x, y));
  }
}

void Game::leftMouseClicked(int x, int y) {
  if (gameContext->type == GAME_CONTEXT_ZOOMED_OUT) {
    gameContext->type = GAME_CONTEXT_ZOOMED_IN;
  }
}

void Game::rightMouseClicked(int x, int y) {
  if (gameContext->type == GAME_CONTEXT_ZOOMED_IN) {
    gameContext->type = GAME_CONTEXT_ZOOMED_OUT;
  }
}

void Game::draw() {
  disp->fillBlack();
  if (gameContext->type == GAME_CONTEXT_ZOOMED_OUT) {
    for (unsigned int i = 0; i < regions.size(); i++) {
      regions[i]->drawAgents();
      if (i == gameContext->getCurrentRegionIndex()) {
        disp->setDrawColor(255, 255, 255);
        regions[i]->drawOutline();
      }
    }
  } else if (gameContext->type == GAME_CONTEXT_ZOOMED_IN) {
    regions[gameContext->getCurrentRegionIndex()]->drawAgentsZoomedIn();
  }
  if (gameContext->isPaused()) {
    disp->drawText("PAUSED", 0, 0);
  }
  int x, y;
  indexToRegCoords(gameContext->getCurrentRegionIndex(), &x, &y);
  const char *s = (std::to_string(x) + ", " + std::to_string(y)).c_str();
  int w, h;
  disp->sizeText(s, &w, &h);
  disp->drawText(s, disp->getSize() - w, 0);
}

void Game::updateRegions() {
  for (unsigned int i = 0; i < regions.size(); i++) {
    regions[i]->update();
  }
}

void Game::mainLoop() {
  SDL_Event e;
  int x, y;
  while (gameContext->type != GAME_CONTEXT_EXIT) {
    // Main SDL event loop
    while (SDL_PollEvent(&e) != 0) {
      /* SDL_QUIT is outside switch statement, so we can break the event
      polling loop*/
      if (e.type == SDL_QUIT) {
        gameContext->type = GAME_CONTEXT_EXIT;
        break;
      }
      switch(e.type) {
        case SDL_MOUSEMOTION:
          SDL_GetMouseState(&x, &y);
          mouseMoved(x, y);
          break;
        case SDL_MOUSEBUTTONDOWN:
          SDL_GetMouseState(&x, &y);
          switch(e.button.button) {
            case SDL_BUTTON_LEFT:
              leftMouseClicked(x, y);
              break;
            case SDL_BUTTON_RIGHT:
              rightMouseClicked(x, y);
              break;
          }
          break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_SPACE:
              gameContext->togglePause();
          }
          break;
      }
    }
    if (!gameContext->isPaused()) {
      updateRegions();
      gameContext->t++;
    }
    draw();
    disp->update();
    disp->wait(10);
  }
}

Game::~Game() {
  for (unsigned int i = 0; i < regions.size(); i++) {
    delete regions[i];
  }
  regions.clear();
}
