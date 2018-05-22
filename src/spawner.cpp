#include "spawner.h"

#include <cstdlib>
#include <ctime>

#include "agent.h"
#include "game.h"

/* Constructor of the spawner also tells the argument region that it has a spawner,
   and creates region units where the spawner is */
Spawner::Spawner(Game* g, MapUnit* u, const Team* t, \
                 unsigned int s, unsigned int t_) : Square(s), \
                 timeToCreateAgent(t_), paths(g, u), topLeft(u), game(g), team(t) {
  srand(time(0));
  MapUnit* firstInRow = topLeft;
  MapUnit* current = topLeft;
  for (unsigned int i = 0; i < size && firstInRow->type != UNIT_TYPE_OUTSIDE; i++) {
    for (unsigned int j = 0; j < size && current->type != UNIT_TYPE_OUTSIDE; j++) {
      current->type = UNIT_TYPE_SPAWNER;
      current = current->right;
    }
    firstInRow = firstInRow->down;
    current = firstInRow;
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
  int spawnX = relX + topLeft->x;
  int spawnY = relY + topLeft->y;
  /* Then depending on which side we want to spawn on, spawnx or spawnY will
     either increment or decrement by the size of the spawner */
  int s = getSize();
  int ns = getSize() * -1;
  int spawnIncrementOptions[4][2] = {{s, 0}, {0, s}, {ns, 0}, {0, ns}};
  spawnX += spawnIncrementOptions[whichSide][0];
  spawnY += spawnIncrementOptions[whichSide][1];
  int spawnUnitIndex = game->coordsToSqIndex(spawnX, spawnY, game->getSize());
  /* Check if we can actually spawn an agent in the desired space; if not, just
     don't do it (clear your spawn region!) */
  if (game->mapunits[spawnUnitIndex]->type == UNIT_TYPE_EMPTY) {
    MapUnit* uptr = game->mapunits[spawnUnitIndex];
    Agent* a = new Agent(game, &paths, uptr, team);
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
