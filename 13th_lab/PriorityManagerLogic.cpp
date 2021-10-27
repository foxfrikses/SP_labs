#include "PriorityManagerLogic.h"

#include "tlhelp32.h"

#include <algorithm>
#include <iterator>
#include <numeric>

struct Positions {
  Position procesesPosition; 
  Position modulesPosition; 
};

enum IDs
{
  ID_PROCESSES,
  ID_MODULES,

  ID_UPDATE_PROCESSES_MENU_ITEM,

  ID_IDLE_MENU_ITEM,
  ID_BELOW_NORMAL_MENU_ITEM,
  ID_NORMAL_MENU_ITEM,
  ID_ABOVE_NORMAL_MENU_ITEM,
  ID_HIGH_MENU_ITEM,
  ID_REALTIME_MENU_ITEM,
};

Positions ComputePositions(int width, int height);
HWND CreateListBox(
  const Position&, LPCWSTR, int, HWND, HINSTANCE
);

inline static const bool __isSystem64 = [] {
  SYSTEM_INFO si;
  GetNativeSystemInfo(&si);
  return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64;
}();

PriorityManagerLogic::PriorityManagerLogic(
  HWND hwnd, 
  HINSTANCE hInst
)
  : __hwnd(hwnd)
  , __hInst(hInst)
  , __popupMenu(NULL)
{
  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto p = ComputePositions(rc.right - rc.left,
                            rc.bottom - rc.top);
  __hProcessesList = CreateListBox(
    p.procesesPosition,
    L"Processes",
    ID_PROCESSES,
    __hwnd,
    __hInst
  );
  __hModuleList = CreateListBox(
    p.modulesPosition,
    L"Modules",
    ID_MODULES,
    __hwnd,
    __hInst
  );
  UpdateProcessList();
  ShowWindow(__hProcessesList, SW_SHOWNORMAL);
  ShowWindow(__hModuleList, SW_SHOWNORMAL);
}

void PriorityManagerLogic::Command(WPARAM wparam, LPARAM lparam)
{
  int id = LOWORD(wparam);
  int eventId = HIWORD(wparam);

  switch (id) {
    case ID_PROCESSES:
      switch (eventId) {
        case LBN_SELCHANGE:
          if (auto id = GetSelectedProcessId(); id.has_value()) {
            UpdateModuleList(id.value());
          }
          break;

        default:
          break;
      }
      break;

    case ID_UPDATE_PROCESSES_MENU_ITEM:
      UpdateProcessList();
      break;

    case ID_IDLE_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), IDLE_PRIORITY_CLASS);
      }
      break;

    case ID_BELOW_NORMAL_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), BELOW_NORMAL_PRIORITY_CLASS);
      }
      break;

    case ID_NORMAL_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), NORMAL_PRIORITY_CLASS);
      }
      break;

    case ID_ABOVE_NORMAL_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), ABOVE_NORMAL_PRIORITY_CLASS);
      }
      break;

    case ID_HIGH_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), HIGH_PRIORITY_CLASS);
      }
      break;

    case ID_REALTIME_MENU_ITEM:
      if (auto p = GetContexMenuProcess(); p.has_value()) {
        SetPriorityClass(p.value(), REALTIME_PRIORITY_CLASS);
      }
      break;

    default:
      break;
  }
}

