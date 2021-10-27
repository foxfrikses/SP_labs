#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <optional>

#include "PriorityManagerLogic.h"
#include "PaintTools.h"
#include "common.h"

class PriorityManagerLogic final
{
public:
  explicit PriorityManagerLogic(HWND hwnd, HINSTANCE hinst);

  PriorityManagerLogic(const PriorityManagerLogic&) = delete;
  PriorityManagerLogic& operator =(const PriorityManagerLogic&) = delete;
  PriorityManagerLogic(PriorityManagerLogic&&) = default;
  PriorityManagerLogic& operator =(PriorityManagerLogic&&) = default;

  void Command(WPARAM wparam, LPARAM lparam);
  void ContextMenu(HWND hwnd, int x, int y);
  void Resize(int width, int height);
  void Print();

private:
  HWND __hProcessesList;
  HWND __hModuleList;

  void UpdateProcessList();
  void UpdateModuleList(DWORD processId);

  std::optional<int> __contextMenuProcessIndex;
  bool LoadContextMenuProcess(int x, int y);
  std::optional<DWORD> GetSelectedProcessId() const;
  std::optional<Handle> GetContexMenuProcess() const;
  std::wstring GetContexMenuProcessName() const;

private:
  HWND __hwnd;
  HINSTANCE __hInst;
};