#ifndef AGENT_H
#define AGENT_H

/* Forward Declarations */
class Game;
class Paths;
struct MapUnit;
struct Team;

const int OBJECTIVE_TYPE_WAIT = 0;
const int OBJECTIVE_TYPE_EXPLORE = 1;
const int OBJECTIVE_TYPE_FOLLOW_SCENT = 2;
const int OBJECTIVE_TYPE_BUILD = 3;
const int OBJECTIVE_TYPE_CHARGE = 4;

class Agent {
  friend class Spawner;
private:
  int objective, dx, dy;
  unsigned long long int lastUpdatedTimestamp;
  Paths* paths;
  const Team* team;
  bool move();
  bool moveTo(MapUnit*);
public:
  Game* game;
  MapUnit* unit;
  Agent(Game*, Paths*, MapUnit*, const Team*);
  ~Agent();
  void update();
};

#endif
