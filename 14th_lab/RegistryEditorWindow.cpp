#include "RegistryEditorWindow.h"

RegistryEditorWindow::RegistryEditorWindow(
  std::unique_ptr<IWindowClass> wc,
  int nCmdShow
) : IWindow(std::move(wc), nCmdShow)
{}

LPCWSTR RegistryEditorWindow::_GetWindowName() const
{
  return L"Registry Editor";
}