#pragma once

#include "IWindow.h"

class PriorityManagerWindowClass final
  : public IWindowClass 
{
public:
  explicit PriorityManagerWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  WNDPROC _GetWindowProcedure() const override;
  HICON _GetIcon() const override;
};
