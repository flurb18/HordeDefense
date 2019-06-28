#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <SDL2/SDL_rect.h>
#include <list>
#include <vector>

#include "mapunit.h"

const int OBJECTIVE_TYPE_BUILD = 2;
const int OBJECTIVE_TYPE_GOTO = 3;
const int OBJECTIVE_TYPE_DESTROY = 4;
const int OBJECTIVE_TYPE_SUPER = 5;

class Game;

struct Objective {
  int type, strength;
  std::vector<Objective> subObjectives;
  std::vector<Objective>::iterator iter;
  Game* game;
  SDL_Rect region;
  const Team* team;
  Objective(int, int, Game*, SDL_Rect, const Team*);
  MapUnit::iterator getIterator();
  void addSubObjective(Objective);
  bool isDone();
  void update();
};

#endif
