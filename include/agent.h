#ifndef AGENT_H
#define AGENT_H

#include "region.h"

/* Forward Declarations */
class Paths;
class RegionUnit;
struct Team;

const int OBJECTIVE_TYPE_WAIT = 0;
const int OBJECTIVE_TYPE_EXPLORE = 1;
const int OBJECTIVE_TYPE_GOTO = 2;
const int OBJECTIVE_TYPE_BUILD = 3;
const int OBJECTIVE_TYPE_RUN = 4;

/* Agent extends RegionUnit, so they are stored within the region they occupy */
class Agent {
  friend class Spawner;
private:
  int index, objective, dx, dy;
  Region* region;
  RegionUnit* rUnit;
  Paths* paths;
  const Team* team;
  bool move();
public:
  /* Constructor makes sure Agent's region unit type is correct */
  Agent(Paths* p, RegionUnit* r, const Team* t);
  virtual void update();
};

#endif
