#ifndef AGENT_H
#define AGENT_H

// Forward declaration of current region
class Region;

class Agent {
  Region* currentRegion;
public:
  Agent();
  Agent(Region*);
};

#endif
