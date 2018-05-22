#ifndef PATHS_H
#define PATHS_H

#include <vector>

class Game;
class Region;
struct MapUnit;

class Paths {
private:
  Game* game;
  std::vector<bool> visible;
  void addRegion(Region*);
public:
  Paths(Game*, MapUnit*);
};

#endif
