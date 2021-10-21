#pragma once

#include <windows.h>
#include <string>
#include <mutex>
#include <unordered_set>
#include <memory>

class IWindowClass
{
public:
  explicit IWindowClass(HINSTANCE hInst);

  LPCWSTR GetWindowClassName() const;
  HINSTANCE GetWindowClassHinstanse() const;

protected:
  virtual LPCWSTR _GetWindowClassName() const = 0;
  virtual WNDPROC _GetWindowProcedure() const = 0;
  virtual HCURSOR _GetCursor() const;
  virtual HICON _GetIcon() const;
  virtual UINT _GetWindowStyle() const;
  virtual LPCWSTR _GetWindowMenuName() const;

protected:
  void _Initialize() const;

private:
  bool __IsWindowClassRegistered() const;
  void __RegisterWindowClass() const;
  WNDCLASS __BuildWindowClass() const;

  HINSTANCE __hInst;
  mutable bool __initialized = false;
};
