#include "mapunit.h"

#include "agent.h"
#include "game.h"

MapUnit::MapUnit(Game* g): type(UNIT_TYPE_OUTSIDE), game(g) {
  initializeScents();
}

MapUnit::MapUnit(Game* g, int x_, int y_): \
             x(x_), y(y_), type(UNIT_TYPE_EMPTY), game(g)  {
  index = y * game->getSize() + x;
  initializeScents();
}

MapUnit::MapUnit(Game* g, const Team* t, int type_, int x_, int y_): \
             x(x_), y(y_), type(type_), game(g), team(t) {
  index = y * game->getSize() + x;
  initializeScents();
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

void MapUnit::initializeScents() {
  for (int i = 0; i < NUM_OF_TEAMS; i++) {
    scent[i] = 0.0;
    prevScent[i] = 0.0;
    diffusion[i] = 0.2;
  }
}

void MapUnit::update() {
  for (int i = 0; i < NUM_OF_TEAMS; i++) {
    prevScent[i] = scent[i];
    if (type == UNIT_TYPE_EMPTY || (type == UNIT_TYPE_DOOR && team->teamNum == i)) {
      // Left and up have already been iterated through while updating
      scent[i] = diffusion[i]*(left->prevScent[i] + up->prevScent[i] + right->scent[i] + down->scent[i]);
    } else scent[i] = 0.0;
  }
}
