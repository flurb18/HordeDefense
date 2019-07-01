#include "agent.h"

#include <vector>
#include <random>

#include "game.h"
#include "mapunit.h"

Agent::Agent(Game* g, MapUnit* m, const Team* t):
              team(t), game(g), unit(m) {
  dx = 0;
  dy = 0;
  unit->agent = this;
  unit->type = UNIT_TYPE_AGENT;
}

/* Update agent based on objective */
bool Agent::update() {
  /* Only update once per tick */
  if (lastUpdatedTimestamp == game->t) return true;
  MapUnit* neighbors[4] = {unit->left, unit->right, unit->up, unit->down};
  // Handle objectives at the neighbors of the agent
  for (MapUnit* m: neighbors) {
    if (m->objective != nullptr) {
      switch (m->objective->type) {
        case OBJECTIVE_TYPE_BUILD_WALL:
          if (moveTo(m)) {
            m->type = UNIT_TYPE_WALL;
            m->agent = nullptr;
            m->team = nullptr;
            return false;
          }
          break;
        case OBJECTIVE_TYPE_DESTROY:
          if (m->type == UNIT_TYPE_WALL) m->type = UNIT_TYPE_EMPTY;
          break;
      }
    }
  }
  lastUpdatedTimestamp = game->t;
  int incrementOptions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
  std::vector<int> options;
  // Code for choosing a scent at random (weighted)
  double scents[4] = {\
    unit->left->scent[team->teamNum], \
    unit->right->scent[team->teamNum], \
    unit->up->scent[team->teamNum], \
    unit->down->scent[team->teamNum]};
    double total = 0.0;
    for (int i = 0; i < 4; i++) {
      total += scents[i];
    }
    int choice = rand() % 4;
    if (total > 0.0) {
      double rnd = ((double)rand() / (double)RAND_MAX) * total;
      for(int i=0; i<4; i++) {
        if(rnd < scents[i]) {
          choice = i;
          break;
        }
        rnd -= scents[i];
      }
    }
    dx = incrementOptions[choice][0];
    dy = incrementOptions[choice][1];
    if (!move()) {
      dx = -dx;
      dy = -dy;
    }
    return true;
  }

bool Agent::moveTo(MapUnit* destUnit) {
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

/* Attempts to move the agent based on dx and dy; returns true if successful,
   false otherwise */
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
