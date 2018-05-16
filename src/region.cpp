#include "region.h"

#include "display.h"
#include "game.h"
#include "agent.h"
#include "spawner.h"
#include "square.h"

#include <iostream>

Region::Region(Game* g, int x_, int y_, int s): \
  Square(s), x(x_), y(y_), containsSpawner(false), game(g), outside(RegionUnit(this)) {
    containsSpawner = false;
  regionUnits.reserve(size * size);
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++)
    regionUnits.emplace_back(this, j, i);
  }
}

void Region::drawOutline() {
  game->disp->drawRect(x, y, size, size);
}

void Region::drawUnits() {
  for (RegionUnit u: regionUnits) {
    if (u.type != UNIT_TYPE_EMPTY) {
      if (u.team) {
        game->disp->setDrawColor(u.team);
      } else {
        game->disp->setDrawColor(255, 255, 255);
      }
      game->disp->drawPixel(x + u.regX, y + u.regY);
    }
  }
}

void Region::drawUnitsZoomedIn() {
  double scale = game->disp->getSize() / size;
  for (RegionUnit u : regionUnits) {
      if (u.type != UNIT_TYPE_EMPTY) {
        if (u.team) {
          game->disp->setDrawColor(u.team);
        } else {
          game->disp->setDrawColor(u.team);
        }
        game->disp->drawRectFilled(u.regX * scale, u.regY * scale, scale, scale);
      }
  }
}

void Region::update() {
  if (containsSpawner) {
    spawn->update();
  }
}
