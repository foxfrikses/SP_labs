#pragma once

#include "PrintBoard.h"

class ExpandablePrintBoard 
{
  PrintBoard __printBoard;

  UINT __width = 0;
  UINT __height = 0;

public:
  explicit ExpandablePrintBoard(HDC targetDC, UINT width, UINT height);

  void Resize(UINT width, UINT height);
  void ShrinkToFit();

  operator HDC() const {return __printBoard;}

  UINT CapacityWidth() const {return __printBoard.Width();}
  UINT CapacityHeight() const {return __printBoard.Height();}
  UINT Width() const {return __width;}
  UINT Height() const {return __height;}
};
