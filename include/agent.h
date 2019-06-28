#ifndef AGENT_H
#define AGENT_H

/* Forward Declarations */
class Game;
struct MapUnit;
struct Team;

class Agent {
  friend class Spawner;
private:
  int dx, dy;
  unsigned long long int lastUpdatedTimestamp;
  const Team* team;
  bool move();
  bool moveTo(MapUnit*);
public:
  Game* game;
  MapUnit* unit;
  Agent(Game*, MapUnit*, const Team*);
  ~Agent();
  bool update();
};

#endif
