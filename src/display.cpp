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
    std::cerr << SDL_GetError() << std::endl;
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
    std::cerr << SDL_GetError() << std::endl;
    throw SDL_GetError();
  }
  render = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
  font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono.ttf", 24);
  if (font == NULL) {
    std::cerr << "Font could not be laoded!\n";
    std::cerr << TTF_GetError() << std::endl;
    throw TTF_GetError();
  }
}

void Display::fillBlack() {
  SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
  // Clear the window with the color
  SDL_RenderClear(render);
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

void Display::drawPixel(int x, int y) {
  SDL_RenderDrawPoint(render, x, y);
}

void Display::drawRect(SDL_Rect* r) {
  SDL_RenderDrawRect(render, r);
}

void Display::drawText(const char *text, int x, int y) {
  SDL_Color white = {255,255,255};
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, white);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
  SDL_Rect rect = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  SDL_RenderCopy(render, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
}

void Display::update() {
  SDL_RenderPresent(render);
}

void Display::wait(int t) {
  SDL_Delay(t);
}

void Display::end() {
  TTF_Quit();
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
