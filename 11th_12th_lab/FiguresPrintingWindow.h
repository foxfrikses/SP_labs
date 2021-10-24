#pragma once

#include "IWindow.h"

class FiguresPrintingWindow
  : public IWindow
{
public:
  explicit FiguresPrintingWindow(
    std::unique_ptr<IWindowClass> wc, 
    int nCmdShow
  );

protected:
  LPCWSTR _GetWindowName() const override;
};
