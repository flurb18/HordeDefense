#ifndef DISPLAY_H
#define DISPLAY_H

#include "square.h"

// Forward declarations of SDL and TTF stuff
class SDL_Window;
class SDL_Renderer;
typedef struct _TTF_Font TTF_Font;

class Display: public Square {
  // Pointers to the SDL window and renderer, and TTF font
  SDL_Window* window;
  SDL_Renderer* render;
  TTF_Font* font;
  // File where font was opened from
  const char* fontFile;
public:
  Display(int);
  void fillBlack();
  void end();
  void drawPixel(int, int);
  void drawRect(int, int, int, int);
  void drawRectFilled(int, int, int, int);
  void drawText(const char*, int, int);
  void update();
  void wait(int);
};

#endif
