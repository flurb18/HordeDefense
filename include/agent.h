#ifndef AGENT_H
#define AGENT_H

#include "region.h"

/* Agent extends RegionUnit, so they are stored within the region they occupy */
class Agent: public RegionUnit {
private:
  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
public:
  /* Constructor makes sure Agent's region unit type is correct */
  Agent(Region* r, const Team* t, int regX, int regY):\
        RegionUnit(r, t, UNIT_TYPE_AGENT, regX, regY) {};
  void update();
};

#endif
