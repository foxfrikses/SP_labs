#pragma once

#include <windows.h>

struct Position {int x, y, w, h;};

class SolidPen {
  HPEN pen;
public:
  explicit SolidPen(int width, COLORREF rgb) 
    : pen{CreatePen(BS_SOLID, width, rgb)}
  {}
  operator HPEN() const {return pen;}
  ~SolidPen() {
    DeleteObject(pen);
  }
};
class SolidBrush {
  HBRUSH brush;
public:
  explicit SolidBrush(COLORREF rgb) 
    : brush{CreateSolidBrush(rgb)}
  {}
  operator HBRUSH() const {return brush;}
  ~SolidBrush() {
    DeleteObject(brush);
  }
};

constexpr COLORREF WHITE = RGB(255, 255, 255);
constexpr COLORREF BLACK = RGB(0, 0, 0);
