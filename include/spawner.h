#ifndef SPAWNER_H
#define SPAWNER_H

// Forward declaration of Region class
class Region;

// A spawner is always at the center of its region
class Spawner {
  Region* reg;
public:
  Spawner(Region*);
  void spawnUnit();
};

#endif
