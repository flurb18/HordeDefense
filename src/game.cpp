#include "game.h"

#include <SDL2/SDL_events.h>
#include <iostream>
#include <string>

#include "display.h"
#include "region.h"
#include "spawner.h"

/* Constructor sets up regions, creates a friendly spawner */
Game::Game(Display* d, const int& s, const int& r):\
           rSize(s), rPerSide(r), disp(d) {
  context = GAME_CONTEXT_ZOOMED_OUT;
  currentRegionIndex = 0;
  currentUnitIndex = 0;
  t = 0;
  paused = true;
  // Regions is a vector
  regions.reserve(rPerSide * rPerSide);
  /* Regions are created in the x direction; it fills the top row with regions
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  for (unsigned int i = 0; i < rPerSide; i++) {
    for (unsigned int j = 0; j < rPerSide; j++) {
      regions.push_back(new Region(this, j*rSize, i*rSize, rSize, i * rPerSide + j));
    }
  }
  // do something with this
  Region* centerRegion = regions[coordsToSqIndex(rPerSide/2, rPerSide/2, rPerSide)];
  spawn = new Spawner(this, centerRegion, &WHITE_TEAM, rSize / 8, 3);
}


/* The following three methods are helper methods to compute the array index of
   a certain spot in an array, or vice versa*/
unsigned int Game::coordsToSqIndex(int x, int y, int sqPerSide) {
  return y * sqPerSide + x;
}

unsigned int Game::dispCoordsToSqIndex(int x, int y, int sqPerSide) {
  int sqSize = disp->getSize() / sqPerSide;
  return coordsToSqIndex(x / sqSize, y / sqSize, sqPerSide);
}

void Game::indexToSqCoords(int index, int sqPerSide, int *x, int *y) {
  *x = index % sqPerSide;
  *y = index / sqPerSide;
}

/* Handle a mouse moved to (x,y) */
void Game::mouseMoved(int x, int y) {
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      currentRegionIndex = dispCoordsToSqIndex(x, y, rPerSide);
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      currentUnitIndex = dispCoordsToSqIndex(x, y, rSize);
      break;
  }
}

/* Handle a left mouse click at (x,y) */
void Game::leftMouseClicked(int x, int y) {
  if (context == GAME_CONTEXT_ZOOMED_OUT) {
    context = GAME_CONTEXT_ZOOMED_IN;
  }
}

/* Handle a right mouse click at (x,y) */
void Game::rightMouseClicked(int x, int y) {
  if (context == GAME_CONTEXT_ZOOMED_IN) {
    context = GAME_CONTEXT_ZOOMED_OUT;
  }
}

/* Draw the current game screen based on context */
void Game::draw() {
  disp->fillBlack();
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      for (unsigned int i = 0; i < regions.size(); i++) {
        regions[i]->drawUnits();
        if (i == currentRegionIndex) {
          disp->setDrawColorWhite();
          regions[i]->drawOutline();
        }
      }
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      regions[currentRegionIndex]->drawUnitsZoomedIn();
      break;
  }
  if (paused) {
    disp->drawText("PAUSED", 0, 0);
  }
  // Display coordinates of current region selection
  int x, y;
  indexToSqCoords(currentRegionIndex, rPerSide, &x, &y);
  const char *s = (std::to_string(x) + ", " + std::to_string(y)).c_str();
  int w;
  disp->sizeText(s, &w, nullptr);
  disp->drawText(s, disp->getSize() - w, 0);
}

/* Advance each region in game time, which means updating the units in each
   region; if the region contains a spawner it will check if should spawn
   an agent, and attempt to */
void Game::updateRegions() {
  for (unsigned int i = 0; i < regions.size(); i++) {
    regions[i]->update();
  }
}

/* Main loop of the game; largely just handles events, calls draw() and updateRegions
   the display */
void Game::mainLoop() {
  SDL_Event e;
  int x, y;
  while (context != GAME_CONTEXT_EXIT) {
    // Main SDL event loop
    while (SDL_PollEvent(&e) != 0) {
      /* SDL_QUIT is outside switch statement, so we can break the event
      polling loop*/
      if (e.type == SDL_QUIT) {
        context = GAME_CONTEXT_EXIT;
        break;
      }
      /* Handle other events case by case */
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
          mouseMoved(x, y);
          break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_SPACE:
              paused = !paused;
              break;
          }
          break;
      }
    }
    if (!paused) {
      updateRegions();
      t++;
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
  delete spawn;
}
