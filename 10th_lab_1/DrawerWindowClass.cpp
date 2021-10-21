#include "resource.h"

#include "DrawerWindowClass.h"
#include "DrawerLogic.h"

#include <unordered_map>

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

DrawerWindowClass::DrawerWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{}
HCURSOR DrawerWindowClass::_GetCursor() const {return 0;}
HICON DrawerWindowClass::_GetIcon() const {
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
UINT DrawerWindowClass::_GetWindowStyle() const {return 0;}
LPCWSTR DrawerWindowClass::_GetWindowMenuName() const {return 0;}
WNDPROC DrawerWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
LPCWSTR DrawerWindowClass::_GetWindowClassName() const 
{
  return L"DrawerWindowClass";
}

unordered_map<int, DrawerLogic> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToLogic.emplace(
        (int)hwnd, 
        DrawerLogic(hwnd, 
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

    case WM_LBUTTONUP:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {

        it->second.OnMouseRelease(LOWORD(lparam), HIWORD(lparam));
      }
      break;

    case WM_LBUTTONDOWN:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {

        it->second.OnMousePressed(LOWORD(lparam), HIWORD(lparam));
      }
      break;

    case WM_MOUSEMOVE:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {

        it->second.OnMouseMove(LOWORD(lparam), HIWORD(lparam));
      }
      break;

    case WM_COPYDATA:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.CopyData(wparam, lparam);
      }
      break;

    default:
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}