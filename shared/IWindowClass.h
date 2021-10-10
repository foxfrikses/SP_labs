#pragma once

#include <windows.h>
#include <string>
#include <mutex>
#include <unordered_set>
#include <memory>

class IWindowClass
{
public:
  IWindowClass(HINSTANCE hInst) 
    : __hInst(hInst)
  {}

  LPCWSTR GetWindowClassName() const {
    _Initialize();
    return _GetWindowClassName();
  }

  HINSTANCE GetWindowClassHinstanse() const {
    return __hInst;
  }

protected:
  virtual LPCWSTR _GetWindowClassName() const = 0;
  virtual HCURSOR _GetCursor() const {return 0;}
  virtual HICON _GetIcon() const {return 0;}
  virtual WNDPROC _GetWindowProcedure() const = 0;
  virtual UINT _GetWindowStyle() const {return CS_HREDRAW | CS_VREDRAW;};
  virtual LPCWSTR _GetWindowMenuName() const {return 0;};

protected:
  void _Initialize() const {
    if (__initialized) {
      return;
    }

    if (!__IsWindowClassRegistered()) {
      __RegisterWindowClass();
    }

    __initialized = true;
  }
  mutable bool __initialized = false;

private:
  bool __IsWindowClassRegistered() const {
    WNDCLASSEXW str;
    return GetClassInfoExW(__hInst, _GetWindowClassName(), &str) != FALSE;
  }
  void __RegisterWindowClass() const {
    auto wndCls = __BuildWindowClass();
    RegisterClass(&wndCls);
  }

  WNDCLASS __BuildWindowClass() const {
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

  HINSTANCE __hInst;
};
