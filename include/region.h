#ifndef REGION_H
#define REGION_H

#include <vector>

#include "square.h"

// Forward declaration of Display and Agent classes
class Display;
class Agent;
class Spawner;

class Region: public Square {
  friend class Spawner;
  int x, y;
  bool containsSpawner;
  Display* disp;
  Spawner* spawn;
  std::vector<Agent*> agents;
public:
  Region(Display* d, int x_, int y_, int s): Square(s), x(x_), y(y_), \
         containsSpawner(false), disp(d) {};
  ~Region();
  void drawAgents();
  void drawAgentsZoomedIn();
  void drawOutline();
  void update();
};

#endif
