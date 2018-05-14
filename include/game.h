#ifndef GAME_H
#define GAME_H

#include <vector>

using std::vector;

// Forward declarations
class Context;
class Display;
class Region;
class Spawner;


class Game {
private:
  unsigned int regionSize;
  unsigned int regionsPerSide;
  unsigned int regionRadius;
  Display* disp;
  Spawner* spawn;
  vector<Region*> regions;
  unsigned int regCoordsToIndex(int, int);
  unsigned int winCoordsToIndex(int, int);
  void mouseMoved(int, int);
  void draw();
public:
  Context* gameContext;
  Game(Context*, Display*, const int&, const int&);
  void mainLoop();
};


#endif
