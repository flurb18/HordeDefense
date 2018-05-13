#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>

#include "context.h"

using std::vector;

// Forward declaration of Region, Display
class Display;
class Region;

class Game {
private:
  Display* disp;
  vector<Region*> regions;
  void mouseMoved(int, int);
  int regionSize;
  int regionsPerSide;
public:
  Context gameContext;
  bool paused;
  Game(Display*, const int&, const int&);
  void mainLoop();
  void draw();
};


#endif
