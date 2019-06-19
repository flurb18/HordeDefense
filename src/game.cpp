#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <string>
#include <math.h>

#include "agent.h"
#include "display.h"
#include "spawner.h"


/* Constructor sets up map units, creates a friendly spawner */
Game::Game(int s): Square(s), context(GAME_CONTEXT_UNSELECTED), t(0), dispT(0), paused(true), outside(this) {
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

  /* Set initial selection to just be first map unit */
  selection = {0, 0, 1, 1};
  /* Set initial view to be the maximum power of two under the game size, centered
     around the middle of the board and initialize the display */
  disp = new Display((int)pow(2,(int)log2(size)));
  int viewOffset = (size - disp->getSize())/2;
  view = {viewOffset,viewOffset,disp->getSize(),disp->getSize()};
  /* Set scale variables */
  scaleX = size / view.w;
  scaleY = size / view.h;
  MapUnit* spawnUnit = mapUnits[coordsToSqIndex(size/2,size/2,size)];
  selectedUnit = spawnUnit;
  spawn = new Spawner(this, spawnUnit, &GREEN_TEAM, 8, 3);

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
  /* Width and height of each unit in current view */
  int mouseUnitX = x / scaleX;
  int mouseUnitY = y / scaleY;
  if (mouseUnitX > view.w) mouseUnitX = view.w;
  if (mouseUnitY > view.h) mouseUnitY = view.h;
  if (mouseUnitX < 0) mouseUnitX = 0;
  if (mouseUnitY < 0) mouseUnitY = 0;
  int mouseUnitAbsoluteX = mouseUnitX + view.x;
  int mouseUnitAbsoluteY = mouseUnitY + view.y;
  switch(context) {
    case GAME_CONTEXT_UNSELECTED:
      selectedUnit = mapUnits[coordsToSqIndex(mouseUnitAbsoluteX, mouseUnitAbsoluteY, size)];
      selection.x = mouseUnitX * scaleX;
      selection.y = mouseUnitY * scaleY;
      selection.w = scaleX;
      selection.h = scaleY;
      break;
    case GAME_CONTEXT_SELECTING:
      if (mouseUnitAbsoluteX >= (int)selectedUnit->x) {
        selection.w = (mouseUnitAbsoluteX - selectedUnit->x) * scaleX;
      } else {
        selection.x = mouseUnitX * scaleX;
        selection.w = (selectedUnit->x - mouseUnitAbsoluteX) * scaleX;
      }
      if (mouseUnitAbsoluteY >= (int)selectedUnit->y) {
        selection.h = (mouseUnitAbsoluteY - selectedUnit->y) * scaleY;
      } else {
        selection.y = mouseUnitY * scaleY;
        selection.h = (selectedUnit->y - mouseUnitAbsoluteY) * scaleY;
      }
      if (selection.w == 0) selection.w = scaleX;
      if (selection.h == 0) selection.h = scaleY;
      break;
    case GAME_CONTEXT_SELECTED:
      break;
  }
}

/* Handle a left mouse down at (x,y) */
void Game::leftMouseDown(int x, int y) {
  context = GAME_CONTEXT_UNSELECTED;
  mouseMoved(x,y);
  context = GAME_CONTEXT_SELECTING;
}

/* Handle a left mouse up at (x,y) */
void Game::leftMouseUp(int x, int y) {
  context = GAME_CONTEXT_SELECTED;
}

/* Handle a right mouse click at (x,y) */
void Game::rightMouseDown(int x, int y) {
  context = GAME_CONTEXT_UNSELECTED;
}

/* Self explanatory zooming / panning functions */
void Game::zoomViewIn(int x, int y) {
  mouseMoved(x,y);
  if (context == GAME_CONTEXT_UNSELECTED) {
    if (view.w >= 8) view.w /=2;
    if (view.h >= 8) view.h /=2;
    view.x = selectedUnit->x - view.w/2;
    view.y = selectedUnit->y - view.h/2;
    if (view.x < 0) view.x = 0;
    if (view.y < 0) view.y = 0;
    if (view.x > (int)size - view.w) view.x = size - view.w;
    if (view.y > (int)size - view.h) view.y = size - view.h;
  }
  scaleX = disp->getSize() / view.w;
  scaleY = disp->getSize() / view.h;
  mouseMoved(x,y);
}

void Game::zoomViewOut(int x, int y) {
  mouseMoved(x,y);
  if (context == GAME_CONTEXT_UNSELECTED && view.w < disp->getSize()) {
    view.w *= 2;
    view.h *= 2;
    view.x = selectedUnit->x - view.w/2;
    view.y = selectedUnit->y - view.h/2;
    if (view.x < 0) view.x = 0;
    if (view.y < 0) view.y = 0;
    if (view.x > (int)size - view.w) view.x = size - view.w;
    if (view.y > (int)size - view.h) view.y = size - view.h;
    if (view.w >= disp->getSize()-1 || view.h >= disp->getSize()-1) {
      view.w = disp->getSize();
      view.h = disp->getSize();
    }
  }
  scaleX = disp->getSize() / view.w;
  scaleY = disp->getSize() / view.h;
  mouseMoved(x,y);
}

