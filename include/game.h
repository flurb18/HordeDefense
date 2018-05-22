#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL_rect.h>

#include "mapunit.h"
#include "square.h"

/* Forward declarations */
class Context;
class Display;
class Region;
class Spawner;

/* Various game contexts */
const int GAME_CONTEXT_EXIT = 0;
const int GAME_CONTEXT_ZOOMED_IN = 1;
const int GAME_CONTEXT_ZOOMED_OUT = 2;

/* Data structure to contain information about a team; unique team number and
   color */
struct Team {
  int teamNum;
  int R;
  int G;
  int B;
  Team(int t, int r, int g, int b): teamNum(t), R(r), G(g), B(b) {};
  bool equals(const Team* that) const {return this->teamNum == that->teamNum;};
};

/* Predefined teams */
const Team WHITE_TEAM = Team(0, 255, 255, 255);
const Team RED_TEAM = Team(1, 255, 0, 0);
const Team GREEN_TEAM = Team(2, 0, 255, 0);
const Team BLUE_TEAM = Team(3, 0, 0, 255);

class Game: public Square {
private:
  Spawner* spawn;
  void mouseMoved(int, int);
  void leftMouseClicked(int, int);
  void rightMouseClicked(int, int);
  void draw();
  void update();
public:
  int context;
  /* Game ticks since this object was created */
  unsigned long long t;
  /* Whether or not the game is currently paused */
  bool paused;
  SDL_Rect selection;
  std::vector<MapUnit*> mapUnits;
  Display* disp;
  MapUnit outside;
  unsigned int coordsToSqIndex(int, int, int);
  unsigned int dispCoordsToSqIndex(int, int, int);
  void indexToSqCoords(int, int, int*, int*);
  Game(Display*);
  ~Game();
  void mainLoop();
};


#endif
