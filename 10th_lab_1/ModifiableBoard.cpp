#include "ModifiableBoard.h"

#include "DCEllipsePrinter.h"

ModifiableBoard::ModifiableBoard(HDC targetDC, UINT width, UINT height) 
  : __board(targetDC, width, height)
  , __activeBoard(targetDC, width, height)
{
  RecalcualteActive();
}

void ModifiableBoard::SetActiveModifier(DCModifier modifier) {
  if (modifier != __activeModifier) {
    __activeModifier = std::move(modifier);
  }
  RecalcualteActive();
}

void ModifiableBoard::SaveActiveModifier() {
  if (__activeModifier) {
    __activeModifier->Modify(__board);
    __activeModifier.reset();
  }
}

void ModifiableBoard::SetSize(UINT width, UINT height) {
  if (width == Width() && height == Height())
    return;

  __board.Resize(width, height);
  __activeBoard.Resize(width, height);
  RecalcualteActive();
}

void ModifiableBoard::RecalcualteActive() {
  BitBlt(__activeBoard, 
         0, 
         0, 
         __activeBoard.Width(), 
         __activeBoard.Height(), 
         __board, 
         0, 
         0, 
         SRCCOPY);

  if (__activeModifier) {
    __activeModifier->Modify(__activeBoard);
  }
}