void Game::panViewLeft() {
  if (context == GAME_CONTEXT_UNSELECTED) {
    view.x -= view.w/4;
    if (view.x < 0) view.x = 0;
  }
}

void Game::panViewRight() {
  if (context == GAME_CONTEXT_UNSELECTED) {
    view.x += view.w/4;
    if (view.x > (int)size - view.w) view.x = size - view.w;
  }
}

void Game::panViewUp() {
  if (context == GAME_CONTEXT_UNSELECTED) {
    view.y -= view.h/4;
    if (view.y < 0) view.y = 0;
  }
}

void Game::panViewDown() {
  if (context == GAME_CONTEXT_UNSELECTED) {
    view.y += view.h/4;
    if (view.y > (int)size - view.h) view.y = size - view.h;
  }
}

/* Get MapUnit iterator of the MapUnits in the current selected region */
MapUnit::iterator Game::getSelectionIterator() {
  int selectionUnitX = (selection.x / scaleX) + view.x;
  int selectionUnitY = (selection.y / scaleY) + view.y;
  int selectionUnitWidth = selection.w / scaleX;
  int selectionUnitHeight = selection.h / scaleY;
  MapUnit* first = mapUnits[coordsToSqIndex(selectionUnitX, selectionUnitY, size)];
  return first->getIterator(selectionUnitWidth, selectionUnitHeight);
}

/* Draw the current game screen based on context */
void Game::draw() {
  disp->fillBlack();
  MapUnit* first = mapUnits[coordsToSqIndex(view.x, view.y, size)];
  /* Iterate over view */
  for (MapUnit::iterator iter = first->getIterator(view.w, view.h); \
  iter.hasNext(); iter++) {
    if (iter->type != UNIT_TYPE_EMPTY) {
      if (iter->team) disp->setDrawColor(iter->team);
      else disp->setDrawColorWhite();
      int scaledX = (iter->x - view.x) * scaleX;
      int scaledY = (iter->y - view.y) * scaleY;
      // Checker the spawner
      if (iter->type == UNIT_TYPE_SPAWNER && ((iter.i + iter.j) % 2) == 0) disp->setDrawColorBlack();
      disp->drawRectFilled(scaledX, scaledY, scaleX, scaleY);
    }
  }
  /* Blink selection */
  disp->setDrawColorBlack();
  if (context == GAME_CONTEXT_SELECTED) {
    for (MapUnit::iterator iter = getSelectionIterator(); iter.hasNext(); iter++) {
      int scaledX = (iter->x - view.x) * scaleX;
      int scaledY = (iter->y - view.y) * scaleY;
      if (dispT % 20 > 10) disp->drawRectFilled(scaledX, scaledY, scaleX, scaleY);
    }
  }
  disp->setDrawColorWhite();
  disp->drawRect(&selection);
  std::string unitInfo = "";
  switch(selectedUnit->type) {
    case UNIT_TYPE_AGENT:
    unitInfo = "Agent @ ";
    break;
    case UNIT_TYPE_SPAWNER:
    unitInfo = "Spawner @ ";
    break;
  }
  unitInfo += std::to_string(selectedUnit->x) + ", " + std::to_string(selectedUnit->y);
  int unitInfoWidth;
  int unitInfoHeight;
  const char *unitInfoCstr = unitInfo.c_str();
  disp->sizeText(unitInfoCstr, &unitInfoWidth, &unitInfoHeight);
  disp->drawText(unitInfoCstr, disp->getSize() - unitInfoWidth, 0);
  if (paused) disp->drawText("PAUSED", 0, 0);
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
        case SDL_MOUSEWHEEL:
          if (e.wheel.y > 0) zoomViewIn(x,y);
          else if (e.wheel.y < 0) zoomViewOut(x,y);
        case SDL_MOUSEMOTION:
          mouseMoved(x, y);
          break;
        case SDL_MOUSEBUTTONDOWN:
          switch(e.button.button) {
            case SDL_BUTTON_LEFT:
              leftMouseDown(x, y);
              break;
            case SDL_BUTTON_RIGHT:
              rightMouseDown(x, y);
              break;
          }
          mouseMoved(x, y);
          break;
        case SDL_MOUSEBUTTONUP:
          switch(e.button.button) {
            case SDL_BUTTON_LEFT:
              leftMouseUp(x, y);
              break;
          }
          mouseMoved(x,y);
          break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_SPACE:
              paused = !paused;
              break;
            case SDLK_UP:
              panViewUp();
              mouseMoved(x,y);
              break;
            case SDLK_DOWN:
              panViewDown();
              mouseMoved(x,y);
              break;
            case SDLK_RIGHT:
              panViewRight();
              mouseMoved(x,y);
              break;
            case SDLK_LEFT:
              panViewLeft();
              mouseMoved(x,y);
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
    dispT++;
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
  disp->end();
}
