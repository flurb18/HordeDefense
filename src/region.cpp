#include "region.h"

#include "agent.h"
#include "display.h"
#include "game.h"
#include "spawner.h"

#include <string>
#include <iostream>

/* Constructor fills the regionUnits vector with empty Region Units */
Region::Region(Game* g, int x_, int y_, int s, int ind): \
  Square(s), x(x_), y(y_), containsSpawner(false), game(g), outside(RegionUnit(this)) {
  index = ind;
  containsSpawner = false;
  regionUnits.reserve(size * size);
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      regionUnits.emplace_back(this, j, i);
    }
  }
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      int index = game->coordsToSqIndex(j, i, size);
      if (i == 0) regionUnits[index].up = &outside;
      else regionUnits[index].up = &regionUnits[index - size];
      if (i == size - 1) regionUnits[index].down = &outside;
      else regionUnits[index].down = &regionUnits[index + size];
      if (j == 0) regionUnits[index].left = &outside;
      else regionUnits[index].left = &regionUnits[index - 1];
      if (j == size - 1) regionUnits[index].right = &outside;
      else regionUnits[index].right = &regionUnits[index + 1];
    }
  }
}

/* Update the region one tick in game time, which means telling the spawner to
   update and any updatable units within the region to update */
void Region::update() {
  std::set<Agent*> agents;
  if (containsSpawner) {
    spawn->update();
  }
  for (RegionUnit u: regionUnits) {
    if (u.type == UNIT_TYPE_AGENT) agents.insert(u.agent);
  }
  for (Agent* a : agents) {
    a->update();
  }
}

/* Draw the outline of this region in zoomed out view */
void Region::drawOutline() {
  game->disp->drawRect(x, y, size, size);
}

/* Draw the Region Units in this region in zoomed out view */
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

/* Draw the Region units in this region in zoomed in view */
void Region::drawUnitsZoomedIn() {
  /* rPerSide = display size / region size, which is exactly the scale factor
  that we need to display units in zoomed-in view */
  int scale = game->rPerSide;
  for (unsigned int i = 0; i < regionUnits.size(); i++) {
    RegionUnit* u = &regionUnits[i];
    int drawX = u->regX * scale;
    int drawY = u->regY * scale;
    if (u->type != UNIT_TYPE_EMPTY) {
      if (u->team) {
        game->disp->setDrawColor(u->team);
      } else {
        game->disp->setDrawColorWhite();
      }
      /* All units have there team color as a background for their square;
         Agents have nothing more on top, spawners are 2x2 checkerboards, walls
         are plus signs, doors are lines indicating their direction */
      game->disp->drawRectFilled(drawX, drawY, scale, scale);
      switch(u->type) {
        case UNIT_TYPE_SPAWNER:
          game->disp->setDrawColorBlack();
          int h = scale/2;
          game->disp->drawRectFilled(drawX, drawY, h, h);
          game->disp->drawRectFilled(drawX + h, drawY + h, h, h);
          break;
      }
    } else if (i == game->currentUnitIndex) {
      /* Only draw the selection outline if this region unit is empty */
      game->disp->setDrawColorWhite();
      game->disp->drawRect(drawX, drawY, scale, scale);
    }
  }
  // Also handle the text in bottom left indicating which unit / type
  int x, y, h;
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
  game->disp->sizeText(s.c_str(), nullptr, &h);
  game->disp->drawText(s.c_str(), 0, game->disp->getSize() - h);
}
