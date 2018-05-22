#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>

#include "game.h"
#include "square.h"
#include "paths.h"

/* Forward declarations */
class Region;
class Game;
struct Team;

class Spawner: public Square {
private:
  /* How many ticks between Agent spawn attempts */
  unsigned int timeToCreateAgent;
  /* Paths object for agent newborns */
  Paths paths;
  /* Region this spawner is in */
  MapUnit* topLeft;
  void spawnAgent();

public:
  Game* game;
  const Team* team;
  Spawner(Game*, MapUnit*, const Team*, unsigned int, unsigned int);
  void update();
};

#endif
