#ifndef GAME_H
#define GAME_H

#include <vector>

// Forward declarations
class Context;
class Display;
class Region;
class Spawner;


class Game {
private:
  unsigned int regionSize;
  unsigned int regionsPerSide;
  Context* gameContext;
  Display* disp;
  Spawner* spawn;
  std::vector<Region*> regions;
  unsigned int regCoordsToIndex(int, int);
  unsigned int winCoordsToIndex(int, int);
  void indexToRegCoords(int, int*, int*);
  void mouseMoved(int, int);
  void leftMouseClicked(int, int);
  void rightMouseClicked(int, int);
  void draw();
  void updateRegions();
public:
  Game(Context*, Display*, const int&, const int&);
  ~Game();
  void mainLoop();
};


#endif
