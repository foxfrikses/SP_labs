#include "ToolSelectorLogic.h"

#include "PaintTools.h"

using namespace tools_state;

HWND CreateRadioButton(HWND hwnd, HINSTANCE hInst, const Position &pos, 
                       LPCWSTR name, int id, bool isFirstInTheGroup = false);
HWND CreateCheckBox(HWND hwnd, HINSTANCE hInst, const Position &pos, 
                    LPCWSTR name, int id);

struct Positions {
  Position redPos;
  Position bluePos;
  Position greenPos;
  Position circlePos;
  Position rhombusPos;
  Position rectanglePos;
  Position starPos;
  Position drawPos;
};

Positions ComputePositions(int width, int height);

void SetPositions(const Positions& p,
                  HWND hred, 
                  HWND hgreen,
                  HWND hblue,
                  HWND hcircle,
                  HWND hrhombus,
                  HWND hrectangle,
                  HWND hstar,
                  HWND hdraw);


enum class TSCommand {
  ID_RED,
  ID_GREEN,
  ID_BLUE,
  ID_CIRCLE,
  ID_RHOMBUS,
  ID_RECTANGLE,
  ID_STAR,
  ID_DRAW
};

ToolSelectorLogic::ToolSelectorLogic(HWND hwnd, HINSTANCE hinst)
  : __hwnd(hwnd)
  , __hInst(hinst)
  , WM_TOOLS_REQUEST(RegisterWindowMessage(RequestToolsMessageName.c_str()))
{
  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto c = ComputePositions(rc.right - rc.left, rc.bottom - rc.top);

  __redButtonHwnd = CreateRadioButton(hwnd, hinst, c.redPos, L"Red",
                                      static_cast<int>(TSCommand::ID_RED), true);
  __blueButtonHwnd = CreateRadioButton(hwnd, hinst, c.bluePos, L"Blue",
                                       static_cast<int>(TSCommand::ID_BLUE));
  __greenButtonHwnd = CreateRadioButton(hwnd, hinst, c.greenPos, L"Green",
                                        static_cast<int>(TSCommand::ID_GREEN));
  __circleButtonHwnd = CreateRadioButton(hwnd, hinst, c.circlePos, L"Circle",
                                         static_cast<int>(TSCommand::ID_CIRCLE), true);
  __rhombusButtonHwnd = CreateRadioButton(hwnd, hinst, c.rhombusPos, L"Rhombus",
                                          static_cast<int>(TSCommand::ID_RHOMBUS));
  __rectangleButtonHwnd = CreateRadioButton(hwnd, hinst, c.rectanglePos, L"Rectangle",
                                            static_cast<int>(TSCommand:: ID_RECTANGLE));
  __starButtonHwnd = CreateRadioButton(hwnd, hinst, c.starPos, L"Star",
                                       static_cast<int>(TSCommand:: ID_STAR));
  __drawCheckBoxHwnd = CreateCheckBox(hwnd, hinst, c.drawPos, L"Draw", 
                                      static_cast<int>(TSCommand::ID_DRAW));

  ShowWindow(__redButtonHwnd, SW_SHOWNORMAL);
  ShowWindow(__blueButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__greenButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__circleButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__rhombusButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__rectangleButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__starButtonHwnd,SW_SHOWNORMAL);
  ShowWindow(__drawCheckBoxHwnd,SW_SHOWNORMAL);

  OnSendTools();
}

void ToolSelectorLogic::Command(WPARAM wparam, LPARAM lparam) {
  const auto command = static_cast<TSCommand>(wparam);
  switch (command) {
    case TSCommand::ID_RED:
      toolsState.color = SelectedColor::Red;
      break;
    case TSCommand::ID_GREEN:
      toolsState.color = SelectedColor::Green;
      break;
    case TSCommand::ID_BLUE:
      toolsState.color = SelectedColor::Blue;
      break;
    case TSCommand::ID_CIRCLE:
      toolsState.figure = SelectedFigure::Circle;
      break;
    case TSCommand::ID_RHOMBUS:
      toolsState.figure = SelectedFigure::Rhombus;
      break;
    case TSCommand::ID_RECTANGLE:
      toolsState.figure = SelectedFigure::Rectangle;
      break;
    case TSCommand::ID_STAR:
      toolsState.figure = SelectedFigure::Star;
      break;
    case TSCommand::ID_DRAW:
      switch (SendMessage(__drawCheckBoxHwnd, BM_GETCHECK, 0, 0)) {
        case BST_CHECKED:
          toolsState.mode = DrawMode::Draw;
          break;
        case BST_UNCHECKED:
          toolsState.mode = DrawMode::NotDraw;
          break;
      }
      break;
    default:
      return;
  }
  OnSendTools();
}

void ToolSelectorLogic::CustomStringMessage(UINT message, WPARAM wparam, LPARAM lparam) {
  if (message == WM_TOOLS_REQUEST) {
    OnSendTools((HWND)wparam);
  }
}

