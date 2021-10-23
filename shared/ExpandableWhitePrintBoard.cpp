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

    const RECT rcVert{cw, 0, newCw, newCh};
    const RECT rcHor{0, ch, newCw, newCh};

    SolidBrush whiteBrush(WHITE);

    FillRect(__printBoard, &rcVert, whiteBrush);
    FillRect(__printBoard, &rcHor, whiteBrush);
  }
}

void ExpandableWhitePrintBoard::Clear()
{
  const RECT rc{0, 0, (LONG)Width(), (LONG)Height()};
  FillRect(__printBoard, &rc, SolidBrush(WHITE));
}
