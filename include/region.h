#ifndef REGION_H
#define REGION_H

#include <vector>
#include <SDL2/SDL_rect.h>

using std::vector;

// Forward declaration of Display  and Agent classes
class Display;
class Agent;

class Region {
  int x, y, size;
  SDL_Rect rect;
  Display* disp;
  vector<Agent*> agents;
public:
  Region(int, int, int, Display*);
  void drawAgents();
  void drawOutline();
  void addAgent(int x, int y);
};

#endif
