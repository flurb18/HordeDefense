#include "spawner.h"

#include "region.h"
#include "square.h"

Spawner::Spawner(Region* spawnerRegion, unsigned int s) : Square(s) {
  reg = spawnerRegion;
  reg->containsSpawner = true;
  reg->spawn = this;
}
