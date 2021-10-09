#include "MovingLineWindowClass.h"

#include "resource.h"
#include <windows.h>

#include <unordered_map>

using namespace std;

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

MovingLineWindowClass::MovingLineWindowClass(HINSTANCE hInst) 
  : IWindowClass(hInst)
{
  Initialize();
}

LPCWSTR MovingLineWindowClass::_GetWindowClassName() const 
{
  return L"MovingLineWindowClass";
}
HCURSOR MovingLineWindowClass::_GetCursor() const 
{
  return LoadCursor(GetWindowClassHinstanse(), 
                    MAKEINTRESOURCE(IDC_STUPID_CURSOR));
}
HICON MovingLineWindowClass::_GetIcon() const 
{
  return LoadIcon(GetWindowClassHinstanse(), 
                  MAKEINTRESOURCE(IDI_STUPID_ICON));
}
WNDPROC MovingLineWindowClass::_GetWindowProcedure() const 
{
  return reinterpret_cast<WNDPROC>(WindowsProc);
}
UINT MovingLineWindowClass::_GetWindowStyle() const 
{
  return 0;
}
LPCWSTR MovingLineWindowClass::_GetWindowMenuName() const 
{
  return MAKEINTRESOURCE(IDR_START_STOP_MENU);
}

class MovingLine {
  HWND hwnd;

  static const int leftBorder = 0;
  static const int rightBorder = 100;
  int leftPosition = 25;
  int rightPosition = 75;

  bool isVisible = false;

  enum MovingDirection {right = 1, left = -1};
  int dir = MovingDirection::right;

  std::wstring message = L"Hello World!";

public:
  MovingLine(HWND hwnd) 
    : hwnd(hwnd)
  {}
  
  void Move() {
    if (rightPosition == rightBorder || 
        leftPosition == leftBorder) {
      dir *= -1;
    }
    rightPosition += dir;
    leftPosition += dir;
  }

  void SetVisible(bool visible) {
    isVisible = visible;
  }

  void Print(HDC hdc) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    FillRect(hdc, &rc, (HBRUSH) (COLOR_WINDOW+1));
    if (isVisible) {
      auto left = rc.left;
      auto right = rc.right;
      rc.right = 
        left + (right - left) * rightPosition / 
        (rightBorder - leftBorder);
      rc.left = 
        left + (right - left) * leftPosition / 
        (rightBorder - leftBorder);
      SetROP2(hdc, R2_NOT);
      DrawText(
        hdc,
        message.c_str(),
        message.size(),
        &rc,
        DT_VCENTER | DT_CENTER | DT_SINGLELINE  
      );
    }
  }
};

unordered_map<int, MovingLine> handlerToMovingLine;

enum objectsId {
  TIMER_ID,
};

LRESULT APIENTRY WindowsProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_CREATE:
      handlerToMovingLine.emplace((int)hwnd, hwnd);
      return 0L;

    case WM_DESTROY:
      KillTimer(hwnd, TIMER_ID);
      handlerToMovingLine.erase((int)hwnd);
      PostQuitMessage(0);
      return 0L;

    case WM_TIMER:
      if (auto it = handlerToMovingLine.find((int)hwnd);
          it != handlerToMovingLine.end()) {
        MovingLine& ml = it->second;

        ml.Move();
        auto hdc = GetDC(hwnd);
        ml.Print(hdc);
      }
      return 0L;

    case WM_PAINT:
      if (auto it = handlerToMovingLine.find((int)hwnd);
          it != handlerToMovingLine.end()) {
        MovingLine& ml = it->second;

        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hwnd, &ps);
        ml.Print(hdc);
        EndPaint(hwnd, &ps);
        return 0L;
      }
      return DefWindowProc(hwnd, message, wparam, lparam);
      break;

    case WM_COMMAND:
      if (auto it = handlerToMovingLine.find((int)hwnd);
          it != handlerToMovingLine.end()) {
        MovingLine& ml = it->second;

        switch (wparam) {
          case ID_START:
            ml.SetVisible(true);
            SetTimer(hwnd, TIMER_ID, 50, nullptr);
            break;
          case ID_STOP:
            KillTimer(hwnd, TIMER_ID);
            break;
        }
      }
      break;
  }
  return DefWindowProc(hwnd, message, wparam, lparam);
}
