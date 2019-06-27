#include "objective.h"

#include "game.h"

Objective::Objective(int t, int s, Game* g, SDL_Rect r, const Team* t_): \
  type(t), strength(s), game(g), region(r), team(t_) {
  for (MapUnit::iterator m = getIterator(); m.hasNext(); m++) {
    m->objective = this;
  }
}

MapUnit::iterator Objective::getIterator() {
  MapUnit* first = game->mapUnits[game->coordsToSqIndex(region.x, region.y, game->getSize())];
  return first->getIterator(region.w, region.h);
}

void Objective::update() {
  for (MapUnit::iterator m = getIterator(); m.hasNext(); m++) {
    if (m->type == UNIT_TYPE_EMPTY) m->scent[team->teamNum] = strength;
  }
}
