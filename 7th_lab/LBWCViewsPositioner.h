#pragma once

struct Position {int x, y, w, h;};

struct LBWCPositions {
  Position addBtnPosition;
  Position clearBtnPosition;
  Position deleteBtnPosition;
  Position toRightBtnPosition;
  Position rightListPosition;
  Position leftListPosition;
  Position editPosition;
};

class LBWCViewsPositioner {
public:
  LBWCPositions ComputePositions(int clientWidth, int clientHeight);
};
