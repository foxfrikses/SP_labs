#pragma once

#include "IWindow.h"

class ToolSelectorWindow 
  : public IWindow 
{
public:
  explicit ToolSelectorWindow(std::unique_ptr<IWindowClass> wc, int nCmdShow);

protected:
  int _GetWindowHeight() const override;
  int _GetWindowWidth() const override;
  LPCWSTR _GetWindowName() const override;
};
