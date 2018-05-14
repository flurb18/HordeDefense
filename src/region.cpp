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
    int start = disp->getRadius() - spawn->getRadius();
    disp->setDrawColor(spawn->team->R, spawn->team->G, spawn->team->B);
    disp->drawRectFilled(start, start, spawn->getSize(), spawn->getSize());
  }
  //TODO
}

void Region::update() {
  if (containsSpawner) {
    spawn->spawnAgent();
  }
}
