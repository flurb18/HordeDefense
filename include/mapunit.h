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
  unsigned int x, y;
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
  MapUnit(Game* g): type(UNIT_TYPE_OUTSIDE), game(g) {};
  MapUnit(Game* g, int x_, int y_): \
             x(x_), y(y_), type(UNIT_TYPE_EMPTY), game(g)  {};
  MapUnit(Game* g, const Team* t, int type_, int x_, int y_): \
             x(x_), y(y_), type(type_), game(g), team(t) {};
};

#endif
