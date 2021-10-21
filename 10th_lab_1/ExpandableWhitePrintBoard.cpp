#include "ExpandableWhitePrintBoard.h"

#include "PaintTools.h"

ExpandableWhitePrintBoard::ExpandableWhitePrintBoard(
  HDC targetDC, UINT width, UINT height
) : __printBoard(targetDC, width, height)
{
  RECT rc{0, 0, width, height};
  FillRect(__printBoard, &rc, SolidBrush(WHITE));
}

void ExpandableWhitePrintBoard::Resize(
  UINT width, UINT height
) {
  if (width == Width() && height == Height())
    return;

  const auto cw = __printBoard.CapacityWidth();
  const auto ch = __printBoard.CapacityHeight();

  __printBoard.Resize(width, height);

  if (cw < width || ch < height) {
    const auto newCw = __printBoard.CapacityWidth();
    const auto newCh = __printBoard.CapacityHeight();

    const RECT rcVert{cw, 0, newCw-1, newCh-1};
    const RECT rcHor{0, ch, newCw-1, newCh-1};

    SolidBrush whiteBrush(WHITE);

    FillRect(__printBoard, &rcVert, whiteBrush);
    FillRect(__printBoard, &rcHor, whiteBrush);
  }
}
