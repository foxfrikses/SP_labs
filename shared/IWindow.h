#pragma once

#include "IWindowClass.h"
#include <memory>

class IWindow {
public:
  IWindow(std::unique_ptr<IWindowClass> wc, int nCmdShow) 
    : __wc(std::move(wc))
  {
    __CreateWindow();

    ShowWindow(__hwnd, nCmdShow);
    UpdateWindow(__hwnd);
  }

  int Run() {
    return __ProcessMessages();
  }

protected:
  virtual int _GetWindowHeight() const {return 480;}
  virtual int _GetWindowWidth() const {return 720;}
  virtual LPCWSTR _GetWindowName() const {return L"MyWindowName"; }

private:
  void __CreateWindow() {
    __hwnd = CreateWindowEx(
      0,
      __wc->GetWindowClassName(),
      _GetWindowName(),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 
      CW_USEDEFAULT, 
      _GetWindowWidth(), 
      _GetWindowHeight(),
      nullptr,
      NULL,
      __wc->GetWindowClassHinstanse(),
      nullptr
    );
  }

  int __ProcessMessages() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return 0;
  }

  const std::unique_ptr<IWindowClass> __wc;
  HWND __hwnd;
};
