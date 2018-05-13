#ifndef AGENT_H
#define AGENT_H

// Forward declaration of class Region
class Region;

class Agent {
  friend class Region;
private:
  Region* currentRegion;
  int x, y;
public:
  Agent(Region*);
  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
};

#endif
