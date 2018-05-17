#include "agent.h"

#include <algorithm>
#include <iostream>

#include "game.h"
#include "region.h"

Agent::Agent(Paths* p, RegionUnit* r, const Team* t):
             region(r->region), rUnit(r), paths(p), team(t) {
  index = region->game->coordsToSqIndex(rUnit->regX, rUnit->regY, region->size);
  paths = p;
  objective = OBJECTIVE_TYPE_EXPLORE;
  dx = 0;
  dy = 0;
}

/* Update agent based on objective */
void Agent::update() {
  switch(objective) {
    case OBJECTIVE_TYPE_EXPLORE:
      move();
      break;
    case OBJECTIVE_TYPE_RUN:
      move();
      break;
  }
}

/* Attempts to move the agent; returns true if successful, false otherwise */
bool Agent::move() {
  int destX = rUnit->regX + dx;
  int destY = rUnit->regY + dy;
  /* Handle the 4 cases of the agent moving out of the region */
  if (destX == -1 || destX == region->getSize() ||\
      destY == -1 || destY == region->getSize()) {
    //TODO
    return true;
  }
  /* Otherwise, the agent will still be inside the region; check if the space is
     occupied */
  int destIndex = region->game->coordsToSqIndex(destX, destY, region->size);
  RegionUnit* destUnit = &(region->regionUnits[destIndex]);
  if (destUnit->type != UNIT_TYPE_EMPTY) {
    if (destUnit->type == UNIT_TYPE_DOOR) {
      if (team->equals(destUnit->team)) {
        if (dx) dx = 2;
        if (dy) dy = 2;
        bool moved = move();
        if (dx) dx = 1;
        if (dy) dy = 1;
        return moved;
      }
    }
    return false;
  }
  // Switch around the two region units, which just means exchanging their data
  rUnit->type = UNIT_TYPE_EMPTY;
  rUnit->agent = nullptr;
  rUnit->team = nullptr;
  destUnit->type = UNIT_TYPE_AGENT;
  destUnit->agent = this;
  destUnit->team = team;
  rUnit = destUnit;
  index = destIndex;
  return true;
}
