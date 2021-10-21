#include "ListBoxesWindowClass.h"

#include "resource.h"
#include <windows.h>

#include <unordered_map>
#include <string>

#include "LBWCViews.h"

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

ListBoxesWindowClass::ListBoxesWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{}
HICON ListBoxesWindowClass::_GetIcon() const {
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
WNDPROC ListBoxesWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
LPCWSTR ListBoxesWindowClass::_GetWindowClassName() const 
{
  return L"ListBoxesWindowClass";
}

unordered_map<int, LBWCViews> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_SIZE:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        LBWCViews& l = it->second;

        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        l.Resize(width, height);
      }
      break;

    case WM_CREATE:
      handlerToLogic.emplace((int)hwnd,  LBWCViews(hwnd, ((LPCREATESTRUCT)lparam)->hInstance)); 
      break;

    case WM_DESTROY:
      handlerToLogic.erase((int)hwnd);
      PostQuitMessage(0);
      break;

    case WM_COMMAND:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        LBWCViews& l = it->second;
        l.Command(wparam, lparam);
      }
      break;

    case WM_PAINT:
      {
        PAINTSTRUCT ps; 
        auto hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
        EndPaint(hwnd, &ps);
      }
      break;

    default:
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}