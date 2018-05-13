#ifndef SPAWNER_H
#define SPAWNER_H

// Forward declaration of Region class
class Region;

class Spawner {
  Region* reg;
public:
  Spawner(Region*);
};

#endif
