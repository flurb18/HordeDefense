#include "spawner.h"

#include "agent.h"
#include "region.h"
#include "square.h"
#include "team.h"

Spawner::Spawner(Context* c, Region* reg, const Team* t, unsigned int s) : \
                Square(s), gameContext(c), spawnerRegion(reg), team(t) {
  reg->containsSpawner = true;
  reg->spawn = this;
}

void Spawner::spawnAgent() {

}
