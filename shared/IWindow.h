#pragma once

#include "IWindowClass.h"
#include <memory>
#include <windows.h>

class IWindow 
{
public:
  explicit IWindow(std::unique_ptr<IWindowClass> wc, int nCmdShow);
  int Run();

protected:
  virtual int _GetWindowHeight() const;
  virtual int _GetWindowWidth() const;
  virtual LPCWSTR _GetWindowName() const;

private:
  void __CreateWindow();

  int __ProcessMessages();

  const int __nCmdShow;
  const std::unique_ptr<IWindowClass> __wc;
  HWND __hwnd = nullptr;
};