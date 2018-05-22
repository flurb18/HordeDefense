#include "agent.h"

#include <algorithm>
#include <iostream>
#include <set>

#include "game.h"
#include "mapunit.h"

Agent::Agent(Game* g, Paths* p, MapUnit* m, const Team* t):
             game(g), unit(m), paths(p), team(t) {
  paths = p;
  objective = OBJECTIVE_TYPE_EXPLORE;
  dx = 0;
  dy = 0;
  unit->agent = this;
  unit->type = UNIT_TYPE_AGENT;
}

/* Update agent based on objective */
void Agent::update() {
  if (lastUpdatedTimestamp == game->t) return;
  lastUpdatedTimestamp = game->t;
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

bool Agent::moveTo(MapUnit* destUnit) {
  if (destUnit->type == UNIT_TYPE_OUTSIDE) return false;
  if (destUnit->type != UNIT_TYPE_EMPTY) return false;
  destUnit->type = UNIT_TYPE_AGENT;
  destUnit->agent = this;
  destUnit->team = team;
  unit->type = UNIT_TYPE_EMPTY;
  unit->agent = nullptr;
  unit->team = nullptr;
  unit = destUnit;
  return true;
}

/* Attempts to move the agent; returns true if successful, false otherwise */
bool Agent::move() {
  if (dx == 0 && dy == 0) return false;
  MapUnit* destUnit = unit;
  if (dx > 0) destUnit = unit->right;
  if (dx < 0) destUnit = unit->left;
  if (dy > 0) destUnit = unit->down;
  if (dy < 0) destUnit = unit->up;
  if (destUnit->type != UNIT_TYPE_EMPTY) {
    /* If attempting to move into a door, check if the door's team equals this
       agent's team; if it does, try to move into the square past the door based
       on the motion of the agent */
    if (destUnit->type == UNIT_TYPE_DOOR) {
      if (team == destUnit->team) {
        if (dx > 0) destUnit = destUnit->right;
        if (dx < 0) destUnit = destUnit->left;
        if (dy > 0) destUnit = destUnit->down;
        if (dy < 0) destUnit = destUnit->up;
        return moveTo(destUnit);
      }
    }
    return false;
  }
  return moveTo(destUnit);
}

Agent::~Agent() {}
