#pragma once

#include "IDCModifier.h"
#include "ExpandablePrintBoard.h"
#include "ExpandableWhitePrintBoard.h"

class ModifiableBoard final
{
  ExpandableWhitePrintBoard __board;
  ExpandablePrintBoard __activeBoard;
  DCModifier __activeModifier;

public:
  explicit ModifiableBoard(HDC targetDC, UINT width, UINT height);

  void SetActiveModifier(DCModifier modifier = nullptr);
  void SaveActiveModifier();

  DCModifier GetActiveModifier() {return __activeModifier;}

  void SetSize(UINT width, UINT height);

  operator HDC() const {return __activeBoard;}

  UINT Width() const {return __board.Width();}
  UINT Height() const {return __board.Height();}

private:
  void RecalcualteActive();
};
