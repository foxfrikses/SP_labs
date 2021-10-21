#include "DCEllipsePrinter.h"

void DCEllipsePrinter::Print(HDC hdc) {
  auto [x1, y1] = BeginPoint();
  auto [x2, y2] = EndPoint();
  Ellipse(hdc, x1, y1, x2, y2);
}
