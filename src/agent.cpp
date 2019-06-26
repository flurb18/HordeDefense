#include "agent.h"

#include <cstdlib>
#include <vector>

#include "game.h"
#include "mapunit.h"

Agent::Agent(Game* g, MapUnit* m, const Team* t):
              team(t), game(g), unit(m) {
  objective = OBJECTIVE_TYPE_FOLLOW_SCENT;
  dx = 0;
  dy = 0;
  unit->agent = this;
  unit->type = UNIT_TYPE_AGENT;
}

/* Update agent based on objective */
void Agent::update() {
  /* Only update once per tick */
  if (lastUpdatedTimestamp == game->t) return;
  lastUpdatedTimestamp = game->t;
  int incrementOptions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
  std::vector<int> options;
  switch(objective) {
    case OBJECTIVE_TYPE_EXPLORE:
      if (unit->left->type != UNIT_TYPE_OUTSIDE) {
        options.push_back(0);
      }
      if (unit->right->type != UNIT_TYPE_OUTSIDE) {
        options.push_back(1);
      }
      if (unit->up->type != UNIT_TYPE_OUTSIDE) {
        options.push_back(2);
      }
      if (unit->down->type != UNIT_TYPE_OUTSIDE) {
        options.push_back(3);
      }
      int exploreSelection;
      if (options.size() != 0) exploreSelection = options[rand() % options.size()];
      else exploreSelection = rand() % 4;
      dx = incrementOptions[exploreSelection][0];
      dy = incrementOptions[exploreSelection][1];
      if (!move()) {
        dx = -dx;
        dy = -dy;
      }
      break;
    case OBJECTIVE_TYPE_CHARGE:
      if (!move()) {
        dx = 0;
        dy = 0;
      }
      break;
    case OBJECTIVE_TYPE_FOLLOW_SCENT:
      double maxAvailableScent = 0.0;
      // If none of the adjacent squares have any scent, try a random one
      int choice = rand() % 4;
      double scents[4] = {unit->left->scent[team->teamNum], \
                          unit->right->scent[team->teamNum], \
                          unit->up->scent[team->teamNum], \
                          unit->down->scent[team->teamNum]};
      for (int i = 0; i < 4; i++) {
        if (scents[i] > maxAvailableScent) maxAvailableScent = scents[i];
      }
      for (int i = 0; i < 4; i++) {
        if (scents[i] == maxAvailableScent) options.push_back(i);
      }
      choice = options[rand() % options.size()];
      dx = incrementOptions[choice][0];
      dy = incrementOptions[choice][1];
      if (!move()) {
        dx = -dx;
        dy = -dy;
      }
      break;
  }
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
