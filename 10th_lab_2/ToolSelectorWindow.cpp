#include "ToolSelectorWindow.h"

ToolSelectorWindow::ToolSelectorWindow(
  std::unique_ptr<IWindowClass> wc, 
  int nCmdShow
)
  : IWindow(std::move(wc), nCmdShow) 
{}

int ToolSelectorWindow::_GetWindowHeight() const
{
  return 280;
}

int ToolSelectorWindow::_GetWindowWidth() const
{
  return 250;
}

LPCWSTR ToolSelectorWindow::_GetWindowName() const 
{
  return L"ToolSelectorWindow";
}
