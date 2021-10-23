#include "ExpandablePrintBoard.h"

ExpandablePrintBoard::ExpandablePrintBoard(HDC targetDC, UINT width, UINT height) 
  : __printBoard(targetDC, width, height)
  , __width(width)
  , __height(height)
{}

void ExpandablePrintBoard::Resize(UINT width, UINT height) {
  if (width == Width() && height == Height())
    return;

  UINT cw = CapacityWidth();
  UINT ch = CapacityHeight();

  if (cw < width || ch < height) {
    if (cw < 3) {
      cw = 3;
    }

    if (ch < 3) {
      ch = 3;
    }

    while (cw < width) {
      cw *= 1.5;
    }

    while (ch < height) {
      ch *= 1.5;
    }

    __printBoard.SetSize(cw, ch);
  }

  __width = width;
  __height = height;
}

void ExpandablePrintBoard::ShrinkToFit() {
  if (__width < __printBoard.Width() ||
      __height < __printBoard.Height()) {
    __printBoard.SetSize(__width, __height);
  }
}
