#include "PriorityManagerWindow.h"

PriorityManagerWindow::PriorityManagerWindow(
  std::unique_ptr<IWindowClass> wc,
  int nCmdShow
) : IWindow(std::move(wc), nCmdShow)
{}

LPCWSTR PriorityManagerWindow::_GetWindowName() const
{
  return L" PriorityManager";
}