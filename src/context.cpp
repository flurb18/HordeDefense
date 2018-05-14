#include "context.h"

Context::Context() {
  type = GAME_CONTEXT_ZOOMED_OUT;
  currentRegionIndex = 0;
  paused = true;
}

unsigned int Context::getCurrentRegionIndex() {
  return currentRegionIndex;
}

void Context::setCurrentRegionIndex(unsigned int i) {
  if (type != GAME_CONTEXT_ZOOMED_OUT) {
    throw "Incorrect game context! Shouldn't be setting current region...";
  }
  currentRegionIndex = i;
}
