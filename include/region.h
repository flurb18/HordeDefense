#ifndef REGION_H
#define REGION_H

#include <vector>
#include <SDL2/SDL_rect.h>

#include "display.h"
#include "agent.h"

using std::vector;

class Region {
  int x, y, size;
  SDL_Rect rect;
  vector<Agent*> agents;
public:
  Region(int, int, int);
  void drawAgents(Display);
  void drawOutline(Display d) {d.drawRect(&rect);};
  void addAgent(int x, int y);
};

#endif
