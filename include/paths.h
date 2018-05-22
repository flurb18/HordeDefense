#ifndef PATHS_H
#define PATHS_H

#include <vector>

class Game;
struct MapUnit;

class Paths {
private:
  Game* game;
  std::vector<bool> visible;
public:
  Paths(Game*, MapUnit*);
};

#endif
