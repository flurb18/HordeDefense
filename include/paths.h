#ifndef PATHS_H
#define PATHS_H

#include <vector>

class Game;
struct MapUnit;

const double DIFFUSION_CONSTANT = 0.25;

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
  std::vector<double> scent;
public:
  Paths(Game*, MapUnit*);
  void addGoal(MapUnit*, double);
  void update();
};

#endif
