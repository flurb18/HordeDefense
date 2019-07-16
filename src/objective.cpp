#include "objective.h"

#include "game.h"

Objective::Objective(int t, int s, Game* g, SDL_Rect r, const Team* t_): \
  type(t), strength(s), game(g), region(r), team(t_) {
  done = false;
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
      } else {
        numUnitsRequired = region.w * region.h;
        numUnitsDone = 0;
      }
      break;
  }
}

MapUnit::iterator Objective::getIterator() {
  MapUnit* first = game->mapUnitAt(region.x, region.y);
  return first->getIterator(region.w, region.h);
}

bool Objective::isDone() {
  switch(type) {
    case OBJECTIVE_TYPE_BUILD_WALL:
      return done;
    case OBJECTIVE_TYPE_SUPER:
      if (iter != subObjectives.end()) return false;
      else return true;
    default:
      return false;
  }
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
      done = true;
      for (MapUnit::iterator m = getIterator(); m.hasNext(); m++) {
        if (m->type == UNIT_TYPE_EMPTY) {
          done = false;
          m->objective = this;
          m->scent[team->teamNum] = strength;
        }
      }
      break;
  }
}
