#ifndef PATHS_H
#define PATHS_H

#include <vector>

class Game;
struct MapUnit;

struct PathEdge {
  int from;
  int to;
  float weight;
};

class Paths {
  friend class Agent;
  friend class Spawner;
private:
  Game* game;
  std::vector<bool> visible;
public:
  Paths(Game*, MapUnit*);
};

#endif
