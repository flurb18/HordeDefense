#include "display.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

// Initialize a display, with a window size (square, so width == height)
Display::Display(int s) {
  windowSize = s;
  // Initialize SDL, and check if it initialized correctly
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL not initialized correctly\n";
    throw SDL_GetError();
  }
  if (TTF_Init() < 0) {
    std::cerr <<  "TTF not initialized correctly\n";
    throw TTF_GetError();
  }
  window = SDL_CreateWindow(
    "Horde Defense",                  //    window title
    SDL_WINDOWPOS_UNDEFINED,           //    initial x position
    SDL_WINDOWPOS_UNDEFINED,           //    initial y position
    windowSize,                               //    width, in pixels
    windowSize,                               //    height, in pixels
    SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL //    flags
  );
  // Check that the window was successfully made
  if (window == NULL){
    // In the event that the window could not be made...
    std::cerr << "Could not create window!\n";
    throw SDL_GetError();
  }
  render = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
  font = TTF_OpenFont("Sans.ttf", 24);
}

void Display::fillBlack() {
  SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
  // Clear the window with the color
  SDL_RenderClear(render);
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

void Display::drawPixel(int x, int y) {SDL_RenderDrawPoint(render, x, y);}

void Display::drawRect(SDL_Rect* r) {SDL_RenderDrawRect(render, r);}

void Display::update() {SDL_RenderPresent(render);}

void Display::wait(int t) {SDL_Delay(t);}

void Display::end() {
  TTF_Quit();
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
