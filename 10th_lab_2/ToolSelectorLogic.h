#pragma once

#include <windows.h>
#include <string>

#include "ToolsState.h"

class ToolSelectorLogic 
{
public:
  explicit ToolSelectorLogic(HWND hwnd, HINSTANCE hinst);

  void CustomStringMessage(UINT message, WPARAM wparam, LPARAM lparam);

  void Command(WPARAM wparam, LPARAM lparam);
  void Resize(int width, int height);
  void Print();

public:
  void OnSendTools(HWND targetHwnd = HWND_BROADCAST) const;

private:
  tools_state::ToolsState toolsState;

  const UINT WM_TOOLS_REQUEST;

private:
  const HWND __hwnd;
  const HINSTANCE __hInst;

  HWND __redButtonHwnd;
  HWND __blueButtonHwnd;
  HWND __greenButtonHwnd;

  HWND __circleButtonHwnd;
  HWND __rectangleButtonHwnd;
  HWND __starButtonHwnd;
  HWND __rhombusButtonHwnd;

  HWND __drawCheckBoxHwnd;
};
