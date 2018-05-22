#include "paths.h"

#include <vector>

#include "game.h"

struct WeightedEdge {
  int from;
  int to;
  float weight;
};

struct Digraph {
  std::vector<bool> marked;
  std::vector<std::vector<int>> adj;
};

Paths::Paths(Game* g, MapUnit* startUnit): game(g) {
  //TODO
}
