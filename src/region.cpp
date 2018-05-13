#include "region.h"

Region::Region(int x, int y, int s) {
  this->x = x;
  this->y = y;
  size = s;
  rect = {x, y, size, size};
}
