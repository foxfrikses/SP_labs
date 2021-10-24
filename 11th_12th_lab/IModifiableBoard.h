#pragma once

#include <windows.h>
#include <memory>

#include "IDCModifier.h"

class IHDCHandler
{
public:
  virtual operator HDC() const = 0;
  virtual ~IHDCHandler() {}
};

using HDCHandlerPtr = std::unique_ptr<IHDCHandler>;

class IModifiableBoard
{
public:
  virtual ~IModifiableBoard() {}

  virtual void AddModifier(DCModifier modifier) = 0;
  virtual void RemoveModifier(DCModifier modifier) = 0;

  virtual void SetSize(UINT width, UINT height) = 0;
  virtual auto GetSize() const -> std::pair<UINT, UINT> = 0;

  virtual HDCHandlerPtr GetDC() = 0;
};
