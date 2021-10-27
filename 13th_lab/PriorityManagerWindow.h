#pragma once

#include "IWindow.h"

class PriorityManagerWindow
  : public IWindow
{
public:
  explicit PriorityManagerWindow(
    std::unique_ptr<IWindowClass> wc, 
    int nCmdShow
  );

protected:
  LPCWSTR _GetWindowName() const override;
};
