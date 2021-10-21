#pragma once

#include "IWindowClass.h"

class SimplePicPrinterWindowClass final 
  : public IWindowClass 
{
public:
  explicit SimplePicPrinterWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
};

