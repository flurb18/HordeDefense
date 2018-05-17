#include "agent.h"

#include <algorithm>
#include <iostream>
#include <set>

#include "game.h"
#include "region.h"

Agent::Agent(Game* g, Paths* p, RegionUnit* r, const Team* t):
             game(g), region(r->region), rUnit(r), paths(p), team(t) {
  index = game->coordsToSqIndex(rUnit->regX, rUnit->regY, region->size);
  paths = p;
  objective = OBJECTIVE_TYPE_EXPLORE;
  dx = 0;
  dy = 0;
}

/* Update agent based on objective */
void Agent::update() {
  switch(objective) {
    case OBJECTIVE_TYPE_EXPLORE:
      //TODO
      move();
      break;
    case OBJECTIVE_TYPE_CHARGE:
      if (!move()) {
        dx = 0;
        dy = 0;
      }
      break;
  }
}

bool Agent::moveTo(Region* destRegion, int destX, int destY) {
  int destIndex = game->coordsToSqIndex(destX, destY, game->rSize);
  RegionUnit* destUnit = &destRegion->regionUnits[destIndex];
  if (destUnit->type != UNIT_TYPE_EMPTY) return false;
  rUnit->type = UNIT_TYPE_EMPTY;
  rUnit->agent = nullptr;
  rUnit->team = nullptr;
  destUnit->type = UNIT_TYPE_AGENT;
  destUnit->agent = this;
  destUnit->team = team;
  rUnit = destUnit;
  index = destIndex;
  region = destRegion;
  return true;
}

/* Attempts to move the agent; returns true if successful, false otherwise */
bool Agent::move() {
  if (dx == 0 && dy == 0) return false;
  int destX = rUnit->regX + dx;
  int destY = rUnit->regY + dy;
  /* Handle the 4 cases of the agent moving out of the region */
  int max = region->getSize() - 1;
  if (destX < 0 || destX > max || destY < 0 || destY > max) {
    int destRegionX, destRegionY;
    int rPerSide = game->rPerSide;
    game->indexToSqCoords(region->index, rPerSide, &destRegionX, &destRegionY);
    if (destX < 0) {
      destRegionX--;
      destX = max;
    }
    if (destX > max) {
      destRegionX++;
      destX = 0;
    }
    if (destY < 0) {
      destRegionY--;
      destY = max;
    }
    if (destY > max) {
      destRegionY++;
      destY = 0;
    }
    if (destRegionX < 0 || destRegionY < 0 ||\
        destRegionX >= rPerSide || destRegionY >= rPerSide) return false;
    Region* destReg = game->regions[game->coordsToSqIndex(destRegionX, destRegionY, rPerSide)];
    return moveTo(destReg, destX, destY);
  }
  /* Otherwise, the agent will still be inside the region; check if the space is
     occupied */
  int destIndex = region->game->coordsToSqIndex(destX, destY, region->size);
  RegionUnit* destUnit = &(region->regionUnits[destIndex]);
  if (destUnit->type != UNIT_TYPE_EMPTY) {
    if (destUnit->type == UNIT_TYPE_DOOR) {
      if (team == destUnit->team) {
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
  return moveTo(region, destX, destY);
}
