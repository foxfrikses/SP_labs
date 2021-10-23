#include "ModifiableBoard.h"

ModifiableBoard::ModifiableBoard(
  HDC targetDC, UINT width, UINT height
)
  : __board(targetDC, width, height)
  , __isCalculated(true)
{}

void ModifiableBoard::AddModifier(DCModifier modifier)
{
  std::lock_guard lock(__m);
  if (__modifiers.AddModifier(std::move(modifier))) {
    __isCalculated = false;
  }
}
void ModifiableBoard::RemoveModifier(DCModifier modifier)
{
  std::lock_guard lock(__m);
  if (__modifiers.DeleteModifier(std::move(modifier))) {
    __isCalculated = false;
  }
}

void ModifiableBoard::SetSize(UINT width, UINT height) 
{
  std::lock_guard lock(__m);
  if (width == __board.Width() && 
      height == __board.Height())
    return;

  __board.Resize(width, height);
  __isCalculated = false;
}

std::pair<UINT, UINT> ModifiableBoard::GetSize() const
{
  std::lock_guard lock(__m);
  return {__board.Width(), __board.Height()};
}

UINT ModifiableBoard::Width() const
{
  std::lock_guard lock(__m);
  return __board.Width();
}
UINT ModifiableBoard::Height() const
{
  std::lock_guard lock(__m);
  return __board.Height();
}

ModifiableBoard::HDCHandler::HDCHandler(
  std::mutex& m, 
  const ExpandableWhitePrintBoard& board
) 
  : __lock(m)
{
  __hdc = board; 
}

void ModifiableBoard::Calcualate()
{
  std::lock_guard lock(__m);
  if (__isCalculated) {
    return;
  }
  __board.Clear();
  for (auto modifier : __modifiers) {
    modifier->Modify(__board);
  }
}

ModifiableBoard::HDCHandler::operator HDC() const 
{
  return __hdc;
}

ModifiableBoard::HDCHandler 
ModifiableBoard::GetDC()
{
  Calcualate();
  return HDCHandler(__m, __board);
}
