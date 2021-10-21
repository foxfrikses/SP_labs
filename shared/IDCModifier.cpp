#include "IDCModifier.h"

IDCModifier& IDCModifier::SetBeginPoint(POINT p) {
  __beginPoint = p;
  return *this;
}

IDCModifier& IDCModifier::SetEndPoint(POINT p) {
  __endPoint = p;
  return *this;
}

POINT IDCModifier::BeginPoint() const {
  return __beginPoint;
}

POINT IDCModifier::EndPoint() const {
  return __endPoint;
}