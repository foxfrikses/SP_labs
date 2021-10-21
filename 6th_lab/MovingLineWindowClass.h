#pragma once

#include "IWindowClass.h"

class MovingLineWindowClass final 
  : public IWindowClass 
{
public:
  explicit MovingLineWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HCURSOR _GetCursor() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
  UINT _GetWindowStyle() const override;
  LPCWSTR _GetWindowMenuName() const override;
};
