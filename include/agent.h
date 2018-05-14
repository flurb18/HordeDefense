#ifndef AGENT_H
#define AGENT_H

// Forward declarations
class Context;
class Region;
struct Team;

class Agent {
  friend class Region;
private:
  unsigned int x, y;
  Context* gameContext;
  Region* currentRegion;
public:
  const Team* team;
  Agent(Context* c, Region* r, const Team* t, int x_, int y_):\
         x(x_), y(y_), gameContext(c), currentRegion(r), team(t) {};
  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
};

#endif
