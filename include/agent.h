#ifndef AGENT_H
#define AGENT_H

#include "region.h"

class Agent: public RegionUnit {
private:
  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
public:
  Agent(Region* r, const Team* t, int regX, int regY):\
        RegionUnit(r, t, UNIT_TYPE_AGENT, regX, regY) {};
  void update();
};

#endif