void PriorityManagerLogic::ContextMenu(HWND hwnd, int x, int y)
{
  __popupMenu = Menu(CreatePopupMenu());
  AppendMenu(__popupMenu, MF_STRING, ID_UPDATE_PROCESSES_MENU_ITEM, L"Refresh processes");

  if (hwnd == __hProcessesList && LoadContextMenuProcess(x, y)) {
    if (auto process = GetContexMenuProcess();
        process.has_value() && process.value()) {

      AppendMenu(__popupMenu, MF_SEPARATOR, 0, nullptr);
      auto m2 = CreatePopupMenu();
      auto name = GetContexMenuProcessName();
      AppendMenu(__popupMenu, MF_POPUP, (DWORD)m2, name.c_str());
        
      int p = GetPriorityClass(process.value());

      auto idleMf = p == IDLE_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;
      auto bNormalMf = p == BELOW_NORMAL_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;
      auto normalMf = p == NORMAL_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;
      auto aNormalMf = p == ABOVE_NORMAL_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;
      auto highMf = p == HIGH_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;
      auto realTimeMf = p == REALTIME_PRIORITY_CLASS ? 
        MF_CHECKED : MF_STRING;

      AppendMenu(m2, idleMf,     ID_IDLE_MENU_ITEM,         L"Idle");
      AppendMenu(m2, bNormalMf,  ID_BELOW_NORMAL_MENU_ITEM, L"Below normal");
      AppendMenu(m2, normalMf,   ID_NORMAL_MENU_ITEM,       L"Normal");
      AppendMenu(m2, aNormalMf,  ID_ABOVE_NORMAL_MENU_ITEM, L"Above normal");
      AppendMenu(m2, highMf,     ID_HIGH_MENU_ITEM,         L"High");
      AppendMenu(m2, realTimeMf, ID_REALTIME_MENU_ITEM,     L"Realtime");
    }
  }

  TrackPopupMenu(__popupMenu, 
                 TPM_TOPALIGN | TPM_LEFTALIGN, 
                 x, y, 0, __hwnd, NULL);
}

void SetWindowPosition(HWND hwnd, const Position &pos) {
  SetWindowPos(hwnd, 0, pos.x, pos.y, pos.w, pos.h, 0);
}

void PriorityManagerLogic::Resize(int width, int height)
{
  auto p = ComputePositions(width, height);
  SetWindowPosition(__hProcessesList, p.procesesPosition);
  SetWindowPosition(__hModuleList, p.modulesPosition);
  Print();
}

void PriorityManagerLogic::Print()
{
  PAINTSTRUCT ps;
  BeginPaint(__hwnd, &ps);

  RECT rc;
  GetClientRect(__hwnd, &rc);
  auto p = ComputePositions(rc.right - rc.left,
                            rc.bottom - rc.top);
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

  p.procesesPosition.x = m;
  p.procesesPosition.y = m;
  p.procesesPosition.w = lbWidth;
  p.procesesPosition.h = lbHeight;

  p.modulesPosition.x = m * 2 + lbWidth;
  p.modulesPosition.y = m;
  p.modulesPosition.w = lbWidth;
  p.modulesPosition.h = lbHeight;

  return p;
}

HWND CreateListBox(
  const Position& pos,
  LPCWSTR name,
  int id,
  HWND hwnd,
  HINSTANCE hinst
) 
{
  return CreateWindow(
    L"listbox", 
    name, 
    WS_CHILD | WS_VISIBLE | WS_HSCROLL |
    WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_NOREDRAW,
    pos.x, pos.y, pos.w, pos.h, 
    hwnd, 
    (HMENU)id, 
    hinst, 
    NULL);
}

using NameIdList = std::vector<std::pair<std::wstring, DWORD>>;
using HandleOpt = std::optional<Handle>;

NameIdList LoadProcesses()
{
  PROCESSENTRY32 p;
  p.dwSize = sizeof(PROCESSENTRY32);

  auto toolHelp = Handle(
    CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
  );

  NameIdList niList;
  if (Process32First(toolHelp, &p)) {
    do {
      niList.emplace_back(p.szExeFile, p.th32ProcessID);
    } while (Process32Next(toolHelp, &p));
  }
  return niList;
}

// <is32For64System == true>
// for win64 - 32 modules
// <is32For64System == false>
// for win32 - 32 modules
// for win64 - 64 modules
NameIdList LoadModules(DWORD processId, bool is32For64System)
{
  auto mode = is32For64System ? TH32CS_SNAPMODULE32 : TH32CS_SNAPMODULE;

  MODULEENTRY32 m;
  m.dwSize = sizeof(MODULEENTRY32);

  auto res = CreateToolhelp32Snapshot(mode, processId);
  while ((long)res == ERROR_BAD_LENGTH) {
    res = CreateToolhelp32Snapshot(mode, processId);
  }
  if (res == INVALID_HANDLE_VALUE) {
    return {};
  }
  auto toolHelp = Handle(res);

  NameIdList niList;
  if (Module32First(toolHelp, &m)) {
    do {
      niList.emplace_back(m.szModule, m.th32ModuleID);
    } while (Module32Next(toolHelp, &m));
  }
  return niList;
}

