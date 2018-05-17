#include "spawner.h"

#include <cstdlib>
#include <ctime>

#include "agent.h"
#include "game.h"
#include "region.h"

/* Constructor of the spawner also tells the argument region that it has a spawner,
   and creates region units where the spawner is */
Spawner::Spawner(Game* g, Region* reg, const Team* t, \
                 unsigned int s, unsigned int t_) : \
                Square(s), timeToCreateAgent(t_), paths(g, reg), region(reg), \
                game(g), team(t) {
  region->containsSpawner = true;
  region->spawn = this;
  srand(time(0));
  int start = region->getRadius() - getRadius();

  for (unsigned int i = start; i < size + start; i++) {
    for (unsigned int j = start; j < start + size; j++) {
      region->regionUnits[i * region->getSize() + j] = RegionUnit(region, team, UNIT_TYPE_SPAWNER, j, i);
    }
  }
}

/* Update the spawner through one tick of game time */
void Spawner::update() {
  // Spawn an Agent if the current game time is a multiple of timeToCreateAgent
  if (game->t % timeToCreateAgent == 0) {
    spawnAgent();
  }
}

/* Try to spawn an agent. Spawner spawns in areas of its size up, down left and
   right; so if all spawn regions are filled, it looks like a cross with the
   spawner at the center and agents filling boxes of the spawner's size on all
   sides */
void Spawner::spawnAgent() {
  // Spawner can create agents in boxes of its size on each of its sides
  int relX = rand() % getSize();
  int relY = rand() % getSize();
  int whichSide = rand() % 4;
  // set spawnX and spawnY to be relX,Y from the top corner of the spawner
  int spawnX = relX + region->getRadius() - getRadius();
  int spawnY = relY + region->getRadius() - getRadius();
  /* Then depending on which side we want to spawn on, spawnx or spawnY will
     either increment or decrement by the size of the spawner */
  int s = getSize();
  int ns = getSize() * -1;
  int spawnIncrementOptions[4][2] = {{s, 0}, {0, s}, {ns, 0}, {0, ns}};
  spawnX += spawnIncrementOptions[whichSide][0];
  spawnY += spawnIncrementOptions[whichSide][1];
  int regionUnitIndex = spawnY * region->getSize() + spawnX;
  /* Check if we can actually spawn an agent in the desired space; if not, just
     don't do it (clear your spawn region!) */
  if (region->regionUnits[regionUnitIndex].type == UNIT_TYPE_EMPTY) {
    region->regionUnits[regionUnitIndex] = RegionUnit(region, team, UNIT_TYPE_AGENT, spawnX, spawnY);
    RegionUnit* uptr = &(region->regionUnits[regionUnitIndex]);
    Agent* a = new Agent(&paths, uptr, team);
    region->agents.push_back(a);
    uptr->agent = a;
  //  a->dx = 1;
    switch(whichSide) {
      case 0:
        a->dx = 1;
        break;
      case 1:
        a->dy = 1;
        break;
      case 2:
        a->dx = -1;
        break;
      case 3:
        a->dy = -1;
        break;
    }
  }
}
