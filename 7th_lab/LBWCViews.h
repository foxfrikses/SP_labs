#pragma once

#include <windows.h>

#include <string>
#include <unordered_set>

class Position;

class LBWCViews final 
{
public:
  explicit LBWCViews(HWND hwnd, HINSTANCE hInst);
  void Resize(int width, int height);
  void Command(WPARAM wparam, LPARAM lparam);

private:
  HWND __parentHwnd;
  HINSTANCE __hInst;

  std::unordered_set<std::wstring> __leftListData;
  std::unordered_set<std::wstring> __rightListData;

  HWND __leftListHwnd;
  HWND __rightListHwnd;
  HWND __editHwnd;
  HWND __addButtonHwnd;
  HWND __clearButtonHwnd;
  HWND __deleteButtonHwnd;
  HWND __toRightButtonHwnd;

  void OnToRightClicked();
  void OnClearClicked();
  void OnDeleteClicked();
  void OnAddClicked();

  HWND CreateButton(const Position& pos, LPCWSTR name, int id) const;
  HWND CreateListBox(const Position& pos) const;
  HWND CreateEdit(const Position& pos) const;
};
