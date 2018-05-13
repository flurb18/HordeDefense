#ifndef DISPLAY_H
#define DISPLAY_H

// Forward declarations of SDL and TTF stuff
class SDL_Window;
class SDL_Renderer;
class SDL_Rect;
typedef struct _TTF_Font TTF_Font;

class Display {
  // Width and height of the window
  int windowSize;
  // Pointers to the SDL window and renderer, and TTF font
  SDL_Window* window;
  SDL_Renderer* render;
  TTF_Font* font;
public:
  Display(int);
  void fillBlack();
  void end();
  void drawPixel(int, int);
  void drawRect(SDL_Rect*);
  void update();
  void wait(int);
};

#endif
