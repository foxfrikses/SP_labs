#pragma once

#include <windows.h>

#include <string>

#include "ModifiableBoard.h"
#include "ToolsState.h"

class DrawerLogic 
{
  ModifiableBoard __board;

public:
  explicit DrawerLogic(HWND hwnd, HINSTANCE hinst);

  void CopyData(WPARAM wparam, LPARAM lparam);
  void Resize(int width, int height);
  void Print();
  void OnMousePressed(UINT x, UINT y);
  void OnMouseMove(UINT x, UINT y);
  void OnMouseRelease(UINT x, UINT y);

public:
  void OnRequestTools(HWND targetHwnd = HWND_BROADCAST) const;

private:
  tools_state::ToolsState __toolsState;
  bool __isPrintingStarted = false;

private:
  const UINT WM_TOOLS_REQUEST;

private:
  const HWND __hwnd;
  const HINSTANCE __hInst;
};
