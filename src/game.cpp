#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <string>

#include "agent.h"
#include "display.h"
#include "spawner.h"


/* Constructor sets up regions, creates a friendly spawner */
Game::Game(Display* d): Square(d->getSize()), \
                        context(GAME_CONTEXT_ZOOMED_OUT), t(0), paused(true), disp(d), outside(this) {
  /* mapunits is a vector */
  mapunits.reserve(size * size);
  /* Units are created in the x direction; it fills the top row with regions
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  outside = MapUnit(this);
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      mapunits.push_back(new MapUnit(this, j, i));
    }
  }
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      int index = coordsToSqIndex(j, i, size);
      if (i == 0) mapunits[index]->up = &outside;
      else mapunits[index]->up = mapunits[index - size];
      if (i == size - 1) mapunits[index]->down = &outside;
      else mapunits[index]->down = mapunits[index + size];
      if (j == 0) mapunits[index]->left = &outside;
      else mapunits[index]->left = mapunits[index - 1];
      if (j == size - 1) mapunits[index]->right = &outside;
      else mapunits[index]->right = mapunits[index + 1];
    }
  }

  selection = {0, 0, size, size};
  // do something with this
  MapUnit* spawnUnit = mapunits[size/2 * size + size/2];
  spawn = new Spawner(this, spawnUnit, &WHITE_TEAM, 8, 3);
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
    unsigned int adjustedX, adjustedY;
    case GAME_CONTEXT_ZOOMED_OUT:
      adjustedX = x - (selection.w / 2);
      adjustedY = y - (selection.h / 2);
      if (adjustedX < 0) adjustedX = 0;
      if (adjustedY < 0) adjustedY = 0;
      if (adjustedX > size - 1) adjustedX = size- 1;
      if (adjustedY > size - 1) adjustedY = size - 1;
      selection.x = adjustedX;
      selection.y = adjustedY;
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      //TODO
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
      for (MapUnit* u : mapunits) {
        if (u->type != UNIT_TYPE_EMPTY){
          if (u->team) disp->setDrawColor(u->team);
          else disp->setDrawColorWhite();
          disp->drawPixel(u->x, u->y);
        }
      }
      disp->setDrawColorWhite();
      disp->drawRect(&selection);
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      MapUnit* firstInRow = mapunits[coordsToSqIndex(selection.x, selection.y, size)];
      MapUnit* u = firstInRow;
      for (int i = 0; i < selection.h && firstInRow->type != UNIT_TYPE_OUTSIDE; i++) {
        for (int j = 0; j < selection.w && u->type != UNIT_TYPE_OUTSIDE; j++) {
          if (u->type != UNIT_TYPE_EMPTY) {
            if (u->team) disp->setDrawColor(u->team);
            else disp->setDrawColorWhite();
            int scale = size / selection.w;
            disp->drawRectFilled((u->x - selection.x) * scale,
                                 (u->y - selection.y) * scale, scale, scale);
          }
          u = u->right;
        }
        firstInRow = firstInRow->down;
        u = firstInRow;
      }
      break;
  }
  if (paused) {
    disp->drawText("PAUSED", 0, 0);
  }
  // Display coordinates of current region selection
  const char *s = (std::to_string(selection.x) + ", " + std::to_string(selection.y)).c_str();
  int w;
  disp->sizeText(s, &w, nullptr);
  disp->drawText(s, disp->getSize() - w, 0);
}

/* Advance each region in game time, which means updating the units in each
   region; if the region contains a spawner it will check if should spawn
   an agent, and attempt to */
void Game::update() {
  for (MapUnit* u: mapunits) {
    if (u->type == UNIT_TYPE_AGENT) {
      u->agent->update();
    }
  }
  spawn->update();
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
      SDL_GetMouseState(&x, &y);
      switch(e.type) {
        case SDL_MOUSEMOTION:
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
            case SDLK_UP:
              selection.w*=2;
              selection.h*=2;
              if (selection.h > size) selection.h = size;
              if (selection.w > size) selection.w = size;
              mouseMoved(x, y);
              break;
            case SDLK_DOWN:
              selection.w/=2;
              selection.h/=2;
              if (selection.w < 1) selection.w = 1;
              if (selection.h < 1) selection.h = 1;
              mouseMoved(x, y);
              break;
          }
          break;
      }
    }
    if (!paused) {
      update();
      t++;
    }
    draw();
    disp->update();
    //disp->wait(10);
  }
}

Game::~Game() {
  for (unsigned int i = 0; i < mapunits.size(); i++) {
    if (mapunits[i]->agent) delete mapunits[i]->agent;
    delete mapunits[i];
  }
  mapunits.clear();
  delete spawn;
}
