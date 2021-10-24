#pragma once

#include "DCModifierList.h"
#include "ExpandablePrintBoard.h"
#include "ExpandableWhitePrintBoard.h"

#include "IModifiableBoard.h"

#include <mutex>

template <class MUTEX>
class ModifiableBoard final
  : public IModifiableBoard 
{
  mutable MUTEX __m;
  mutable bool __isCalculated;
  ExpandableWhitePrintBoard __board;
  DCModifierList __modifiers;

public:
  class HDCHandler;

  explicit ModifiableBoard(HDC targetDC, UINT width, UINT height);
  ~ModifiableBoard() override {}
 
  ModifiableBoard(const ModifiableBoard&) = delete;
  ModifiableBoard(ModifiableBoard&&) = delete;
  ModifiableBoard& operator=(const ModifiableBoard&) = delete;
  ModifiableBoard& operator=(ModifiableBoard&&) = delete;

  void AddModifier(DCModifier modifier) override;
  void RemoveModifier(DCModifier modifier) override;

  void SetSize(UINT width, UINT height) override;
  auto GetSize() const -> std::pair<UINT, UINT> override;

  HDCHandlerPtr GetDC() override;

  class HDCHandler final
    : public IHDCHandler
  {
    const std::lock_guard<MUTEX> __lock;
    HDC __hdc;
  public:
    explicit HDCHandler(
      MUTEX& m, 
      const ExpandableWhitePrintBoard& board
    );
    ~HDCHandler() override;
    operator HDC() const override;
  };

private:
  void Calcualate();
};





template <class MUTEX>
ModifiableBoard<MUTEX>::ModifiableBoard(
  HDC targetDC, UINT width, UINT height
)
  : __board(targetDC, width, height)
  , __isCalculated(true)
{}

template <class MUTEX>
void ModifiableBoard<MUTEX>::AddModifier(DCModifier modifier)
{
  std::lock_guard lock(__m);
  if (__modifiers.AddModifier(std::move(modifier))) {
    __isCalculated = false;
  }
}

template <class MUTEX>
void ModifiableBoard<MUTEX>::RemoveModifier(DCModifier modifier)
{
  std::lock_guard lock(__m);
  if (__modifiers.DeleteModifier(std::move(modifier))) {
    __isCalculated = false;
  }
}

template <class MUTEX>
void ModifiableBoard<MUTEX>::SetSize(UINT width, UINT height) 
{
  std::lock_guard lock(__m);
  if (width == __board.Width() && 
      height == __board.Height())
    return;

  __board.Resize(width, height);
  __isCalculated = false;
}

template <class MUTEX>
std::pair<UINT, UINT> ModifiableBoard<MUTEX>::GetSize() const
{
  std::lock_guard lock(__m);
  return {__board.Width(), __board.Height()};
}

template <class MUTEX>
void ModifiableBoard<MUTEX>::Calcualate()
{
  std::lock_guard<MUTEX> lock(__m);
  if (__isCalculated) {
    return;
  }
  __board.Clear();
  for (auto modifier : __modifiers) {
    modifier->Modify(__board);
  }
}

template <class MUTEX>
ModifiableBoard<MUTEX>::HDCHandler::HDCHandler(
  MUTEX& m, 
  const ExpandableWhitePrintBoard& board
) 
  : __lock(m)
{
  __hdc = (HDC)board; 
}

template <class MUTEX>
ModifiableBoard<MUTEX>::HDCHandler::~HDCHandler()
{}

template <class MUTEX>
ModifiableBoard<MUTEX>::HDCHandler::operator HDC() const 
{
  return __hdc;
}

template <class MUTEX>
HDCHandlerPtr ModifiableBoard<MUTEX>::GetDC()
{
  Calcualate();
  return std::make_unique<HDCHandler>(__m, __board);
}

