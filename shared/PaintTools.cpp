#include "PaintTools.h"

SolidPen::SolidPen(int width, COLORREF rgb) 
  : __pen{CreatePen(BS_SOLID, width, rgb)}
{}

SolidPen::operator HPEN() const {return __pen;}

SolidPen::~SolidPen() {
  DeleteObject(__pen);
}

SolidBrush::SolidBrush(COLORREF rgb) 
  : __brush{CreateSolidBrush(rgb)}
{}

SolidBrush::operator HBRUSH() const {return __brush;}

SolidBrush::~SolidBrush() {
  DeleteObject(__brush);
}