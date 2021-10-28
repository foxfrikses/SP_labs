#pragma once

#include "IWindow.h"

class RegistryEditorWindowClass final
  : public IWindowClass 
{
public:
  explicit RegistryEditorWindowClass(HINSTANCE hInst);

  LPCWSTR _GetWindowClassName() const override;
  WNDPROC _GetWindowProcedure() const override;
  HICON _GetIcon() const override;
};