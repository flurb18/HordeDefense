#ifndef DISPLAY_H
#define DISPLAY_H

#include "square.h"

/* Forward declarations */
class SDL_Window;
class SDL_Renderer;
class SDL_Rect;
typedef struct _TTF_Font TTF_Font;
struct Team;

class Display: public Square {
  /* Pointers to the SDL window and renderer, and TTF font */
  SDL_Window* window;
  SDL_Renderer* render;
  TTF_Font* font;
  /* File where font was opened from */
  const char* fontFile;
public:
  Display(int);
  void fillBlack();
  void end();
  void setDrawColor(int, int, int);
  void setDrawColor(const Team*);
  void setDrawColorWhite();
  void setDrawColorBlack();
  void drawPixel(int, int);
  void drawRect(int, int, int, int);
  void drawRect(SDL_Rect*);
  void drawRectFilled(int, int, int, int);
  void drawText(const char*, int, int);
  void sizeText(const char*, int*, int*);
  void update();
  void wait(int);
};

#endif
