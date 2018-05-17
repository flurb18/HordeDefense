#ifndef REGION_H
#define REGION_H

#include <vector>

#include "square.h"

/* Forward declarations */
class Agent;
class Region;
class Game;
class Spawner;
struct Team;

const int UNIT_TYPE_EMPTY = 0;
const int UNIT_TYPE_AGENT = 1;
const int UNIT_TYPE_SPAWNER = 2;
const int UNIT_TYPE_WALL = 3;
const int UNIT_TYPE_DOOR = 4;
const int UNIT_TYPE_OUTSIDE = 5;

/* Each region is comprised of regionSize x regionSize RegionUnits, and one more
to represent the "outside" (a dummy, if a unit passes into the outside it needs
to be moved to another region)*/
class RegionUnit {
public:
  unsigned int regX, regY;
  int type;
  const Team* team;
  Agent* agent;
  Region* region;
  RegionUnit* left;
  RegionUnit* right;
  RegionUnit* up;
  RegionUnit* down;
  RegionUnit(Region* r): type(UNIT_TYPE_OUTSIDE), region(r) {};
  RegionUnit(Region* r, int x_, int y_):\
             regX(x_), regY(y_), type(UNIT_TYPE_EMPTY), region(r) {};
  RegionUnit(Region* r, const Team* t, int type_, int x_, int y_):\
             regX(x_), regY(y_), type(type_), team(t), region(r) {};
  /* Method to be overriden by, say, Agents */
  virtual void update() {};
};

class Region: public Square {
  /* Let Spawner modify private fields of this class (i.e. regionUnits,
  containsSpawner) */
  friend class Agent;
  friend class Paths;
  friend class Spawner;
private:
  int x, y;
  bool containsSpawner;
  Game* game;
  Spawner* spawn;
  std::vector<RegionUnit> regionUnits;
  std::vector<Agent*> agents;
  RegionUnit outside;
public:
  Region(Game*, int, int, int);
  void drawUnits();
  void drawUnitsZoomedIn();
  void drawOutline();
  void update();
};

#endif
