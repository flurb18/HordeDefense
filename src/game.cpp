#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
//#include <iostream>
#include <string>

#include "agent.h"
#include "display.h"
#include "spawner.h"


/* Constructor sets up map units, creates a friendly spawner */
Game::Game(Display* d): Square(d->getSize()), \
                        context(GAME_CONTEXT_ZOOMED_OUT), t(0), paused(true), disp(d), outside(this) {
  /* mapUnits is a vector */
  mapUnits.reserve(size * size);
  /* Units are created in the x direction; it fills the top row with map units
  from left to right, then the next row and so on
  SDL window x and y start at 0 at top left and increase right and
  down respectively*/
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      mapUnits.push_back(new MapUnit(this, j, i));
    }
  }
  /* Interlink the map units so they all know their adjacent units */
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      int index = coordsToSqIndex(j, i, size);
      if (i == 0) mapUnits[index]->up = &outside;
      else mapUnits[index]->up = mapUnits[index - size];
      if (i == size - 1) mapUnits[index]->down = &outside;
      else mapUnits[index]->down = mapUnits[index + size];
      if (j == 0) mapUnits[index]->left = &outside;
      else mapUnits[index]->left = mapUnits[index - 1];
      if (j == size - 1) mapUnits[index]->right = &outside;
      else mapUnits[index]->right = mapUnits[index + 1];
    }
  }

  selection = {0, 0, getRadius(), getRadius()};
  zoomSelection = {0,0,getRadius(),getRadius()};
  // do something with this
  MapUnit* spawnUnit = mapUnits[size/2 * size + size/2];
  zoomSelectedUnit = spawnUnit;
  spawn = new Spawner(this, spawnUnit, &BLUE_TEAM, 8, 3);
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
      int adjustedX, adjustedY;
      adjustedX = x - (selection.w / 2);
      adjustedY = y - (selection.h / 2);
      if (adjustedX < 0) adjustedX = 0;
      if (adjustedY < 0) adjustedY = 0;
      if (adjustedX > (int) size - selection.w) adjustedX = size - selection.w;
      if (adjustedY > (int) size - selection.h) adjustedY = size - selection.h;
      selection.x = adjustedX;
      selection.y = adjustedY;
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      /* Width and height of each unit in current view */
      int scaleX = size / selection.w;
      int scaleY = size / selection.h;
      int mouseUnitX = x / scaleX;
      int mouseUnitY = y / scaleY;
      int mouseUnitAbsoluteX = mouseUnitX + selection.x;
      int mouseUnitAbsoluteY = mouseUnitY + selection.y;
      zoomSelectedUnit = mapUnits[coordsToSqIndex(mouseUnitAbsoluteX, mouseUnitAbsoluteY, size)];
      zoomSelection.x = mouseUnitX * scaleX;
      zoomSelection.y = mouseUnitY * scaleY;
      zoomSelection.w = scaleX;
      zoomSelection.h = scaleY;
      break;
  }

}

/* Handle a left mouse click at (x,y) */
void Game::leftMouseClicked(int x, int y) {
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      context = GAME_CONTEXT_ZOOMED_IN;
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      //TODO
      break;
  }
}

/* Handle a right mouse click at (x,y) */
void Game::rightMouseClicked(int x, int y) {
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      //TODO
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      context = GAME_CONTEXT_ZOOMED_OUT;
      break;
  }
}


//TODO for each of these: handle game context zoomed in
void Game::zoomSelectionIn(int x, int y) {
  selection.w/=2;
  selection.h/=2;
  if (selection.w < 1) selection.w = 1;
  if (selection.h < 1) selection.h = 1;
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      mouseMoved(x, y);
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      // Ideally zoom in around the mouse
      selection.x += selection.w / 2;
      selection.y += selection.h / 2;
      mouseMoved(x, y);
      break;
  }
}

