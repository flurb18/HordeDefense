#include "mapunit.h"

#include "agent.h"
#include "game.h"

MapUnit::MapUnit(Game* g): type(UNIT_TYPE_OUTSIDE), scent(0.0), prevScent(0.0), diffusion(0.1), game(g) {}

MapUnit::MapUnit(Game* g, int x_, int y_): \
             x(x_), y(y_), type(UNIT_TYPE_EMPTY), scent(0.0), prevScent(0.0), diffusion(0.1), game(g)  {
  index = game->coordsToSqIndex(x, y, game->getSize());
}

MapUnit::MapUnit(Game* g, const Team* t, int type_, int x_, int y_): \
             x(x_), y(y_), type(type_), scent(0.0), prevScent(0.0), diffusion(0.1), game(g), team(t) {
  index = game->coordsToSqIndex(x, y, game->getSize());
}

void MapUnit::iterator::next() {
  current = current->right;
  j++;
  if (current->type == UNIT_TYPE_OUTSIDE || j == w) {
    firstInRow = firstInRow->down;
    current = firstInRow;
    j = 0;
    i++;
    if (current->type == UNIT_TYPE_OUTSIDE || i == h) {
      hasNextUnit = false;
    }
  }
}

void MapUnit::update() {
  prevScent = scent;
  if (type == UNIT_TYPE_EMPTY || type == UNIT_TYPE_DOOR) {
    scent = diffusion*(left->prevScent + up->prevScent + right->scent + down->scent);
  } else scent = 0.0;
}
