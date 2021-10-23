#include "FiguresPrintingLogic.h"

using namespace std;

enum IDs {
  ID_RUN,
  ID_STOP,
  ID_REPRINT_TIMER = 1000,
};

struct Positions {
  Position runBtnPosition;
  Position stopBtnPosition;
  Position printPositon;
};

Positions ComputePositions(
  int clientWidth, 
  int clientHeight
); 

FiguresPrintingLogic::FiguresPrintingLogic(
  HWND hwnd, HINSTANCE hinst
)
  : __hwnd(hwnd)
  , __hInst(hinst)
{
  RECT rc;
  GetClientRect(hwnd, &rc);
  auto cp = ComputePositions(
    rc.right - rc.left, 
    rc.bottom - rc.top
  );
  __board = std::make_shared<ModifiableBoard>(
    GetDC(hwnd), 
    cp.printPositon.w,
    cp.printPositon.h
  );
  __printer = std::make_unique<FiguresPrinter>(
    __board
  );
  __hRunWnd = CreateButton(
    cp.runBtnPosition, 
    L"Run", 
    ID_RUN
  );
  __hStopWnd = CreateButton(
    cp.stopBtnPosition, 
    L"Stop", 
    ID_STOP
  );
  ShowWindow(__hRunWnd, SW_SHOWNORMAL);
  ShowWindow(__hStopWnd, SW_SHOWNORMAL);
}

FiguresPrintingLogic::FiguresPrintingLogic(
  FiguresPrintingLogic&& fpl
)
  : __hRunWnd(NULL) 
  , __hStopWnd(NULL)
  , __hwnd(NULL)
  , __hInst(NULL)
{
  std::swap(__hRunWnd, fpl.__hRunWnd);
  std::swap(__hStopWnd, fpl.__hStopWnd);
  std::swap(__hwnd, fpl.__hwnd);
  std::swap(__hInst, fpl.__hInst);
  std::swap(__board, fpl.__board);
  std::swap(__printer, fpl.__printer);
}

FiguresPrintingLogic& 
FiguresPrintingLogic::operator =(
  FiguresPrintingLogic&& fpl
)
{
  std::swap(__hRunWnd, fpl.__hRunWnd);
  std::swap(__hStopWnd, fpl.__hStopWnd);
  std::swap(__hwnd, fpl.__hwnd);
  std::swap(__hInst, fpl.__hInst);
  std::swap(__board, fpl.__board);
  std::swap(__printer, fpl.__printer);
   
  return *this;
}
FiguresPrintingLogic::~FiguresPrintingLogic()
{
  if (__hwnd) {
    KillTimer(__hwnd, ID_REPRINT_TIMER);
  }
}
void FiguresPrintingLogic::Command(
  WPARAM wparam, LPARAM lparam
)
{
  switch (wparam) {
    case ID_RUN:
      OnRunClicked();
      break;
    case ID_STOP:
      OnStopClicked();
      break;
    default:
      break;
  }
}
void FiguresPrintingLogic::Timer(WPARAM wparam)
{
  switch (IDs(wparam)) {
    case ID_REPRINT_TIMER:
      Print();
      break;
    default:
      break;
  }
}
void SetWindowPosition(HWND hwnd, const Position &pos) {
  SetWindowPos(hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}
void FiguresPrintingLogic::Resize(
  int width, int height
)
{
  auto p = ComputePositions(width, height);
  if (__board) {
    __board->SetSize(p.printPositon.w, p.printPositon.h);
  }
  SetWindowPosition(__hRunWnd, p.runBtnPosition);
  SetWindowPosition(__hStopWnd, p.stopBtnPosition);
}
void FiguresPrintingLogic::Print()
{
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc;
  GetClientRect(__hwnd, &rc);
  const auto width = rc.right - rc.left;
  const auto height = rc.bottom - rc.top;
  HDC hdc = GetDC(__hwnd); 

  auto cp = ComputePositions(width, height);
  RECT tempDCRect{0, 0, width, height};
  PrintBoard tempDC(hdc, width, height);
  FillRect(tempDC, 
           &tempDCRect, 
           GetSysColorBrush(COLOR_WINDOW));
  if (__board) {
    auto dc = __board->GetDC();
    BitBlt(tempDC, 
           cp.printPositon.x, 
           cp.printPositon.y,
           cp.printPositon.w,
           cp.printPositon.h, 
           dc, 
           0, 
           0, 
           SRCCOPY);
  }
  BitBlt(hdc, 
         rc.left, 
         rc.top,
         width,
         height, 
         tempDC, 
         0, 
         0, 
         SRCCOPY);

  EndPaint(__hwnd, &ps);
  InvalidateRect(__hRunWnd, nullptr, TRUE);
  InvalidateRect(__hStopWnd,  nullptr, TRUE);
  UpdateWindow(__hRunWnd);
  UpdateWindow(__hStopWnd);
}

Positions ComputePositions(int clientWidth, int clientHeight) 
{
  //  _ - m         __ - m         _ - m
  // ________________________________
  // | ____________   ____________  |  | - h
  // | |           |  | RunBtn    | | |   
  // | | Area      |  \___________/ | | - btnHeight
  // | | For       |  ____________  |  | - H
  // | | Printing  |  | StopBtn   | | |
  // | \__________/   \___________/ | | - btnHeight
  // |______________________________|  | - h

  const int minWidth = 300, minHeight = 300;

  if (clientWidth < minWidth) {
    clientWidth = minWidth;
  }
  if (clientHeight < minHeight) {
    clientHeight = minHeight;
  }

  const int m = 10;
  const int h = 10;
  const int btnHeight = 50;
  const int H = clientHeight - 2 * btnHeight - 2 * h;
  const int afpHeight = clientHeight - 2 * h;
  const int afpWidth = (clientWidth - 3 * m) * 2 / 3;
  const int btnWidth = clientWidth - 3 * m - afpWidth;

  Positions pos;

  pos.printPositon.x = m;
  pos.printPositon.y = h;
  pos.printPositon.w = afpWidth;
  pos.printPositon.h = afpHeight;

  pos.runBtnPosition.x = m * 2 + afpWidth;
  pos.runBtnPosition.y = h;
  pos.runBtnPosition.w = btnWidth;
  pos.runBtnPosition.h = btnHeight;

  pos.stopBtnPosition.x = m * 2 + afpWidth;
  pos.stopBtnPosition.y = h + H + btnHeight;
  pos.stopBtnPosition.w = btnWidth;
  pos.stopBtnPosition.h = btnHeight;

  return pos;
}

void FiguresPrintingLogic::OnRunClicked()
{
  if (__printer) {
    __printer->Run();
  }
  if (__hwnd) {
    SetTimer(__hwnd, ID_REPRINT_TIMER, 60, nullptr);
  }
}

void FiguresPrintingLogic::OnStopClicked()
{
  if (__printer) {
    __printer->Stop();
  }
}

HWND FiguresPrintingLogic::CreateButton(const Position& pos, LPCWSTR name, int id) const
{
  return CreateWindow(
    L"button",
    name,
    WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_BORDER,
    pos.x, pos.y, pos.w, pos.h,
    __hwnd,
    reinterpret_cast<HMENU>(id),
    __hInst,
    nullptr
  );
}

