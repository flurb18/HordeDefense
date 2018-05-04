#include "display.h"

int main(int argc, char* argv[]) {
  Display d = Display(640, 480);
  d.fillBlack();
  d.update();
  d.wait(3000);
  d.end();
}
