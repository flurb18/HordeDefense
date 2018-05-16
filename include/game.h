#ifndef GAME_H
#define GAME_H

#include <vector>

// Forward declarations
class Context;
class Display;
class Region;
class Spawner;

// Various game contexts
const int GAME_CONTEXT_EXIT = 0;
const int GAME_CONTEXT_ZOOMED_IN = 1;
const int GAME_CONTEXT_ZOOMED_OUT = 2;

struct Team {
  int teamNum;
  int R;
  int G;
  int B;
  Team(int t, int r, int g, int b): teamNum(t), R(r), G(g), B(b) {};
  bool equals(Team that) {return this->teamNum == that.teamNum;};
};

const Team WHITE_TEAM = Team(0, 255, 255, 255);
const Team RED_TEAM = Team(1, 255, 0, 0);
const Team GREEN_TEAM = Team(2, 0, 255, 0);
const Team BLUE_TEAM = Team(3, 0, 0, 255);

class Game {
private:
  Spawner* spawn;
  std::vector<Region*> regions;
  void mouseMoved(int, int);
  void leftMouseClicked(int, int);
  void rightMouseClicked(int, int);
  void draw();
  void updateRegions();
public:
  unsigned int rSize, rPerSide, context, t;
  unsigned int currentRegionIndex, currentUnitIndex;
  bool paused;
  Display* disp;
  unsigned int coordsToSqIndex(int, int, int);
  unsigned int dispCoordsToSqIndex(int, int, int);
  void indexToSqCoords(int, int, int*, int*);
  Game(Display*, const int&, const int&);
  ~Game();
  void mainLoop();
};


#endif
