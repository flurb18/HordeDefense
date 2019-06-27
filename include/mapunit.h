#ifndef MAPUNIT_H
#define MAPUNIT_H

#include "teams.h"

const int UNIT_TYPE_EMPTY = 0;
const int UNIT_TYPE_AGENT = 1;
const int UNIT_TYPE_SPAWNER = 2;
const int UNIT_TYPE_WALL = 3;
const int UNIT_TYPE_DOOR = 4;
const int UNIT_TYPE_OUTSIDE = 5;

class Agent;
class Game;
class Objective;

struct MapUnit {
  /* An iterator for traversing through a predefined rectangle of mapunits */
  class iterator {
  private:
    bool hasNextUnit;
    MapUnit* current;
    MapUnit* firstInRow;
    void next();
  public:
    int j, i, w, h;
    iterator(MapUnit* first, int w_, int h_): hasNextUnit(true), \
    current(first), firstInRow(first), j(0), i(0), w(w_), h(h_){};
    iterator operator++() {iterator it = *this; next(); return it;};
    iterator operator++(int junk) {next(); return *this;};
    MapUnit& operator*() {return *current;};
    MapUnit* operator->() {return current;};
    bool hasNext() {return hasNextUnit;};
  };
  unsigned int x, y, index;
  int type;
  double scent[NUM_OF_TEAMS];
  double prevScent[NUM_OF_TEAMS];
  // Should be less than 0.25
  double diffusion[NUM_OF_TEAMS];
  Agent* agent;
  Game* game;
  Objective* objective;
  const Team* team;
  MapUnit* up;
  MapUnit* down;
  MapUnit* left;
  MapUnit* right;
  MapUnit(Game*);
  MapUnit(Game*, int, int);
  MapUnit(Game*, const Team*, int, int, int);
  void initializeScents();
  /* Create an iterator through a rectangle of mapunits starting with this one
     at the top left */
  iterator getIterator(int w, int h) {return iterator(this, w, h);};
  void update();
};

#endif
