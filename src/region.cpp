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
    for (unsigned int j = 0; j < size; j++) {
      regionUnits.emplace_back(this, j, i);
    }
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
        game->disp->setDrawColorWhite();
      }
      game->disp->drawPixel(x + u.regX, y + u.regY);
    }
  }
}

void Region::drawUnitsZoomedIn() {
  /* rPerSide = display size / region size, which is exactly the scale factor
  that we need to display units in zoomed-in view*/
  int scale = game->rPerSide;
  for (unsigned int i = 0; i < regionUnits.size(); i++) {
    RegionUnit* u = &regionUnits[i];
      if (u->type != UNIT_TYPE_EMPTY) {
        if (u->team) {
          game->disp->setDrawColor(u->team);
        } else {
          game->disp->setDrawColorWhite();
        }
        int drawX = u->regX * scale;
        int drawY = u->regY * scale;
        game->disp->drawRectFilled(drawX, drawY, scale, scale);
        switch(u->type) {
          // If spawner, draw 2x2 checkerboard pattern
          case UNIT_TYPE_SPAWNER:
            game->disp->setDrawColorBlack();
            int h = scale/2;
            game->disp->drawRectFilled(drawX, drawY, h, h);
            game->disp->drawRectFilled(drawX + h, drawY + h, h, h);
            break;
        }
      }
  }
  // Also handle the text in bottom left indicating which unit / type
  int x, y, w, h;
  game->indexToSqCoords(game->currentUnitIndex, size, &x, &y);
  std::string s = std::to_string(x) + ", " + std::to_string(y);
  switch(regionUnits[game->currentUnitIndex].type) {
    case UNIT_TYPE_AGENT:
      s += " Type: Agent";
      break;
    case UNIT_TYPE_SPAWNER:
      s += " Type: Spawner";
      break;
    case UNIT_TYPE_WALL:
      s += " Type: Wall";
      break;
    case UNIT_TYPE_DOOR:
      s += " Type: Door";
      break;
  }
  game->disp->sizeText(s.c_str(), &w, &h);
  game->disp->drawText(s.c_str(), 0, game->disp->getSize() - h);
}

void Region::update() {
  if (containsSpawner) {
    spawn->update();
  }
}