void UpdateList(HWND list, const NameIdList& data)
{
  SendMessage(list, WM_SETREDRAW, FALSE, 0);
  SendMessage(list, LB_RESETCONTENT, 0, 0);

  const size_t stringLength = std::accumulate(
    data.begin(), 
    data.end(), 
    0llu,
    [] (size_t f, typename NameIdList::value_type s) {
      return max(f, s.first.size());
    }
  );

  SendMessage(list, LB_INITSTORAGE, data.size(), stringLength + 1);
 
  for (auto& [name, id] : data) {
    int i = SendMessage(list, LB_INSERTSTRING, -1, (LPARAM)name.c_str());
    SendMessage(list, LB_SETITEMDATA , i, id);
  }
  SendMessage(list, WM_SETREDRAW, TRUE, 0);
}

void PriorityManagerLogic::UpdateProcessList()
{
  auto processes = LoadProcesses();
  std::sort(processes.begin(), processes.end());
  UpdateList(__hProcessesList, processes);
  UpdateList(__hModuleList, {});

  RedrawWindow(__hModuleList, NULL, NULL, RDW_ERASE | 
               RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
  RedrawWindow(__hProcessesList, NULL, NULL, RDW_ERASE | 
               RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void PriorityManagerLogic::UpdateModuleList(DWORD processId)
{
  auto modules = LoadModules(processId, false);
  if (__isSystem64) {
    auto modules32 = LoadModules(processId, true);
    modules.reserve(modules.size() + modules32.size());
    std::move(modules32.begin(), 
              modules32.end(),
              std::back_inserter(modules));
  }
  UpdateList(__hModuleList, modules);

  RedrawWindow(__hModuleList, NULL, NULL, RDW_ERASE | 
               RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
  RedrawWindow(__hProcessesList, NULL, NULL, RDW_ERASE | 
               RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

std::optional<DWORD> PriorityManagerLogic::GetSelectedProcessId() const
{
  int index = SendMessage(__hProcessesList, LB_GETCURSEL, 0, 0);
  if (index < 0) {
    return std::nullopt;
  }
  return SendMessage(__hProcessesList, LB_GETITEMDATA, index, 0);
}

std::optional<Handle> PriorityManagerLogic::GetContexMenuProcess() const
{
  if (__contextMenuProcessIndex.has_value()) {
    auto id = SendMessage(__hProcessesList, 
                          LB_GETITEMDATA, 
                          __contextMenuProcessIndex.value(), 
                          0);

    return Handle(OpenProcess(PROCESS_SET_INFORMATION | 
                              PROCESS_QUERY_INFORMATION, 
                              FALSE, 
                              id));
  }
  else {
    return std::nullopt;
  }
}

std::wstring PriorityManagerLogic::GetContexMenuProcessName() const
{
  if (!__contextMenuProcessIndex.has_value()) 
    return {};

  auto len = SendMessage(__hProcessesList, 
                         LB_GETTEXTLEN, 
                         __contextMenuProcessIndex.value(), 
                         0);
  std::wstring str;
  str.resize(len + 1);
  SendMessage(__hProcessesList, 
              LB_GETTEXT, 
              __contextMenuProcessIndex.value(), 
              (LPARAM)str.c_str());
  str.resize(len);
  return str;
}

bool PriorityManagerLogic::LoadContextMenuProcess(int x, int y)
{
  POINT p = {x, y};
  ScreenToClient(__hProcessesList, &p);
  DWORD res = SendMessage(__hProcessesList, LB_ITEMFROMPOINT, 0, MAKELPARAM(p.x, p.y));
  if (HIWORD(res) == 1) {
    __contextMenuProcessIndex.reset();
  }
  else {
    __contextMenuProcessIndex = LOWORD(res);
  }
      
  return __contextMenuProcessIndex.has_value();
}

