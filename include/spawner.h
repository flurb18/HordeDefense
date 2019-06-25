#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>

#include "square.h"
#include "paths.h"

/* Forward declarations */
class Agent;
class Game;
struct MapUnit;
struct Team;

class Spawner: public Square {
private:
  /* How many ticks between Agent spawn attempts */
  unsigned int timeToCreateAgent;
  /* Agents spawned/updated by this spawner */
  std::vector<Agent*> agents;
  MapUnit* topLeft;
  void spawnAgent();

public:
  Game* game;
  const Team* team;
  /* Paths object for agent newborns */
  Paths paths;
  Spawner(Game*, MapUnit*, const Team*, unsigned int, unsigned int);
  ~Spawner();
  void update();
};

#endif
