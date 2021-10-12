#include "LBWCViewsPositioner.h"

LBWCPositions LBWCViewsPositioner::ComputePositions(int clientWidth, int clientHeight)
{
  //  _ - m         __ - m         _ - m
  // ________________________________
  // | ____________   ____________  | 
  // | /           \  /           \ |
  // | |           |  |           | |
  // | | LListBox  |  | RListBox  | |
  // | |           |  |           | |
  // | \___________/  \___________/ |
  // | ___________________________  | | - h
  // | /         TextEdit         \ | 
  // | \__________________________/ |
  // | ____________   ____________  |
  // | | AddBtn    |  | DeleteBtn | |
  // | \___________/  \___________/ |
  // | ____________   ____________  |
  // | | ClearBtn  |  | ToRightBtn| |
  // | \__________/   \___________/ |
  // |______________________________|

  const int minWidth = 300, minHeight = 300;

  if (clientWidth < minWidth) {
    clientWidth = minWidth;
  }
  if (clientHeight < minHeight) {
    clientHeight = minHeight;
  }
  
  const int m = 10;
  const int h = 10;

  int accumulatedViewsHeight = clientHeight - 5 * m;

  const int listBoxHeight = accumulatedViewsHeight * 6 / 10; 
  accumulatedViewsHeight -= listBoxHeight;
  const int buttonHeight = accumulatedViewsHeight / 5 * 2;
  accumulatedViewsHeight -= buttonHeight * 2; 
  const int editHeight = accumulatedViewsHeight;

  const int accumulatedViewsWidth = (clientWidth - 3 * m) / 2 * 2;

  const int leftWidth = accumulatedViewsWidth / 2; 
  const int rightWidth = accumulatedViewsWidth / 2;
  const int editWidth = accumulatedViewsWidth + m;

  LBWCPositions pos;  

  pos.leftListPosition.x = m;
  pos.leftListPosition.y = h;
  pos.leftListPosition.w = leftWidth;
  pos.leftListPosition.h = listBoxHeight;

  pos.rightListPosition.x = m * 2 + leftWidth;
  pos.rightListPosition.y = h;
  pos.rightListPosition.w = rightWidth;
  pos.rightListPosition.h = listBoxHeight;

  pos.editPosition.x = m;
  pos.editPosition.y = h * 2 + listBoxHeight;
  pos.editPosition.w = editWidth;
  pos.editPosition.h = editHeight;

  pos.addBtnPosition.x = m;
  pos.addBtnPosition.y = h * 3 + listBoxHeight + editHeight;
  pos.addBtnPosition.w = leftWidth;
  pos.addBtnPosition.h = buttonHeight;

  pos.clearBtnPosition.x = m;
  pos.clearBtnPosition.y = h * 4 + listBoxHeight + editHeight + buttonHeight;
  pos.clearBtnPosition.w = leftWidth;
  pos.clearBtnPosition.h = buttonHeight;

  pos.deleteBtnPosition.x = m * 2 + leftWidth;
  pos.deleteBtnPosition.y = h * 3 + listBoxHeight + editHeight;
  pos.deleteBtnPosition.w = rightWidth;
  pos.deleteBtnPosition.h = buttonHeight;

  pos.toRightBtnPosition.x = m * 2 + leftWidth;
  pos.toRightBtnPosition.y = h * 4 + listBoxHeight + editHeight + buttonHeight;
  pos.toRightBtnPosition.w = rightWidth;
  pos.toRightBtnPosition.h = buttonHeight;
  
  return pos;
}
