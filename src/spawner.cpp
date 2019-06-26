#include "spawner.h"

#include <cstdlib>

#include "agent.h"
#include "game.h"

/* Constructor of the spawner also tells the argument map unit that it has a spawner,
   and sets the map units where the spawner is */
Spawner::Spawner(Game* g, MapUnit* u, const Team* t, \
                 unsigned int s, unsigned int t_) : Square(s), \
                 timeToCreateAgent(t_), topLeft(u), game(g), team(t) {
  for (MapUnit::iterator iter = topLeft->getIterator(size, size); iter.hasNext(); iter++) {
    iter->type = UNIT_TYPE_SPAWNER;
    iter->team = t;
  }
}

/* Update the spawner through one tick of game time */
void Spawner::update() {
  /* Spawn an Agent if the current game time is a multiple of timeToCreateAgent */
  if (game->t % timeToCreateAgent == 0) {
    spawnAgent();
  }
  /* Update the agents this spawner tracks */
  for (Agent* a: agents) a->update();
}

/* Try to spawn an agent. Spawner spawns in areas of its size up, down left and
   right; so if all spawn map units are filled, it looks like a cross with the
   spawner at the center and agents filling boxes of the spawner's size on all
   sides */
void Spawner::spawnAgent() {
  /* Set spawnX and spawnY to be random unit from the top left corner of the
     spawner to the bottom right */
  int spawnX = (rand() % size) + topLeft->x;
  int spawnY = (rand() % size) + topLeft->y;
  /* Then choose a random side of the spawner to spawn on; depending on which
     side we want to spawn on, spawnx or spawnY will either increment or
     decrement by the size of the spawner */
  int whichSide = rand() % 4;
  /* Convert unsigned int size to normal int so decrement works */
  int s = size;
  /* Do the random changing of the spawn location */
  int spawnIncrementOptions[4][2] = {{s, 0}, {0, s}, {-s, 0}, {0, -s}};
  spawnX += spawnIncrementOptions[whichSide][0];
  spawnY += spawnIncrementOptions[whichSide][1];
  int spawnUnitIndex = game->coordsToSqIndex(spawnX, spawnY, game->getSize());
  /* Check if we can actually spawn an agent in the desired space; if not, just
     don't do it (clear the area around your spawn!) */
  if (game->mapUnits[spawnUnitIndex]->type == UNIT_TYPE_EMPTY) {
    MapUnit* uptr = game->mapUnits[spawnUnitIndex];
    /* Setting the map unit as UNIT_TYPE_AGENT and setting its reference to the
       agent it contains is done in the constructor of Agent */
    agents.push_back(new Agent(game, uptr, team));
    switch(whichSide) {
      case 0:
        agents.back()->dx = 1;
        break;
      case 1:
        agents.back()->dy = 1;
        break;
      case 2:
        agents.back()->dx = -1;
        break;
      case 3:
        agents.back()->dy = -1;
        break;
    }
  }
}

Spawner::~Spawner() {
  for (unsigned int i = 0; i < agents.size(); i++) delete agents[i];
  agents.clear();
}
