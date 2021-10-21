#include "DCStarPrinter.h"

#include <array>

void DCStarPrinter::Print(HDC hdc) {
  auto [x1, y1] = BeginPoint();
  auto [x2, y2] = EndPoint();

  std::array<POINT, 5> star;

  star[0].x = x1 + (x2 - x1) * 1 / 5;
  star[0].y = y2;

  star[1].x = x1 + (x2 - x1) / 2;
  star[1].y = y1;

  star[2].x = x2 - (x2 - x1) * 1 / 5;
  star[2].y = y2;

  star[3].x = x1;
  star[3].y = y1 + (y2 - y1) / 3;

  star[4].x = x2;
  star[4].y = y1 + (y2 - y1) / 3;

  Polygon(hdc, star.data(), star.size());
}
