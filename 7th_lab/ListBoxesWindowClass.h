#pragma once

#include "../shared/IWindowClass.h"

class ListBoxesWindowClass final 
  : public IWindowClass 
{
public:
  explicit ListBoxesWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HCURSOR _GetCursor() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
  UINT _GetWindowStyle() const override;
  LPCWSTR _GetWindowMenuName() const override;
};
