#include "paths.h"

#include "game.h"

Paths::Paths(Game* g, MapUnit* startUnit): game(g) {
  visible.reserve(g->getSize() * g->getSize());
  for (unsigned int i = 0; i < visible.capacity(); i++) visible[i] = false;
}
