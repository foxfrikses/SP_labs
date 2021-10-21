#pragma once

#include <windows.h>
#include <memory>

class IDCModifier {
  POINT __beginPoint = {0, 0}; 
  POINT __endPoint = {0, 0}; 

public:
  virtual void Modify(HDC hdc) = 0;

  IDCModifier& SetBeginPoint(POINT p);
  IDCModifier& SetEndPoint(POINT p);

  POINT BeginPoint() const;
  POINT EndPoint() const;
};

using DCModifier = std::shared_ptr<IDCModifier>;
