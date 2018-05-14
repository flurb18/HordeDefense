#ifndef SPAWNER_H
#define SPAWNER_H

#include "square.h"

// Forward declarations
class Context;
class Region;
struct Team;

// A spawner is always at the center of its region
class Spawner: public Square {
  Context* gameContext;
  Region* spawnerRegion;
public:
  const Team* team;
  Spawner(Context*, Region*, const Team*, unsigned int s);
  void spawnAgent();
};

#endif
