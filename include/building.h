#ifndef BUILDING_H
#define BUILDING_H

const int BUILDING_TYPE_WALL = 1;
const int BUILDING_TYPE_DOOR = 2;
const int BUILDING_TYPE_SPAWNER = 3;

struct Building {
  int hp;
  int numAgentsToBuild;
  int numAgentsAdded;
  int type;
}

#endif
