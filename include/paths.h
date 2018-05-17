#ifndef PATHS_H
#define PATHS_H

#include <vector>

class Game;
class Region;

struct WeightedEdge {
  int from;
  int to;
  float weight;
};

class Paths {
private:
  Game* game;
  std::vector<Region *> knownRegions;
  std::vector<bool> visible;
  void addRegion(Region*);
public:
  Paths(Game*, Region*);
};

#endif
