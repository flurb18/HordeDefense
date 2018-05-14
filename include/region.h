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
  Spawner* spawn;
  bool containsSpawner;
  Display* disp;
  vector<Agent*> agents;
public:
  Region(int, int, int, Display*);
  void drawAgents();
  void drawOutline();
};

#endif
