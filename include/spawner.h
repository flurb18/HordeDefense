#ifndef SPAWNER_H
#define SPAWNER_H

#include "square.h"

// Forward declaration of Region class
class Region;

// A spawner is always at the center of its region
class Spawner: public Square {
  Region* reg;
  unsigned int spawnerRadius;
public:
  Spawner(Region*, unsigned int s);
  void spawnUnit();
};

#endif