void ToolSelectorLogic::OnSendTools(HWND targetHwnd) const {
  if (targetHwnd == nullptr)
    return;

  COPYDATASTRUCT copyData;
  copyData.lpData = (PVOID)&toolsState;
  copyData.cbData = sizeof(toolsState);
  copyData.dwData = ID_MESSAGE;
  SendMessage(targetHwnd,
              WM_COPYDATA,
              (WPARAM)__hwnd,
              (LPARAM)&copyData);
}

void ToolSelectorLogic::Resize(int width, int height) {
  const auto p = ComputePositions(width, height);
}

void InvalidateAndUpdate(HWND hwnd) {
  InvalidateRect(hwnd, nullptr, TRUE);
  UpdateWindow(hwnd);
}

void ToolSelectorLogic::Print() {
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc;
  GetClientRect(__hwnd, &rc);
  FillRect(GetDC(__hwnd), &rc, GetSysColorBrush(COLOR_WINDOW));
  
  EndPaint(__hwnd, &ps);

  InvalidateAndUpdate(__redButtonHwnd);
  InvalidateAndUpdate(__greenButtonHwnd);
  InvalidateAndUpdate(__blueButtonHwnd);
  InvalidateAndUpdate(__circleButtonHwnd);
  InvalidateAndUpdate(__rhombusButtonHwnd);
  InvalidateAndUpdate(__rectangleButtonHwnd);
  InvalidateAndUpdate(__starButtonHwnd);
  InvalidateAndUpdate(__drawCheckBoxHwnd);
}

HWND CreateRadioButton(HWND hwnd, HINSTANCE hInst, const Position &pos, 
                       LPCWSTR name, int id, bool isFirstInTheGroup) 
{
  int style = WS_CHILD | BS_AUTORADIOBUTTON;
  if (isFirstInTheGroup) {
    style = style | WS_GROUP | WS_TABSTOP;
  }
  return CreateWindow(
    L"button",
    name,
    style,
    pos.x, pos.y, pos.w, pos.h,
    hwnd,
    (HMENU)(long long)(id),
    hInst,
    nullptr
  );
}

HWND CreateCheckBox(HWND hwnd, HINSTANCE hInst, const Position &pos, 
                    LPCWSTR name, int id) 
{
  return CreateWindow(
    L"button",
    name,
    WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
    pos.x, pos.y, pos.w, pos.h,
    hwnd,
    (HMENU)(long long)(id),
    hInst,
    nullptr
  );
}

void SetWindowPosition(HWND hwnd, const Position &pos) {
  SetWindowPos(hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}

void SetPositions(const Positions& p,
                  HWND hred, 
                  HWND hgreen,
                  HWND hblue,
                  HWND hcircle,
                  HWND hrhombus,
                  HWND hrectangle,
                  HWND hstar,
                  HWND hdraw) 
{
  SetWindowPosition( hred,  p.redPos );
  SetWindowPosition( hgreen, p.greenPos );
  SetWindowPosition( hblue, p.bluePos ); 
  SetWindowPosition( hcircle, p.circlePos );
  SetWindowPosition( hrhombus, p.rhombusPos );
  SetWindowPosition( hrectangle, p.rectanglePos );
  SetWindowPosition( hstar, p.starPos );
  SetWindowPosition( hdraw, p.drawPos );
}

Positions ComputePositions(int width, int height) {
  constexpr int cbHeight = 20;
  constexpr int cbWidth = 100;
  constexpr int cbMargin = 10;

  Positions p;
  p.redPos.x = cbMargin;
  p.redPos.y = cbMargin;
  p.redPos.w = cbWidth;
  p.redPos.h = cbHeight;

  p.greenPos.x = cbMargin;
  p.greenPos.y = cbMargin + p.redPos.y + p.redPos.h;
  p.greenPos.w = cbWidth;
  p.greenPos.h = cbHeight;

  p.bluePos.x = cbMargin;
  p.bluePos.y = cbMargin + p.greenPos.y + p.greenPos.h;
  p.bluePos.w = cbWidth;
  p.bluePos.h = cbHeight;

  p.circlePos.x = cbMargin + p.redPos.x + p.redPos.w;
  p.circlePos.y = cbMargin;
  p.circlePos.w = cbWidth;
  p.circlePos.h = cbHeight;

  p.rhombusPos.x = cbMargin + p.redPos.x + p.redPos.w;
  p.rhombusPos.y = cbMargin + p.circlePos.y + p.circlePos.h;
  p.rhombusPos.w = cbWidth;
  p.rhombusPos.h = cbHeight;

  p.rectanglePos.x = cbMargin + p.redPos.x + p.redPos.w;
  p.rectanglePos.y = cbMargin + p.rhombusPos.y + p.rhombusPos.h;
  p.rectanglePos.w = cbWidth;
  p.rectanglePos.h = cbHeight;

  p.starPos.x = cbMargin + p.redPos.x + p.redPos.w;
  p.starPos.y = cbMargin + p.rectanglePos.y + p.rectanglePos.h;
  p.starPos.w = cbWidth;
  p.starPos.h = cbHeight;

  p.drawPos.x = cbMargin;
  p.drawPos.y = cbMargin + max(p.starPos.y + p.starPos.w,
                               p.bluePos.y + p.bluePos.w);
  p.drawPos.w = cbWidth;
  p.drawPos.h = cbHeight;

  return p;
}
