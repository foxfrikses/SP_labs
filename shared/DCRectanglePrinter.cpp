#include "DCRectanglePrinter.h"

#include "PaintTools.h"

void DCRectanglePrinter::Print(HDC hdc) {
  auto [x1, y1] = BeginPoint();
  auto [x2, y2] = EndPoint();
  Rectangle(hdc, x1, y1, x2, y2);
}
