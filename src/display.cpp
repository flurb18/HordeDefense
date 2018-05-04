#include <SDL2/SDL.h>
#include <iostream>
#include "display.h"

// Initialize a display, with a window width and height
Display::Display(int w, int h) {
  width = w;
  height = h;
  // Initialize SDL, and check if it initialized correctly
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL not initialized correctly\n";
    throw SDL_GetError();
  }
  window = SDL_CreateWindow(
    "Town Of One",                  //    window title
    SDL_WINDOWPOS_UNDEFINED,           //    initial x position
    SDL_WINDOWPOS_UNDEFINED,           //    initial y position
    width,                               //    width, in pixels
    height,                               //    height, in pixels
    SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL //    flags
  );
  // Check that the window was successfully made
  if (window == NULL){
    // In the event that the window could not be made...
    std::cout << "Could not create window!\n";
    throw SDL_GetError();
  }
  render = SDL_CreateRenderer(window, -1, 0);
}

void Display::fillBlack() {
  SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
  // Clear the window with the color
  SDL_RenderClear(render);
}

void Display::update() {
  SDL_RenderPresent(render);
}

void Display::wait(int t) {
  SDL_Delay(t);
}

void Display::end() {
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
