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
    int sx = x + getRadius() - spawn->getRadius();
    int sy = y + getRadius() - spawn->getRadius();
    disp->setDrawColor(spawn->team->R, spawn->team->G, spawn->team->B);
    disp->drawRectFilled(sx, sy, spawn->getSize(), spawn->getSize());
  }
  for (Agent* a : agents) {
    disp->setDrawColor(a->team->R, a->team->G, a->team->B);
    disp->drawPixel(a->x, a->y);
  }
}

void Region::drawAgentsZoomedIn() {
  if (containsSpawner) {
    int spawnScaledSize = spawn->getSize() * disp->getSize() / getSize();
    int start = disp->getRadius() - (spawnScaledSize / 2);
    disp->setDrawColor(spawn->team->R, spawn->team->G, spawn->team->B);
    disp->drawRectFilled(start, start, spawnScaledSize, spawnScaledSize);
  }
  for (Agent* a : agents) {
    disp->setDrawColor(a->team->R, a->team->G, a->team->B);
    int agentScaledSize = disp->getSize() / getSize();
    int ax = (a->x % size) * agentScaledSize;
    int ay = (a->y % size) * agentScaledSize;
    disp->drawRectFilled(ax, ay, agentScaledSize, agentScaledSize);
  }
}

void Region::update() {
  if (containsSpawner) {
    spawn->update();
  }
}
