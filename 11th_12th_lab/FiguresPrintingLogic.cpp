#include "FiguresPrintingLogic.h"

#include "WinCriticalSectionMutex.h"
#include "WinSemaphoreMutex.h"
#include "WinMutexMutex.h"
#include <mutex>

using namespace std;

enum IDs {
  ID_RUN,
  ID_SUSPEND,
  ID_STOP,

  ID_STD_MUTEX,
  ID_MUTEX_MUTEX,
  ID_SEMAPHORE_MUTEX,
  ID_CRITICAL_SECTION_MUTEX,

  ID_REPRINT_TIMER = 1000,
};

struct Positions {
  Position stdMutexPosition;
  Position mutMutexPosition;
  Position crsMutexPosition;
  Position semMutexPosition;
  Position runBtnPosition;
  Position suspendBtnPosition;
  Position stopBtnPosition;
  Position printPositon;
};

Positions ComputePositions(
  int clientWidth, 
  int clientHeight
); 

std::unique_ptr<IModifiableBoard> GetBoard(
  MutexType type, 
  HDC hdc,
  int width,
  int height
);

FiguresPrintingLogic::FiguresPrintingLogic(
  HWND hwnd, HINSTANCE hinst
)
  : __hwnd(hwnd)
  , __hInst(hinst)
  , __mutexType(MutexType::stdMutex)
{
  RECT rc;
  GetClientRect(hwnd, &rc);
  auto cp = ComputePositions(
    rc.right - rc.left, 
    rc.bottom - rc.top
  );
  __board = GetBoard(
    __mutexType,
    GetDC(__hwnd),
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
  __hSuspendWnd = CreateButton(
    cp.suspendBtnPosition, 
    L"Suspend", 
    ID_SUSPEND
  );
  __hStopWnd = CreateButton(
    cp.stopBtnPosition, 
    L"Stop", 
    ID_STOP
  );
  __hStdWnd = CreateRadioButton(
    cp.stdMutexPosition, 
    L"std::mutex", 
    ID_STD_MUTEX,
    true
  );
  __hMutWnd = CreateRadioButton(
    cp.mutMutexPosition, 
    L"Mutex", 
    ID_MUTEX_MUTEX,
    false
  );
  __hSemWnd = CreateRadioButton(
    cp.semMutexPosition, 
    L"Semaphore", 
    ID_SEMAPHORE_MUTEX,
    false
  );
  __hCrsWnd = CreateRadioButton(
    cp.crsMutexPosition, 
    L"CriticalSection", 
    ID_CRITICAL_SECTION_MUTEX,
    false
  );
  CheckRadioButton(
    __hwnd,
    ID_STD_MUTEX,
    ID_CRITICAL_SECTION_MUTEX,
    ID_STD_MUTEX
  );
  ShowWindow(__hRunWnd, SW_SHOWNORMAL);
  ShowWindow(__hSuspendWnd, SW_SHOWNORMAL);
  ShowWindow(__hStopWnd, SW_SHOWNORMAL);
  ShowWindow(__hStdWnd, SW_SHOWNORMAL);
  ShowWindow(__hMutWnd, SW_SHOWNORMAL);
  ShowWindow(__hSemWnd, SW_SHOWNORMAL);
  ShowWindow(__hCrsWnd, SW_SHOWNORMAL);
  SetTimer(__hwnd, ID_REPRINT_TIMER, 1000 / 60, nullptr);
}

FiguresPrintingLogic::FiguresPrintingLogic(
  FiguresPrintingLogic&& fpl
)
  : __hRunWnd(NULL) 
  , __hSuspendWnd(NULL)
  , __hStopWnd(NULL)
  , __hStdWnd(NULL)
  , __hMutWnd(NULL)
  , __hSemWnd(NULL)
  , __hCrsWnd(NULL)
  , __hwnd(NULL)
  , __hInst(NULL)
{
  *this = std::move(fpl);
}

FiguresPrintingLogic& 
FiguresPrintingLogic::operator =(
  FiguresPrintingLogic&& fpl
)
{
  std::swap(__hRunWnd, fpl.__hRunWnd);
  std::swap(__hSuspendWnd, fpl.__hSuspendWnd);
  std::swap(__hStopWnd, fpl.__hStopWnd);

  std::swap(__hStdWnd, fpl.__hStdWnd);
  std::swap(__hMutWnd, fpl.__hMutWnd);
  std::swap(__hSemWnd, fpl.__hSemWnd);
  std::swap(__hCrsWnd, fpl.__hCrsWnd);

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
    case ID_SUSPEND:
      OnSuspendClicked();
      break;
    case ID_STOP:
      OnStopClicked();
      break;
    case ID_STD_MUTEX:
      SetMutexType(MutexType::stdMutex);
      break;
    case ID_MUTEX_MUTEX:
      SetMutexType(MutexType::winMutex);
      break;
    case ID_SEMAPHORE_MUTEX:
      SetMutexType(MutexType::winSemaphore);
      break;
    case ID_CRITICAL_SECTION_MUTEX:
      SetMutexType(MutexType::winCriticalSection);
      break;
    default:
      break;
  }
}

