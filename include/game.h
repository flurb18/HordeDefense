#ifndef GAME_H
#define GAME_H

#include "display.h"

const int GAME_CONTEXT_ZOOMED_OUT = 2;
const int GAME_CONTEXT_EXIT = 10;

class Game {
  Display* disp;
public:
  int context;
  bool paused;
  Game(Display*);
};


#endif
