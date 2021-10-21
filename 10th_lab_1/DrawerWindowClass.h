#pragma once

#include "../shared/IWindowClass.h"

class DrawerWindowClass final 
  : public IWindowClass 
{
public:
  static inline const std::wstring ClassName = 
    L"DrawerWindowClass";

  explicit DrawerWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  HCURSOR _GetCursor() const override;
  HICON _GetIcon() const override;
  WNDPROC _GetWindowProcedure() const override;
  UINT _GetWindowStyle() const override;
  LPCWSTR _GetWindowMenuName() const override;
};
