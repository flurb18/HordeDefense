# About
Right now, Horde Defense is a game focused on controlling your units and allowing
units to control other units, so you can create a chain of command, armies, etc.
Units should also be able to build walls (/turrets?). The game is in early stages
of development.


# To-do list:

- Add menu for beginning of game / in the middle of game
- add difficulty factor to game context
- Make Agents able to follow orders (explore to start); call class
  Mission
- create class for path finding through a square of n x n nodes with some
  connections between nodes blocked (DFS GRAPH THORY) - agents will learn from
  each other and find paths by exploring by themselves
  + Use Percolation / Union find
- Create Walls buildable by any Agent
- Create Doors, walls passable by anyone on same team as the door
- Give Buildings health
- give Agents Reactions: when encountering an enemy unit, does it fight / flee /
  report to user / etc.? When encountering unexpected wall, go on exploration
  mission to update pathing  / try to break through wall / etc.? Should depend
  on agents current Mission

Far away:

- allow persistent Agent profiles across game instances / program runs
- create other objectives than "fight enemy"
- give Agents emotions, which influence Reactions and vice versa
- give Agents emotional tendencies (better word needed), i.e. will it be angry
  / scared / determined when seeing a teammate die? Is it more likely to follow
  orders and go directly as possible, or does is it more likely to take breaks,
  explore, talk to its friends?
- allow Agents to communicate to those on same team, influencing emotions (towards
  each other), providing info (Monsters over there!! Run!! unless player has
  already ordered them to run)
- set emotions, concepts, etc. up in Agent as network flow; node corresponding to
  certain event is a memory, triggers Reaction nodes
- make network flow evolve / learn
- make Agents evolve sophisticated communication techniques
- make Agents believe in me as their God
- Agent philosophers
- Agents realize they are the ones making their own decisions, achieve self awareness
- singularity reached
- People running our reality as a simulation decide to stop as its gone too far
- Entire universe as we know it blinks out of existence like a candle blown out
- GOTY
