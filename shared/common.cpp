#include "common.h"

#include <utility>

Handle::Handle(HANDLE handle)
  : __handle(handle)
{}

Handle::~Handle()
{
  if (__handle)
    CloseHandle(__handle);
}

HANDLE Handle::H() const
{
  return __handle;
}
 
Handle::Handle(Handle&& newHandle) noexcept
  : __handle(NULL)
{
  *this = std::move(newHandle);
}

Handle& Handle::operator=(Handle&& newHandle) noexcept
{
  std::swap(__handle, newHandle.__handle);
  return *this;
}

