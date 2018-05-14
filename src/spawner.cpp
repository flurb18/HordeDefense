#include "spawner.h"

#include <cstdlib>

#include "agent.h"
#include "context.h"
#include "region.h"
#include "square.h"
#include "team.h"

Spawner::Spawner(Context* c, Region* reg, const Team* t, \
                 unsigned int s, unsigned int t_) : \
                Square(s), timeToCreateAgent(t_), gameContext(c), region(reg), \
                team(t) {
  region->containsSpawner = true;
  region->spawn = this;
}

void Spawner::update() {
  // Spawn an Agent if the current game time is a multiple of timeToCreateAgent
  if (gameContext->t % timeToCreateAgent == 0) {
    spawnAgent();
  }
}

void Spawner::spawnAgent() {
  // Set the seed for the RNG as the current game time
  srand(gameContext->t);
  // Spawner can create agents in boxes of its size on each of its sides
  int relX = rand() % getSize();
  int relY = rand() % getSize();
  int whichSide = rand() % 4;
  // Set spawnX and spawnY to be the top right corner of the spawner
  int spawnX = region->x + region->getRadius() - getRadius();
  int spawnY = region->y + region->getRadius() - getRadius();
  // Then depending on which side we want to spawn on, spawnx or spawnY will
  // either increment or decrement by the size of the spawner
  int s = getSize();
  int ns = getSize() * -1;
  int spawnIncrementOptions[4][2] = {{s, 0}, {0, s}, {ns, 0}, {0, ns}};
  spawnX += spawnIncrementOptions[whichSide][0];
  spawnY += spawnIncrementOptions[whichSide][1];
  Agent *a = new Agent(gameContext, region, team, spawnX + relX, spawnY + relY);
  region->agents.push_back(a);
}
