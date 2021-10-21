#pragma once

#include "IDCPrinter.h"
#include "PaintTools.h"

IDCPrinter& IDCPrinter::SetBrushColor(COLORREF rgb) 
{
  __brushColor = rgb;
  return *this;
}

IDCPrinter& IDCPrinter::SetPenColor(COLORREF rgb) 
{
  __penColor = rgb;
  return *this;
}

COLORREF IDCPrinter::PenColor() const 
{
  return __penColor;
}

COLORREF IDCPrinter::BrushColor() const 
{
  return __brushColor;
}

void IDCPrinter::Modify(HDC hdc) 
{
  SolidPen pen(PenColor(), 1);
  SolidBrush brush(BrushColor());
  auto initialPen = SelectObject(hdc, pen);
  auto initialBrush = SelectObject(hdc, brush);
  Print(hdc);
  SelectObject(hdc, initialPen);
  SelectObject(hdc, initialBrush);
}