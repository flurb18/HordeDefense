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
    disp->setDrawColor(spawn->team);
    disp->drawRectFilled(sx, sy, spawn->getSize(), spawn->getSize());
  }
  for (Agent* a : agents) {
    disp->setDrawColor(a->team);
    disp->drawPixel(a->x, a->y);
  }
}

void Region::drawAgentsZoomedIn() {
  // pxScale is also equal to regionsPerSide, maybe put that in game context
  int pxScale = disp->getSize() / getSize();
  if (containsSpawner) {
    int spawnScaledSize = spawn->getSize() * pxScale;
    int start = disp->getRadius() - (spawnScaledSize / 2);
    disp->setDrawColor(spawn->team);
    disp->drawRectFilled(start, start, spawnScaledSize, spawnScaledSize);
  }
  for (Agent* a : agents) {
    disp->setDrawColor(a->team);
    int ax = (a->x % size) * pxScale;
    int ay = (a->y % size) * pxScale;
    disp->drawRectFilled(ax, ay, pxScale, pxScale);
  }
}

void Region::update() {
  if (containsSpawner) {
    spawn->update();
  }
}

Region::~Region() {
  for (unsigned int i = 0; i < agents.size(); i++) {
    delete agents[i];
  }
  agents.clear();
}
