#include "FiguresPrintingWindow.h"

FiguresPrintingWindow::FiguresPrintingWindow(
  std::unique_ptr<IWindowClass> wc,
  int nCmdShow
) : IWindow(std::move(wc), nCmdShow)
{}

LPCWSTR FiguresPrintingWindow::_GetWindowName() const
{
  return L"FiguresPrinter";
}
