#include "resource.h"

#include "ToolSelectorWindowClass.h"
#include "ToolSelectorLogic.h"

#include <unordered_map>

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

ToolSelectorWindowClass::ToolSelectorWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{}
HICON ToolSelectorWindowClass::_GetIcon() const {
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
WNDPROC ToolSelectorWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
LPCWSTR ToolSelectorWindowClass::_GetWindowClassName() const 
{
  return L"ToolSelectorWindowClass";
}

unordered_map<int, ToolSelectorLogic> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToLogic.emplace(
        (int)hwnd, 
        ToolSelectorLogic(hwnd, 
                          ((LPCREATESTRUCT)lparam)->hInstance)
      ); 
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

    case WM_SIZE:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Resize(LOWORD(lparam), HIWORD(lparam));
      }
      break;

    case WM_COMMAND:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Command(wparam, lparam);
      }
      break;

    default:
      {
        if (0xC000 <= message && message <= 0xFFFF) { 
          if (auto it = handlerToLogic.find((int)hwnd);
              it != handlerToLogic.end()) {
            it->second.CustomStringMessage(message, wparam, lparam);
          }
          break;
        }
      }
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}