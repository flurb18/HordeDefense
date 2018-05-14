#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>

#include "square.h"

// Forward declarations
class Context;
class Region;
struct Team;

// A spawner is always at the center of its region
class Spawner: public Square {
  unsigned int timeToCreateAgent;
  Context* gameContext;
  Region* region;
  void spawnAgent();
public:
  const Team* team;
  Spawner(Context*, Region*, const Team*, unsigned int, unsigned int);
  void update();
};

#endif
