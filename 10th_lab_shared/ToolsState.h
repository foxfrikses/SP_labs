#pragma once

#pragma once

#include <windows.h>
#include <string>

namespace tools_state
{
  enum class DrawMode 
  {
    None, 
    Draw, 
    NotDraw
  };

  enum class SelectedColor {
    None, 
    Red, 
    Blue, 
    Green
  };

  enum class SelectedFigure {
    None, 
    Circle, 
    Rectangle, 
    Star, 
    Rhombus
  };

  static constexpr ULONG ID_MESSAGE = 1155;

  static const std::wstring RequestToolsMessageName = 
    L"RequestToolsMessage::10_lab";


  struct ToolsState {
    DrawMode mode;
    SelectedColor color;
    SelectedFigure figure;
  };
}


