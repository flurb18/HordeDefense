#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>

#include "game.h"
#include "square.h"

/* Forward declarations */
class Region;
class Game;
struct Team;

class Spawner: public Square {
  /* How many ticks between Agent spawn attempts */
  unsigned int timeToCreateAgent;
  /* Region this spawner is in */
  Region* region;
  void spawnAgent();
public:
  Game* game;
  const Team* team;
  Spawner(Game*, Region*, const Team*, unsigned int, unsigned int);
  void update();
};

#endif
