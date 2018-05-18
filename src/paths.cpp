#include "paths.h"

#include <vector>

#include "game.h"
#include "region.h"

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

Paths::Paths(Game* g, Region* startRegion): game(g) {
  knownRegions.reserve(game->rPerSide * game->rPerSide);
  knownRegions.push_back(startRegion);
}

void Paths::addRegion(Region* r) {

}
