#include "PriorityManagerWindowClass.h"
#include "PriorityManagerLogic.h"

#include "resource.h"

#include <windowsx.h>

#include <unordered_map>

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

PriorityManagerWindowClass::PriorityManagerWindowClass(HINSTANCE hInst)
  : IWindowClass(hInst)
{}

LPCWSTR PriorityManagerWindowClass::_GetWindowClassName() const
{
  return L"PriorityManagerWindowClass";
}
WNDPROC PriorityManagerWindowClass::_GetWindowProcedure() const
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
HICON PriorityManagerWindowClass::_GetIcon() const 
{
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}

unordered_map<int, PriorityManagerLogic> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToLogic.emplace(
        (int)hwnd, 
        PriorityManagerLogic(
        hwnd, 
        ((LPCREATESTRUCT)lparam)->hInstance
      )
      );
      break;

    case WM_CONTEXTMENU:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.ContextMenu((HWND)wparam, 
                               GET_X_LPARAM(lparam), 
                               GET_Y_LPARAM(lparam));
      }
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
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}