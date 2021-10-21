#include "SWCController.h"

#include "resource.h"

enum {
  ID_TIMER
};

SWCController::SWCController(HWND hwnd, HINSTANCE hinst) 
  : __hwnd(hwnd)
  , __hInst(hinst)
{
  RECT rc;
  GetClientRect(__hwnd, &rc);
  __aimPoint = __curPoint = FloatPoint{
    ((double)rc.left + rc.right) / 2, 
    ((double)rc.top + rc.bottom) / 2
  };

  __btmps[0] = LoadBitmap(__hInst, MAKEINTRESOURCE(IDB_BITMAP_SPIDER_1));
  __btmps[1] = LoadBitmap(__hInst, MAKEINTRESOURCE(IDB_BITMAP_SPIDER_2));
  __btmps[2] = LoadBitmap(__hInst, MAKEINTRESOURCE(IDB_BITMAP_SPIDER_3));
  __btmps[3] = LoadBitmap(__hInst, MAKEINTRESOURCE(IDB_BITMAP_SPIDER_4));
}

SWCController::~SWCController() 
{
  KillTimer(__hwnd, ID_TIMER);
}

void SWCController::Timer(WPARAM wparam, LPARAM lparam) 
{
  Move();
  Print();
}

void SWCController::Resize(int width, int height) 
{
  __aimPoint.x = min(max(__aimPoint.x, 0), width);
  __aimPoint.y = min(max(__aimPoint.y, 0), height);

  __curPoint.x = min(max(__curPoint.x, 0), width);
  __curPoint.y = min(max(__curPoint.y, 0), height);

  Print();
}

void SWCController::Print() 
{
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto hdc = GetDC(__hwnd);

  HDC hBackDC = CreateCompatibleDC(hdc);
  HBITMAP hBackBmp = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
  SelectObject(hBackDC, hBackBmp);

  FillRect(hBackDC, &rc, GetSysColorBrush(COLOR_WINDOW));

  HDC hSpiderDC = CreateCompatibleDC(hBackDC);
  SelectObject(hSpiderDC, __btmps[__curBtmp]);
  StretchBlt(hBackDC , 
             int(__curPoint.x) - __btmpWidth / 2, 
             int(__curPoint.y) - __btmpHeight / 2, 
             __btmpWidth, 
             __btmpHeight, 
             hSpiderDC, 
             0, 0, __btmpWidth, __btmpHeight, 
             SRCCOPY);

  BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
         hBackDC, rc.left, rc.top, SRCCOPY);
  DeleteDC(hSpiderDC);

  DeleteObject(hBackBmp);
  DeleteDC(hBackDC);

  EndPaint(__hwnd, &ps);
}

void SWCController::SetAimPoint(int x, int y) 
{
  RECT rc;
  GetClientRect(__hwnd, &rc);

  __aimPoint.x = min(max(x, 0), rc.right);
  __aimPoint.y = min(max(y, 0), rc.bottom);

  SetTimer(__hwnd, ID_TIMER, __timerPeriod, nullptr);
}

void SWCController::Move() 
{
  __curBtmp = (__curBtmp + 1) % __btmps.size();

  const int dx = int(__aimPoint.x - __curPoint.x);
  const int dy = int(__aimPoint.y - __curPoint.y);

  if (dx == 0 && dy == 0)
  {
    KillTimer(__hwnd, ID_TIMER);
    return;
  }

  const double d = sqrt(double(dx) * dx + double(dy) * dy);

  if (__aimPoint.x != __curPoint.x) {
    const double __Dx = __D * double(dx) / d;
    if (abs(__Dx) >= abs(dx)) {
      __curPoint.x += dx;
    }
    else {
      __curPoint.x += __Dx;
    }
  }
  
  if (__aimPoint.y != __curPoint.y) {
    const double __Dy = __D * dy / d;
    if (abs(__Dy) >= abs(dy)) {
      __curPoint.y += dy;
    }
    else {
      __curPoint.y += __Dy;
    }
  }
}
