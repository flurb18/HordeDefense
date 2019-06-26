#ifndef TEAMS_H
#define TEAMS_H

/* Data structure to contain information about a team; unique team number and
   color */
struct Team {
  int teamNum;
  int R;
  int G;
  int B;
  Team(int t, int r, int g, int b): teamNum(t), R(r), G(g), B(b) {};
  bool equals(const Team* that) const {return this->teamNum == that->teamNum;};
};

/* Predefined teams */
const Team WHITE_TEAM = Team(0, 255, 255, 255);
const Team RED_TEAM = Team(1, 255, 0, 0);
const Team GREEN_TEAM = Team(2, 0, 255, 0);
const Team BLUE_TEAM = Team(3, 0, 0, 255);
const int NUM_OF_TEAMS = 4;

#endif
