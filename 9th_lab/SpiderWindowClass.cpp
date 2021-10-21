#include "SpiderWindowClass.h"

#include "SWCController.h"

#include <unordered_map>

#include <windowsx.h>
#include <windows.h>

#include "resource.h"

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

SpiderWindowClass::SpiderWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{}
HICON SpiderWindowClass::_GetIcon() const {
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
WNDPROC SpiderWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
LPCWSTR SpiderWindowClass::_GetWindowClassName() const 
{
  return L"SpiderWindowClass";
}

unordered_map<int, SWCController> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToLogic.emplace((int)hwnd,  SWCController(hwnd, ((LPCREATESTRUCT)lparam)->hInstance)); 
      break;

    case WM_DESTROY:
      handlerToLogic.erase((int)hwnd);
      PostQuitMessage(0);
      break;

    case WM_PAINT:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Print();
      }
      break;

    case WM_TIMER:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Timer(wparam, lparam);
      }
      break;

    case WM_SIZE:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Resize(LOWORD(lparam), HIWORD(lparam));
      }
      break;

    case WM_LBUTTONUP:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.SetAimPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
      }
      break;

    default:
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}
