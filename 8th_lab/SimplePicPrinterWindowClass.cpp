#include "SimplePicPrinterWindowClass.h"

#include "resource.h"
#include <windows.h>

#include <unordered_map>
#include <string>

#include "SimplePicPrinter.h"

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

SimplePicPrinterWindowClass::SimplePicPrinterWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{}
HCURSOR SimplePicPrinterWindowClass::_GetCursor() const {return 0;}
HICON SimplePicPrinterWindowClass::_GetIcon() const {
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
UINT SimplePicPrinterWindowClass::_GetWindowStyle() const {return 0;}
LPCWSTR SimplePicPrinterWindowClass::_GetWindowMenuName() const {return 0;}
WNDPROC SimplePicPrinterWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
LPCWSTR SimplePicPrinterWindowClass::_GetWindowClassName() const 
{
  return L"SimplePicPrinterWindowClass";
}

unordered_map<int, SimplePicPrinter> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_SIZE:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        SimplePicPrinter& l = it->second;

        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        l.Resize(width, height);
      }
      break;

    case WM_CREATE:
      handlerToLogic.emplace((int)hwnd,  SimplePicPrinter(hwnd, ((LPCREATESTRUCT)lparam)->hInstance)); 
      break;

    case WM_DESTROY:
      handlerToLogic.erase((int)hwnd);
      PostQuitMessage(0);
      break;

    case WM_COMMAND:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        SimplePicPrinter& l = it->second;

        l.Command(wparam, lparam);
      }
      break;

    case WM_PAINT:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        SimplePicPrinter& l = it->second;

        l.Print();
      }
      break;

    case WM_DRAWITEM:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        SimplePicPrinter& l = it->second;

        l.DrawItem(lparam);
      }
      break;

    default:
      return DefWindowProc(hwnd, message, wparam, lparam);
  }
  return 0;
}
