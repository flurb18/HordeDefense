#include "paths.h"

#include <vector>

#include "game.h"
#include "region.h"


Paths::Paths(Game* g, Region* startRegion): game(g) {
  knownRegions.reserve(game->rPerSide * game->rPerSide);
  knownRegions.push_back(startRegion);
}

void Paths::addRegion(Region* r) {

}
