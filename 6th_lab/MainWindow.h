#pragma once

#include "IWindow.h"

class MainWindow : public IWindow {
public:
  MainWindow(std::unique_ptr<IWindowClass> wc, int nCmdShow)
    : IWindow(std::move(wc), nCmdShow) 
  {}
};
