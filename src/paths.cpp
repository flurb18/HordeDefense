#include "paths.h"

#include <vector>

#include "game.h"

using std::vector;

struct RegionEdgeSegment {
  int from;
  int to;
  bool edgeInXDirection;
  bool edgeInPositiveDirection;
  int lo;
  int hi;
};

struct WeightedEdge {
  int from;
  int to;
  float weight;
};

struct Digraph {
  vector<bool> marked;
  vector<vector<int>> adj;
};

Paths::Paths(Game* g, MapUnit* startUnit): game(g) {
  //TODO
}
