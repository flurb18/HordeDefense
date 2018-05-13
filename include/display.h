#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

class Display {
  // Width and height of the window
  int windowSize;
  // Pointers to the SDL window and renderer
  SDL_Window* window;
  SDL_Renderer* render;
public:
  Display(int);
  void fillBlack();
  void end();
  void drawPixel(int x, int y) {SDL_RenderDrawPoint(render, x, y);};
  void drawRect(SDL_Rect* r) {SDL_RenderDrawRect(render, r);};
  void update() {SDL_RenderPresent(render);};
  void wait(int t) {SDL_Delay(t);};
};

#endif
