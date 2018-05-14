#include "region.h"

#include "display.h"
#include "agent.h"
#include "spawner.h"
#include "square.h"

Region::Region(int x, int y, int s, Display* d) : Square(s) {
  this->x = x;
  this->y = y;
  size = s;
  disp = d;
  containsSpawner = false;
}

void Region::drawOutline() {
  disp->drawRect(x, y, size, size);
}

void Region::drawAgents() {
  // drawAgents also draws the spawner, if this region contains it
  if (containsSpawner) {
    int start = disp->getRadius() - spawn->getRadius();
    disp->drawRectFilled(start, start, spawn->getSize(), spawn->getSize());
  }
  //TODO
}
