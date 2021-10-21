#pragma once

#include "ExpandablePrintBoard.h"

class ExpandableWhitePrintBoard
{
  ExpandablePrintBoard __printBoard;

public:
  explicit ExpandableWhitePrintBoard(HDC targetDC, UINT width, UINT height);

  void Resize(UINT width, UINT height);
  void ShrinkToFit() {__printBoard.ShrinkToFit();}

  operator HDC() const {return __printBoard;}

  UINT CapacityWidth() const {return __printBoard.CapacityWidth();}
  UINT CapacityHeight() const {return __printBoard.CapacityHeight();}
  UINT Width() const {return __printBoard.Width();}
  UINT Height() const {return __printBoard.Height();}
};
