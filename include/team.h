#ifndef TEAM_H
#define TEAM_H

struct Team {
  int teamNum;
  int R;
  int G;
  int B;
  Team(int t, int r, int g, int b): teamNum(t), R(r), G(g), B(b) {};
  bool equals(Team that) {return this->teamNum == that.teamNum;};
};

const Team WHITE_TEAM = Team(0, 255, 255, 255);
const Team RED_TEAM = Team(1, 255, 0, 0);
const Team GREEN_TEAM = Team(2, 0, 255, 0);
const Team BLUE_TEAM = Team(3, 0, 0, 255);
// Add any teams we like

#endif
