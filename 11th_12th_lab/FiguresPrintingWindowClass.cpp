#include "FiguresPrintingWindowClass.h"
#include "FiguresPrintingLogic.h"

#include <unordered_map>

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

FiguresPrintingWindowClass::FiguresPrintingWindowClass(HINSTANCE hInst)
  : IWindowClass(hInst)
{}

LPCWSTR FiguresPrintingWindowClass::_GetWindowClassName() const
{
  return L"FiguresPrintingWindowClass";
}
WNDPROC FiguresPrintingWindowClass::_GetWindowProcedure() const
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}

unordered_map<int, FiguresPrintingLogic> handlerToLogic;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToLogic.emplace(
        (int)hwnd, 
        FiguresPrintingLogic(
          hwnd, 
          ((LPCREATESTRUCT)lparam)->hInstance
        )
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

    case WM_TIMER:
      if (auto it = handlerToLogic.find((int)hwnd);
          it != handlerToLogic.end()) {
        it->second.Timer(wparam);
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
