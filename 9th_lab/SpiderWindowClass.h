#pragma once

#include "IWindowClass.h"

class SpiderWindowClass final 
  : public IWindowClass 
{
public:
  explicit SpiderWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
};
