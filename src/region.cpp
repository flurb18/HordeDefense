#include "region.h"

#include "display.h"
#include "agent.h"
#include "spawner.h"
#include "square.h"
#include "team.h"

void Region::drawOutline() {
  disp->drawRect(x, y, size, size);
}

void Region::drawAgents() {
  // drawAgents also draws the spawner, if this region contains it
  if (containsSpawner) {
    int sx = x + this->getRadius() - spawn->getRadius();
    int sy = y + this->getRadius() - spawn->getRadius();
    disp->setDrawColor(spawn->team->R, spawn->team->G, spawn->team->B);
    disp->drawRectFilled(sx, sy, spawn->getSize(), spawn->getSize());
  }
  //TODO
}

void Region::drawAgentsZoomedIn() {
  if (containsSpawner) {
    int spawnScaledSize = spawn->getSize() * disp->getSize() / this->getSize();
    int start = disp->getRadius() - (spawnScaledSize / 2);
    disp->setDrawColor(spawn->team->R, spawn->team->G, spawn->team->B);
    disp->drawRectFilled(start, start, spawnScaledSize, spawnScaledSize);
  }
}

void Region::update() {
  if (containsSpawner) {
    spawn->spawnAgent();
  }
}