void FiguresPrintingLogic::Timer(WPARAM wparam)
{
  switch (IDs(wparam)) {
    case ID_REPRINT_TIMER:
      PrintPicture();
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
  SetWindowPosition(__hStdWnd, p.stdMutexPosition);
  SetWindowPosition(__hMutWnd, p.mutMutexPosition);
  SetWindowPosition(__hSemWnd, p.semMutexPosition);
  SetWindowPosition(__hCrsWnd, p.crsMutexPosition);
  SetWindowPosition(__hRunWnd, p.runBtnPosition);
  SetWindowPosition(__hSuspendWnd, p.suspendBtnPosition);
  SetWindowPosition(__hStopWnd, p.stopBtnPosition);
}

void FiguresPrintingLogic::SetMutexType(MutexType type)
{
  if (__mutexType == type) {
    return;
  }

  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto cp = ComputePositions(
    rc.right - rc.left, 
    rc.bottom - rc.top
  );

  __board = GetBoard(
    type, 
    GetDC(__hwnd), 
    cp.printPositon.w, 
    cp.printPositon.h
  );

  __printer = std::make_unique<FiguresPrinter>(__board);
  __mutexType = type;
}

void FiguresPrintingLogic::PrintPicture()
{
  if (__board) {
    PAINTSTRUCT ps;
    BeginPaint(__hwnd, &ps);

    RECT rc;
    GetClientRect(__hwnd, &rc);
    const auto width = rc.right - rc.left;
    const auto height = rc.bottom - rc.top;
    HDC hdc = GetDC(__hwnd); 

    auto cp = ComputePositions(width, height);
    auto dc = __board->GetDC();
    BitBlt(hdc, 
           cp.printPositon.x, 
           cp.printPositon.y,
           cp.printPositon.w,
           cp.printPositon.h, 
           *dc, 
           0, 
           0, 
           SRCCOPY);
    
    EndPaint(__hwnd, &ps);
  }
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
           *dc, 
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
}

Positions ComputePositions(int clientWidth, int clientHeight) 
{
  //  _ - m         __ - m         _ - m
  // ________________________________
  // | ____________                 |  | - h
  // | |           |  |V| stdMut    | | - cbHeight
  // | |           |                |  | - cbm  
  // | |           |  | | mutMut    | | - cbHeight
  // | |           |                |  | - cbm  
  // | |           |  | | crsMut    | | - cbHeight
  // | |           |                |  | - cbm  
  // | | Area      |  | | semMut    | | - cbHeight
  // | | For       |  ____________  |  | - H
  // | | Printing  |  | RunBtn    | | | - btnHeight  
  // | |           |  \___________/ | |   
  // | |           |  ____________  |  | - h
  // | |           |  | SuspendBtn| | | - btnHeight
  // | |           |  \___________/ | |
  // | |           |  ____________  |  | - h
  // | |           |  | StopBtn   | | | - btnHeight
  // | \__________/   \___________/ | |
  // |______________________________|  | - h

  const int minWidth = 300, minHeight = 400;

  if (clientWidth < minWidth) {
    clientWidth = minWidth;
  }
  if (clientHeight < minHeight) {
    clientHeight = minHeight;
  }

  constexpr int m = 10;
  constexpr int h = 10;
  constexpr int cbm = 10;
  constexpr int btnHeight = 50;
  constexpr int cbHeight = 20;
  const int H = (clientHeight - 
                 btnHeight * 3 - 
                 cbHeight * 4 - 
                 cbm * 3 -
                 4 * h);
  const int afpHeight = clientHeight - 2 * h;
  const int afpWidth = (clientWidth - 3 * m) * 2 / 3;
  const int btnWidth = clientWidth - 3 * m - afpWidth;
  const int cbWidth = btnWidth;

  Positions pos;

  pos.printPositon.x = m;
  pos.printPositon.y = h;
  pos.printPositon.w = afpWidth;
  pos.printPositon.h = afpHeight;

  pos.stdMutexPosition.x = m * 2 + afpWidth;
  pos.stdMutexPosition.y = h;
  pos.stdMutexPosition.w = cbWidth;
  pos.stdMutexPosition.h = cbHeight;

  pos.mutMutexPosition.x = m * 2 + afpWidth;
  pos.mutMutexPosition.y = h + cbm * 1 + cbHeight * 1;
  pos.mutMutexPosition.w = cbWidth;
  pos.mutMutexPosition.h = cbHeight;

  pos.crsMutexPosition.x = m * 2 + afpWidth;
  pos.crsMutexPosition.y = h + cbm * 2 + cbHeight * 2;
  pos.crsMutexPosition.w = cbWidth;
  pos.crsMutexPosition.h = cbHeight;

  pos.semMutexPosition.x = m * 2 + afpWidth;
  pos.semMutexPosition.y = h + cbm * 3 + cbHeight * 3;
  pos.semMutexPosition.w = cbWidth;
  pos.semMutexPosition.h = cbHeight;

  pos.runBtnPosition.x = m * 2 + afpWidth;
  pos.runBtnPosition.y = h + cbm * 3 + cbHeight * 4 + H;
  pos.runBtnPosition.w = btnWidth;
  pos.runBtnPosition.h = btnHeight;

  pos.suspendBtnPosition.x = m * 2 + afpWidth;
  pos.suspendBtnPosition.y = h * 2 + cbm * 3 + cbHeight * 4 + H + btnHeight;
  pos.suspendBtnPosition.w = btnWidth;
  pos.suspendBtnPosition.h = btnHeight;

  pos.stopBtnPosition.x = m * 2 + afpWidth;
  pos.stopBtnPosition.y = h * 3 + cbm * 3 + cbHeight * 4 + H + btnHeight * 2;
  pos.stopBtnPosition.w = btnWidth;
  pos.stopBtnPosition.h = btnHeight;

  return pos;
}

void FiguresPrintingLogic::OnRunClicked()
{
  if (__printer) {
    __printer->Run();
  }
}

void FiguresPrintingLogic::OnSuspendClicked()
{
  if (__printer) {
    __printer->Suspend();
  }
}

void FiguresPrintingLogic::OnStopClicked()
{
  if (__printer) {
    __printer->Stop();
  }
}

HWND FiguresPrintingLogic::CreateButton(
  const Position& pos, 
  LPCWSTR name, 
  int id
) const
{
  return CreateWindow(
    L"button",
    name,
    WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | WS_TABSTOP,
    pos.x, pos.y, pos.w, pos.h,
    __hwnd,
    reinterpret_cast<HMENU>(id),
    __hInst,
    nullptr
  );
}

HWND FiguresPrintingLogic::CreateRadioButton(
  const Position &pos, 
  LPCWSTR name, 
  int id, 
  bool isFirstInTheGroup
) const
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
    __hwnd,
    (HMENU)(long long)(id),
    __hInst,
    nullptr
  );
}

std::unique_ptr<IModifiableBoard> GetBoard(
  MutexType type,
  HDC hdc,
  int width,
  int height
)
{
  switch (type) {
    case MutexType::stdMutex:
      return std::make_unique<ModifiableBoard<std::mutex>>(
        hdc, width, height   
      );
    case MutexType::winMutex:
      return std::make_unique<ModifiableBoard<WinMutexMutex>>(
        hdc, width, height   
      );
    case MutexType::winSemaphore:
      return std::make_unique<ModifiableBoard<WinSemaphoreMutex>>(
        hdc, width, height   
      );
    case MutexType::winCriticalSection:
      return std::make_unique<ModifiableBoard<WinCriticalSectionMutex>>(
        hdc, width, height   
      );
    default:
      return std::make_unique<ModifiableBoard<std::mutex>>(
        hdc, width, height   
      );
  }
}
