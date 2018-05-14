#ifndef CONTEXT_H
#define CONTEXT_H

// Forward declarations
struct Team;

const int GAME_CONTEXT_ZOOMED_OUT = 2;
const int GAME_CONTEXT_ZOOMED_IN = 3;
const int GAME_CONTEXT_EXIT = 10;

class Context {
private:
  bool paused;
  unsigned int currentRegionIndex;
public:
  unsigned int type, t;
  Context();
  unsigned int getCurrentRegionIndex();
  void setCurrentRegionIndex(unsigned int);
  void togglePause() {paused = !paused;};
  bool isPaused() {return paused;};
  Team getTeam(int);
};


#endif
