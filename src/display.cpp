#include "display.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "game.h"

// Initialize a display, with a window size (square, so width == height)
Display::Display(int s) : Square(s) {
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
    size,                               //    width, in pixels
    size,                               //    height, in pixels
    SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL //    flags
  );
  // Check that the window was successfully made
  if (window == nullptr){
    // In the event that the window could not be made...
    std::cerr << "Could not create window!\n";
    std::cerr << SDL_GetError() << std::endl;
    throw SDL_GetError();
  }
  render = SDL_CreateRenderer(window, -1, 0);
  //fontFile = "/usr/share/fonts/TTF/DejaVuSansMono.ttf";
  fontFile = "/usr/share/fonts/TTF/Roboto-Regular.ttf";
  font = TTF_OpenFont(fontFile, 16);
  if (font == nullptr) {
    std::cerr << "Font could not be loaded!\n";
    std::cerr << TTF_GetError() << std::endl;
    throw TTF_GetError();
  }
}

/* Fill the display with black */
void Display::fillBlack() {
  setDrawColorBlack();
  // Clear the window with the color
  SDL_RenderClear(render);
}

/* Set the draw color to an rgb triple */
void Display::setDrawColor(int r, int g, int b) {
  SDL_SetRenderDrawColor(render, r, g, b, 255);
}

/* Set the draw color to the color of a given team */
void Display::setDrawColor(const Team *team) {
  SDL_SetRenderDrawColor(render, team->R, team->G, team->B, 255);
}

/* Set the draw color to white */
void Display::setDrawColorWhite() {
  setDrawColor(255, 255, 255);
}

/* Set the draw color to black */
void Display::setDrawColorBlack() {
  setDrawColor(0, 0, 0);
}

/* Draw a single pixel at (x,y) */
void Display::drawPixel(int x, int y) {
  SDL_RenderDrawPoint(render, x, y);
}

/* Draw a rectangle outline at (x,y) of size (w,h) */
void Display::drawRect(int x, int y, int w, int h) {
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderDrawRect(render, &rect);
}

/* Draw the SDL rectangle */
void Display::drawRect(SDL_Rect* r) {
  SDL_RenderDrawRect(render, r);
}

/* Draw a filled in rectangle at (x,y) of size (w,h) */
void Display::drawRectFilled(int x, int y, int w, int h) {
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(render, &rect);
}

/* Draw the string text at (x,y) using the preloaded font */
void Display::drawText(const char *text, int x, int y) {
  SDL_Color white = {255,255,255};
  SDL_Surface* surface = TTF_RenderText_Blended(font, text, white);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
  SDL_Rect rect = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  SDL_RenderCopy(render, texture, nullptr, &rect);
  SDL_DestroyTexture(texture);
}

/* Get what the size of some text will be on the display */
void Display::sizeText(const char *text, int *w, int *h) {
  TTF_SizeText(font, text, w, h);
}

/* Refresh the display */
void Display::update() {
  SDL_RenderPresent(render);
}

/* Wait t milliseconds */
void Display::wait(int t) {
  SDL_Delay(t);
}

/* Destroy all SDL and TTF stuff */
void Display::end() {
  TTF_Quit();
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
