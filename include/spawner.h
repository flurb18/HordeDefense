#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>

#include "game.h"
#include "square.h"

// Forward declarations
class Region;
class Game;
struct Team;

// A spawner is always at the center of its region
class Spawner: public Square {
  unsigned int timeToCreateAgent;
  Region* region;
  void spawnAgent();
public:
  Game* game;
  const Team* team;
  Spawner(Game*, Region*, const Team*, unsigned int, unsigned int);
  void update();
};

#endif
