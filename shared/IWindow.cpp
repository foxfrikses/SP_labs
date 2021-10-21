#include "IWindow.h"

IWindow::IWindow(std::unique_ptr<IWindowClass> wc, int nCmdShow) 
  : __wc(std::move(wc))
  , __nCmdShow(nCmdShow)
{}

int IWindow::Run() {
  __CreateWindow();

  ShowWindow(__hwnd, __nCmdShow);
  UpdateWindow(__hwnd);

  return __ProcessMessages();
}

int IWindow::_GetWindowHeight() const {return 480;}
int IWindow::_GetWindowWidth() const {return 720;}
LPCWSTR IWindow::_GetWindowName() const {return L"MyWindowName"; }

void IWindow::__CreateWindow() {
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

int IWindow::__ProcessMessages() {
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}
