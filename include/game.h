#ifndef GAME_H
#define GAME_H

#include <vector>

using std::vector;

// Forward declaration of Region, Display and Context
class Display;
class Region;
class Context;

class Game {
private:
  Display* disp;
  vector<Region*> regions;
  void mouseMoved(int, int);
  int regionSize;
  int regionsPerSide;
public:
  Context* gameContext;
  Game(Context*, Display*, const int&, const int&);
  void mainLoop();
  void draw();
};


#endif
