#include "DCRhombusPrinter.h"

#include <array>

void DCRhombusPrinter::Print(HDC hdc) {
  auto [x1, y1] = BeginPoint();
  auto [x2, y2] = EndPoint();

  std::array<POINT, 4> rhombus;

  rhombus[0].x = (x2 + x1) / 2;
  rhombus[0].y = y1;

  rhombus[1].x = x1;
  rhombus[1].y = (y1 + y2) / 2;

  rhombus[2].x = (x2 + x1) / 2;
  rhombus[2].y = y2;

  rhombus[3].x = x2;
  rhombus[3].y = (y1 + y2) / 2;

  Polygon(hdc, rhombus.data(), rhombus.size());
}
