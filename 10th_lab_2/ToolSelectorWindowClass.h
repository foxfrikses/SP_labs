#pragma once

#include "IWindowClass.h"

#include <string>

class ToolSelectorWindowClass final 
  : public IWindowClass 
{
public:
  explicit ToolSelectorWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
};
