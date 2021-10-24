#pragma once

#include "IWindowClass.h"

class FiguresPrintingWindowClass final
  : public IWindowClass 
{
public:
  explicit FiguresPrintingWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  WNDPROC _GetWindowProcedure() const override;
  HICON _GetIcon() const override;
};
