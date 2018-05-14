#ifndef REGION_H
#define REGION_H

#include <vector>

#include "square.h"

using std::vector;

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
  vector<Agent*> agents;
public:
  Region(Display* d, int x_, int y_, int s): Square(s), x(x_), y(y_), \
         containsSpawner(false), disp(d) {};
  void drawAgents();
  void drawOutline();
  void update();
};

#endif
