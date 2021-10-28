#include "WinEvent.h"

#include <stdexcept>

WinEvent::WinEvent()
  : __hEvent(CreateEvent(NULL,  // default security attributes
                         TRUE,  // manual-reset event
                         FALSE, // initial state is nonsignaled
                         NULL))  // object name
{
  if (!__hEvent) {
    throw std::runtime_error("WinEvent creation error");
  }
}

bool WinEvent::Wait()
{
  auto res = WaitForSingleObject(__hEvent, INFINITE);
  return WAIT_OBJECT_0 == res;
}

bool WinEvent::Signal()
{
  return SetEvent(__hEvent);
}
