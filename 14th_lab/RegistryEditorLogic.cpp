#include "RegistryEditorLogic.h"

#include "common.h"
#include "PaintTools.h"

#include <algorithm>

struct Positions {
  Position keyTreePos; 
  Position valuesListPos; 
};

enum IDs
{
};

Positions ComputePositions(int width, int height);

RegistryEditorLogic::RegistryEditorLogic(
  HWND hwnd, 
  HINSTANCE hInst
)
  : __hwnd(hwnd)
  , __hInst(hInst)
{
  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto p = ComputePositions(rc.right - rc.left,
                            rc.bottom - rc.top);


  //ShowWindow(__hProcessesList, SW_SHOWNORMAL);
  //ShowWindow(__hModuleList, SW_SHOWNORMAL);
}

void RegistryEditorLogic::Command(WPARAM wparam, LPARAM lparam)
{
  int id = LOWORD(wparam);
  int eventId = HIWORD(wparam);

  switch (id) {

    default:
      break;
  }
}

void RegistryEditorLogic::ContextMenu(HWND hwnd, int x, int y)
{
}

void SetWindowPosition(HWND hwnd, const Position &pos) {
  SetWindowPos(hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}

void RegistryEditorLogic::Resize(int width, int height)
{
  auto p = ComputePositions(width, height);
  //SetWindowPosition(__hProcessesList, p.keyTreePos);
  //SetWindowPosition(__hModuleList, p.valuesListPos);
  Print();
}

void RegistryEditorLogic::Print()
{
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc;
  GetClientRect(__hwnd, &rc);
  FillRect(GetDC(__hwnd), &rc, 
           GetSysColorBrush(COLOR_WINDOW));

  EndPaint(__hwnd, &ps);
}

Positions ComputePositions(int clientWidth, int clientHeight)
{
  //  _ - m         __ - m         _ - m
  // ________________________________
  // | ____________   ____________  |  | - m
  // | |           |  |           | | | 
  // | |           |  |           | | |
  // | |processesLb|  | modulesLb | | | - lbHeight
  // | |           |  |           | | |
  // | \__________/   \___________/ | |
  // |______________________________|  | - m

  constexpr int minWidth = 300, minHeight = 400;
  constexpr int m = 10;

  if (clientWidth < minWidth) {
    clientWidth = minWidth;
  }
  if (clientHeight < minHeight) {
    clientHeight = minHeight;
  }

  int lbWidth = (clientWidth - m * 3) / 2;
  int lbHeight = clientHeight - m * 2;

  Positions p;

  p.keyTreePos.x = m;
  p.keyTreePos.y = m;
  p.keyTreePos.w = lbWidth;
  p.keyTreePos.h = lbHeight;

  p.valuesListPos.x = m * 2 + lbWidth;
  p.valuesListPos.y = m;
  p.valuesListPos.w = lbWidth;
  p.valuesListPos.h = lbHeight;

  return p;
}
