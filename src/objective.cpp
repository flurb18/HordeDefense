#include "objective.h"

#include "game.h"

#include <iostream>

Objective::Objective(int t, int s, Game* g, SDL_Rect r, const Team* t_): \
  type(t), strength(s), game(g), region(r), team(t_) {
  switch(type) {
    case OBJECTIVE_TYPE_BUILD_WALL:
      if (region.w > 1 && region.h > 1) {
        type = OBJECTIVE_TYPE_SUPER;
        if (region.h >= region.w) {
          for (int i = 0; i < region.h; i++) {
            SDL_Rect slice = {region.x, region.y + i, region.w, 1};
            subObjectives.emplace_back(OBJECTIVE_TYPE_BUILD_WALL, strength, game, slice, team);
          }
        } else {
          for (int j = 0; j < region.w; j++) {
            SDL_Rect slice = {region.x + j, region.y, 1, region.h};
            subObjectives.emplace_back(OBJECTIVE_TYPE_BUILD_WALL, strength, game, slice, team);
          }
        }
        iter = subObjectives.begin();
      }
      break;
  }
}

MapUnit::iterator Objective::getIterator() {
  MapUnit* first = game->mapUnits[game->coordsToSqIndex(region.x, region.y, game->getSize())];
  return first->getIterator(region.w, region.h);
}

bool Objective::isDone() {
  switch(type) {
    case OBJECTIVE_TYPE_BUILD_WALL:
      for (MapUnit::iterator m = getIterator(); m.hasNext(); m++) {
        if (m->type != UNIT_TYPE_WALL) return false;
      }
      return true;
    case OBJECTIVE_TYPE_SUPER:
      if (iter != subObjectives.end()) return false;
      else return true;
    default:
      return false;
  }
}

void Objective::addSubObjective(Objective o){
  subObjectives.push_back(o);
}

void Objective::update() {
  switch(type) {
    case OBJECTIVE_TYPE_SUPER:
      if (iter != subObjectives.end()) {
        iter->update();
        if (iter->isDone()){
          iter++;
        }
      }
      break;
    case OBJECTIVE_TYPE_BUILD_WALL:
      for (MapUnit::iterator m = getIterator(); m.hasNext(); m++) {
        m->objective = this;
        if (m->type == UNIT_TYPE_EMPTY) m->scent[team->teamNum] = strength;
      }
      break;
  }
}
