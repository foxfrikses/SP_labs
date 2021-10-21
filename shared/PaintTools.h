#pragma once

#include <windows.h>

struct Position {int x, y, w, h;};

class SolidPen final {
  HPEN __pen;
public:
  explicit SolidPen(int width, COLORREF rgb);
  operator HPEN() const;
  ~SolidPen();
};
class SolidBrush final {
  HBRUSH __brush;
public:
  explicit SolidBrush(COLORREF rgb);
  operator HBRUSH() const;
  ~SolidBrush();
};

constexpr COLORREF WHITE = RGB(255, 255, 255);
constexpr COLORREF BLACK = RGB(0, 0, 0);
