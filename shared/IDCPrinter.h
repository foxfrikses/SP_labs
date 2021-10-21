#pragma once

#include "IDCModifier.h"

class IDCPrinter
  : public IDCModifier 
{
  COLORREF __penColor = RGB(0, 0, 0); 
  COLORREF __brushColor = RGB(0xFF, 0xFF, 0xFF); 

protected:
  virtual void Print(HDC hdc) = 0;

public:
  IDCPrinter& SetPenColor(COLORREF rgb);
  IDCPrinter& SetBrushColor(COLORREF rgb);

  COLORREF PenColor() const;
  COLORREF BrushColor() const;

  void Modify(HDC hdc) override final;
};
