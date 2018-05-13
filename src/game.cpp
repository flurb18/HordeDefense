#include "game.h"

Game::Game(Display* d) {
  context = GAME_CONTEXT_ZOOMED_OUT;
  paused = false;
  disp = d;
}
