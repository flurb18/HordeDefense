#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

class Display {
  int width, height;
  SDL_Window* window;
  SDL_Renderer* render;
public:
  Display(int, int);
  void fillBlack();
  void end();
  void update();
  void wait(int);
};

#endif
