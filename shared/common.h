#pragma once

#include <windows.h>

class Handle final
{
  HANDLE __handle;

public:
  Handle(HANDLE handle);
  ~Handle();

  HANDLE H() const;
 
  Handle(Handle&&) noexcept;
  Handle& operator=(Handle&&) noexcept;

  Handle(const Handle&) = delete;
  Handle& operator=(const Handle&) = delete;
};
