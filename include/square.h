#ifndef SQUARE_H
#define SQUARE_H

// Square structure to be extended by various classes
struct Square {
  unsigned int size;
  Square(unsigned int s) {size = s;};
  int getRadius() { return size / 2; };
  int getSize() { return size; };
};

#endif
