#include "region.h"

#include "display.h"
#include "agent.h"

Region::Region(int x, int y, int s, Display* d) {
  this->x = x;
  this->y = y;
  size = s;
  rect = {x, y, size, size};
  disp = d;
}

void Region::drawOutline() {
  disp->drawRect(&rect);
}

void Region::addAgent(int x, int y) {
  agents.emplace_back(new Agent(this));
}
