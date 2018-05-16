#ifndef REGION_H
#define REGION_H

#include <vector>

#include "square.h"

// Forward declarations
class Region;
class Game;
class Spawner;
struct Team;

const int UNIT_TYPE_EMPTY = 0;
const int UNIT_TYPE_AGENT = 1;
const int UNIT_TYPE_SPAWNER = 2;
const int UNIT_TYPE_WALL = 3;
const int UNIT_TYPE_DOOR = 4;
const int UNIT_TYPE_OUTSIDE = 10;

struct RegionUnit {
  unsigned int regX, regY;
  int type;
  const Team* team;
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
};

class Region: public Square {
  friend class Spawner;
  int x, y;
  bool containsSpawner;
  Game* game;
  Spawner* spawn;
  std::vector<RegionUnit> regionUnits;
  RegionUnit outside;
public:
  Region(Game*, int, int, int);
  void drawUnits();
  void drawUnitsZoomedIn();
  void drawOutline();
  void update();
};

#endif
