#pragma once

#include "IWindow.h"

class RegistryEditorWindow
  : public IWindow
{
public:
  explicit RegistryEditorWindow(
    std::unique_ptr<IWindowClass> wc, 
    int nCmdShow
  );

protected:
  LPCWSTR _GetWindowName() const override;
};