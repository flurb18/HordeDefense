#ifndef CONTEXT_H
#define CONTEXT_H

const int GAME_CONTEXT_ZOOMED_OUT = 2;
const int GAME_CONTEXT_EXIT = 10;

class Context {
private:
  unsigned int outlinedRegionIndex;
public:
  unsigned int type;
  Context();
  unsigned int getOutlinedRegionIndex();
  void setOutlinedRegionIndex(unsigned int);
};


#endif
