#ifndef MAPUNIT_H
#define MAPUNIT_H

const int UNIT_TYPE_EMPTY = 0;
const int UNIT_TYPE_AGENT = 1;
const int UNIT_TYPE_SPAWNER = 2;
const int UNIT_TYPE_WALL = 3;
const int UNIT_TYPE_DOOR = 4;
const int UNIT_TYPE_OUTSIDE = 5;

class Agent;
class Game;
struct Team;

class MapUnit {
  friend class Spawner;
  friend class Game;
private:
  unsigned int x, y, index;
public:
  int type;
  bool movedTo;
  Agent* agent;
  Game* game;
  const Team* team;
  MapUnit* up;
  MapUnit* down;
  MapUnit* left;
  MapUnit* right;
  MapUnit(Game*);
  MapUnit(Game*, int, int);
  MapUnit(Game*, const Team*, int, int, int);
  ~MapUnit();
};

#endif
