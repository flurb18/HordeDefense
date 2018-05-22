#include "mapunit.h"

#include "agent.h"
#include "game.h"

MapUnit::MapUnit(Game* g): type(UNIT_TYPE_OUTSIDE), game(g) {}

MapUnit::MapUnit(Game* g, int x_, int y_): \
             x(x_), y(y_), type(UNIT_TYPE_EMPTY), game(g)  {
  index = game->coordsToSqIndex(x, y, game->getSize());
}

MapUnit::MapUnit(Game* g, const Team* t, int type_, int x_, int y_): \
             x(x_), y(y_), type(type_), game(g), team(t) {
  index = game->coordsToSqIndex(x, y, game->getSize());
}

MapUnit::~MapUnit() {
  if (agent) delete agent;
}
