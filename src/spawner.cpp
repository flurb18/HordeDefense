#include "spawner.h"

#include <cstdlib>
#include <ctime>

#include "agent.h"
#include "game.h"
#include "region.h"

Spawner::Spawner(Game* g, Region* reg, const Team* t, \
                 unsigned int s, unsigned int t_) : \
                Square(s), timeToCreateAgent(t_), region(reg), \
                game(g), team(t) {
  region->containsSpawner = true;
  region->spawn = this;
  srand(time(0));
  int start = region->getRadius() - getRadius();

  for (unsigned int i = start; i < size + start; i++) {
    for (unsigned int j = start; j < start + size; j++) {
      region->regionUnits[i * size + j] = RegionUnit(region, team, UNIT_TYPE_SPAWNER, j, i);
    }
  }
}

void Spawner::update() {
  // Spawn an Agent if the current game time is a multiple of timeToCreateAgent
  if (game->t % timeToCreateAgent == 0) {
    spawnAgent();
  }
}

void Spawner::spawnAgent() {
  // Spawner can create agents in boxes of its size on each of its sides
  int relX = rand() % getSize();
  int relY = rand() % getSize();
  int whichSide = rand() % 4;
  // set spawnX and spawnY to be relX,Y from the top corner of the spawner
  int spawnX = relX + region->getRadius() - getRadius();
  int spawnY = relY + region->getRadius() - getRadius();
  // Then depending on which side we want to spawn on, spawnx or spawnY will
  // either increment or decrement by the size of the spawner
  int s = getSize();
  int ns = getSize() * -1;
  int spawnIncrementOptions[4][2] = {{s, 0}, {0, s}, {ns, 0}, {0, ns}};
  spawnX += spawnIncrementOptions[whichSide][0];
  spawnY += spawnIncrementOptions[whichSide][1];
  int regionUnitIndex = spawnY * region->getSize() + spawnX;
  if (region->regionUnits[regionUnitIndex].type == UNIT_TYPE_EMPTY) {
    region->regionUnits[regionUnitIndex] = Agent(region, team, spawnX, spawnY);
  }
}
