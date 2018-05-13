#include "context.h"

Context::Context() {
  type = GAME_CONTEXT_ZOOMED_OUT;
  outlinedRegionIndex = 0;
}

unsigned int Context::getOutlinedRegionIndex() {
  if (type != GAME_CONTEXT_ZOOMED_OUT) {
    throw "Incorrect game context! Shouldn't be requesting region outline...";
  }
  return outlinedRegionIndex;
}

void Context::setOutlinedRegionIndex(unsigned int i) {
  if (type != GAME_CONTEXT_ZOOMED_OUT) {
    throw "Incorrect game context! Shouldn't be setting region outline...";
  }
  outlinedRegionIndex = i;
}
