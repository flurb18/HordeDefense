#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL_rect.h>

#include "mapunit.h"
#include "square.h"
#include "teams.h"

/* Forward declarations */
class Display;
class Spawner;

/* Various game contexts */
const int GAME_CONTEXT_EXIT = 0;
const int GAME_CONTEXT_UNSELECTED = 1;
const int GAME_CONTEXT_SELECTING = 2;
const int GAME_CONTEXT_SELECTED = 3;

class Game: public Square {
private:
  Spawner* spawn;
  void mouseMoved(int, int);
  void leftMouseDown(int, int);
  void leftMouseUp(int, int);
  void rightMouseDown(int, int);
  void zoomViewIn(int, int);
  void zoomViewOut(int, int);
  void panViewLeft();
  void panViewRight();
  void panViewUp();
  void panViewDown();
  MapUnit::iterator getSelectionIterator();
  void adjustSelection(MapUnit::iterator);
  void draw();
  void update();
public:
  int context;
  /* Game ticks since this object was created */
  unsigned long long int t;
  /* Display ticks since this object was created */
  unsigned long long int dispT;
  /* Whether or not the game is currently paused */
  bool paused;
  /* Scale of current game view; size of units in pixels */
  int scaleX, scaleY;
  /* Current selection of units; PIXEL coordinates in view*/
  SDL_Rect selection;
  /* Current view of units; MAP UNIT coordinates */
  SDL_Rect view;
  /* Vector of map units */
  std::vector<MapUnit*> mapUnits;
  Display* disp;
  MapUnit outside;
  MapUnit* selectedUnit;
  unsigned int coordsToSqIndex(int, int, int);
  unsigned int dispCoordsToSqIndex(int, int, int);
  void indexToSqCoords(int, int, int*, int*);
  Game(int);
  ~Game();
  void mainLoop();
};

#endif
