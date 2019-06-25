#include "paths.h"

#include "game.h"

Paths::Paths(Game* g, MapUnit* startUnit): game(g) {
  visible.reserve(g->getSize() * g->getSize());
  scent.reserve(g->getSize() * g->getSize());
  for (unsigned int i = 0; i < visible.capacity(); i++) {
    visible[i] = false;
    scent[i] = 0.0;
  }
}

void Paths::addGoal(MapUnit* goal, double desirability) {
  scent[goal->index] += desirability;
}

/* Do 1 tick of the diffusion */
void Paths::update() {
  /*
  std::vector<double> newScents;
  newScents.reserve(scent.capacity());
  for (unsigned int i = 0; i < newScents.capacity(); i++) {
    newScents[i] = 0.0;
    //if (game->mapUnits[i]->type == UNIT_TYPE_EMPTY) {
      if (game->mapUnits[i]->left->type == UNIT_TYPE_EMPTY)
        newScents[i] += DIFFUSION_CONSTANT * scent[game->mapUnits[i]->left->index];
      if (game->mapUnits[i]->right->type == UNIT_TYPE_EMPTY)
        newScents[i] += DIFFUSION_CONSTANT * scent[game->mapUnits[i]->right->index];
      if (game->mapUnits[i]->up->type == UNIT_TYPE_EMPTY)
        newScents[i] += DIFFUSION_CONSTANT * scent[game->mapUnits[i]->up->index];
      if (game->mapUnits[i]->down->type == UNIT_TYPE_EMPTY)
        newScents[i] += DIFFUSION_CONSTANT * scent[game->mapUnits[i]->down->index];
    //}
  }
  scent = newScents;*/
}
