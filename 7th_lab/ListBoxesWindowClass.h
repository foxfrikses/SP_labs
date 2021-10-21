#pragma once

#include "IWindowClass.h"

class ListBoxesWindowClass final 
  : public IWindowClass 
{
public:
  explicit ListBoxesWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
};
