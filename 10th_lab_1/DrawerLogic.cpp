#include "DrawerLogic.h"
 
#include "DCEllipsePrinter.h"
#include "DCRectanglePrinter.h"
#include "DCRhombusPrinter.h"
#include "DCStarPrinter.h"

#include "PaintTools.h"

#include <memory>

using namespace tools_state;

COLORREF SelectedColorToRGB(tools_state::SelectedColor color)
{
  switch (color) {
    case tools_state::SelectedColor::Red:
      return RGB(240, 20, 60);
    case tools_state::SelectedColor::Blue:
      return RGB(177, 255, 250);
    case tools_state::SelectedColor::Green:
      return RGB(30, 180, 50);
    default:
      return RGB(0, 0, 0);
  }
}

DCModifier ToolToModifier(const tools_state::ToolsState& ts)
{
  if (ts.mode != tools_state::DrawMode::Draw ||
      ts.color == tools_state::SelectedColor::None ||
      ts.figure == tools_state::SelectedFigure::None)
    return nullptr;

  std::unique_ptr<IDCPrinter> p;
  
  switch (ts.figure) {
    case tools_state::SelectedFigure::Circle:
      p = std::make_unique<DCEllipsePrinter>();
      break;

    case tools_state::SelectedFigure::Rectangle:
      p = std::make_unique<DCRectanglePrinter>();
      break;

    case tools_state::SelectedFigure::Rhombus:
      p = std::make_unique<DCRhombusPrinter>();
      break;

    case tools_state::SelectedFigure::Star:
      p = std::make_unique<DCStarPrinter>();
      break;

    default:
      return nullptr;
  }

  p->SetPenColor(BLACK);
  p->SetBrushColor(SelectedColorToRGB(ts.color));

  return p;
}


DrawerLogic::DrawerLogic(HWND hwnd, 
                         HINSTANCE hinst)
  : __hwnd(hwnd)
  , __hInst(hinst)
  , WM_TOOLS_REQUEST(
      RegisterWindowMessage(
        tools_state::RequestToolsMessageName.c_str()
      )
    )
  , __board([&hwnd]{
    RECT rc;
    GetClientRect(hwnd, &rc);
    return ModifiableBoard(GetDC(hwnd),
                           rc.right - rc.left,
                           rc.bottom - rc.top);
  }())
{
  OnRequestTools();
  Print();
}

void DrawerLogic::OnRequestTools(HWND targetHwnd) const {
  if (targetHwnd == nullptr)
    return;

  PostMessage(targetHwnd,
              WM_TOOLS_REQUEST,
              (WPARAM)__hwnd,
              (LPARAM)0);
}

void DrawerLogic::Resize(int width, int height) {
  __board.SetSize(width, height);
  Print();
}

void DrawerLogic::CopyData(WPARAM wparam, LPARAM lparam) {
  auto data = reinterpret_cast<PCOPYDATASTRUCT>(lparam);
  if (data->dwData == ID_MESSAGE) {
    __toolsState = *reinterpret_cast<ToolsState*>(data->lpData);
  }
}

void DrawerLogic::OnMousePressed(UINT x, UINT y)
{
  if (!__isPrintingStarted) {
    __isPrintingStarted = true;

    auto modifier = ToolToModifier(__toolsState);
    if (!modifier) {
      __isPrintingStarted = false;
      return;
    }
    modifier->SetBeginPoint(POINT{(LONG)x, (LONG)y});
    modifier->SetEndPoint(POINT{(LONG)x, (LONG)y});
    __board.SetActiveModifier(std::move(modifier));
    Print();
  }
  else OnMouseRelease(x, y);
}

void DrawerLogic::OnMouseMove(UINT x, UINT y)
{
  if (__isPrintingStarted) {
    if (auto modifier = __board.GetActiveModifier()) {
      modifier->SetEndPoint(POINT{(LONG)x, (LONG)y});
      __board.SetActiveModifier(modifier);
    }
    Print();
  }
}

void DrawerLogic::OnMouseRelease(UINT x, UINT y)
{
  if (__isPrintingStarted) {
    if (auto modifier = __board.GetActiveModifier()) {
      modifier->SetEndPoint(POINT{(LONG)x, (LONG)y});
      __board.SaveActiveModifier();
    }
    __isPrintingStarted = false;
    Print();
  }
}

void DrawerLogic::Print() {
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc; 
  GetClientRect(__hwnd, &rc);
  __board.SetSize(rc.right - rc.left, rc.bottom - rc.top);

  BitBlt(GetDC(__hwnd), 
         0, 
         0, 
         __board.Width(), 
         __board.Height(), 
         __board, 
         0, 
         0, 
         SRCCOPY);

  EndPaint(__hwnd, &ps);
}