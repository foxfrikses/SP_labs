#include "IWindowClass.h"

IWindowClass::IWindowClass(HINSTANCE hInst) 
  : __hInst(hInst)
{}

LPCWSTR IWindowClass::GetWindowClassName() const {
  _Initialize();
  return _GetWindowClassName();
}

HINSTANCE IWindowClass::GetWindowClassHinstanse() const {
  return __hInst;
}

HCURSOR IWindowClass::_GetCursor() const {return 0;}
HICON IWindowClass::_GetIcon() const {return 0;}
UINT IWindowClass::_GetWindowStyle() const {return CS_HREDRAW | CS_VREDRAW;};
LPCWSTR IWindowClass::_GetWindowMenuName() const {return 0;};

void IWindowClass::_Initialize() const {
  if (__initialized) {
    return;
  }

  if (!__IsWindowClassRegistered()) {
    __RegisterWindowClass();
  }

  __initialized = true;
}

bool IWindowClass::__IsWindowClassRegistered() const {
  WNDCLASSEXW str;
  return GetClassInfoExW(__hInst, _GetWindowClassName(), &str) != FALSE;
}

void IWindowClass::__RegisterWindowClass() const {
  auto wndCls = __BuildWindowClass();
  RegisterClass(&wndCls);
}

WNDCLASS IWindowClass::__BuildWindowClass() const {
  WNDCLASS wc = {0};
  wc.hInstance = __hInst;
  wc.lpszClassName = _GetWindowClassName();
  wc.lpfnWndProc = _GetWindowProcedure();
  wc.lpszMenuName = _GetWindowMenuName();
  wc.style = _GetWindowStyle();
  wc.hCursor = _GetCursor();
  wc.hIcon = _GetIcon();
  return wc;
}