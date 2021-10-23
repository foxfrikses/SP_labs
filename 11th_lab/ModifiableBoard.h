#pragma once

#include "DCModifierList.h"
#include "ExpandablePrintBoard.h"
#include "ExpandableWhitePrintBoard.h"

#include <mutex>

class ModifiableBoard final
{
  mutable std::mutex __m;
  mutable bool __isCalculated;
  ExpandableWhitePrintBoard __board;
  DCModifierList __modifiers;

public:
  class HDCHandler;

  explicit ModifiableBoard(HDC targetDC, UINT width, UINT height);

  void AddModifier(DCModifier modifier);
  void RemoveModifier(DCModifier modifier);

  void SetSize(UINT width, UINT height);
  auto GetSize() const -> std::pair<UINT, UINT>;

  UINT Width() const;
  UINT Height() const;

  HDCHandler GetDC();

  class HDCHandler final
  {
    const std::lock_guard<std::mutex> __lock;
    HDC __hdc;
  public:
    explicit HDCHandler(
      std::mutex& m, 
      const ExpandableWhitePrintBoard& board
    );
    operator HDC() const;
  };

private:
  void Calcualate();
};