void Game::zoomSelectionOut(int x, int y) {
  int oldSelectionSize = selection.w;
  selection.w*=2;
  selection.h*=2;
  if (selection.h > getRadius()) selection.h = getRadius();
  if (selection.w > getRadius()) selection.w = getRadius();
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      mouseMoved(x, y);
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      // Ideally zoom out around the mouse
      if (oldSelectionSize != selection.w) {
        selection.x -= selection.w / 4;
        selection.y -= selection.h / 4;
        if (selection.x < 0) selection.x = 0;
        if (selection.y < 0) selection.y = 0;
        if (selection.x > (int) size - selection.w) selection.x = size - selection.w;
        if (selection.y > (int) size - selection.h) selection.y = size - selection.h;
      }
      mouseMoved(x, y);
      break;
  }
}

/* Draw the current game screen based on context */
void Game::draw() {
  disp->fillBlack();
  switch(context) {
    case GAME_CONTEXT_ZOOMED_OUT:
      for (MapUnit* u : mapUnits) {
        /* Only draw nonempty units */
        if (u->type != UNIT_TYPE_EMPTY) {
          /* Set the color to the units team, of white if it has none */
          if (u->team) disp->setDrawColor(u->team);
          else disp->setDrawColorWhite();
          disp->drawPixel(u->x, u->y);
        }
      }
      disp->setDrawColorWhite();
      disp->drawRect(&selection);
      break;
    case GAME_CONTEXT_ZOOMED_IN:
      MapUnit* first = mapUnits[coordsToSqIndex(selection.x, selection.y, size)];
      /* Iterate over selection */
      for (MapUnit::iterator iter = first->getIterator(selection.w, selection.h); \
           iter.hasNext(); iter++) {
        if (iter->type != UNIT_TYPE_EMPTY) {
          if (iter->team) disp->setDrawColor(iter->team);
          else disp->setDrawColorWhite();
          int scaleX = size / selection.w;
          int scaleY = size / selection.h;
          int scaledX = (iter->x - selection.x) * scaleX;
          int scaledY = (iter->y - selection.y) * scaleY;
          if (iter->type == UNIT_TYPE_SPAWNER && ((iter.i + iter.j) % 2) == 0)
            disp->setDrawColorBlack();
          disp->drawRectFilled(scaledX, scaledY, scaleX, scaleY);
        }
      }
      disp->setDrawColorWhite();
      disp->drawRect(&zoomSelection);
      if (zoomSelectedUnit->type != UNIT_TYPE_EMPTY) {
        std::string unitTypeString = "";
        switch(zoomSelectedUnit->type) {
          case UNIT_TYPE_AGENT:
            unitTypeString = "Agent @ ";
            break;
          case UNIT_TYPE_SPAWNER:
            unitTypeString = "Spawner @ ";
        }
        unitTypeString += std::to_string(zoomSelectedUnit->x) + ", " + std::to_string(zoomSelectedUnit->y);
        int unitTypeStringWidth;
        int unitTypeStringHeight;
        const char *unitInfoCstr = unitTypeString.c_str();
        disp->sizeText(unitInfoCstr, &unitTypeStringWidth, &unitTypeStringHeight);
        disp->drawText(unitInfoCstr, disp->getSize() - unitTypeStringWidth, unitTypeStringHeight);
      }
      break;
  }
  if (paused) {
    disp->drawText("PAUSED", 0, 0);
  }
  // Display coordinates of current selection
  const char *s = (std::to_string(selection.x) + ", " + std::to_string(selection.y)).c_str();

  int w;
  disp->sizeText(s, &w, nullptr);
  disp->drawText(s, disp->getSize() - w, 0);
}

/* Update the spawners, which will update the agents they track */
void Game::update() {
  spawn->update();
}

/* Main loop of the game; largely just handles events, calls draw() and update()
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
              zoomSelectionOut(x,y);
              break;
            case SDLK_DOWN:
              zoomSelectionIn(x,y);
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
    disp->wait(10);
  }
}

Game::~Game() {
  delete spawn;
  /* Reset inter-mapunit links, so we don't have bad pointer segfaults on deletion */
  for (MapUnit* u : mapUnits) {
    u->left = nullptr;
    u->right = nullptr;
    u->down = nullptr;
    u->up = nullptr;
  }
  for (unsigned int i = 0; i < mapUnits.size(); i++) {
    delete mapUnits[i];
  }
  mapUnits.clear();
}
