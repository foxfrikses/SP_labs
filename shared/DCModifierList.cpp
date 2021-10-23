#include "DCModifierList.h"

bool DCModifierList::AddModifier(DCModifier modifier, DCModifier beforeModifier) 
{
  if (modifier == nullptr) {
    return false;
  }
  if (__modifierToIterator.find(modifier) != __modifierToIterator.end()) {
    return false;
  }

  auto beforeIt = __modifiers.end();

  if (auto it = __modifierToIterator.find(beforeModifier);
      it != __modifierToIterator.end()) {
    beforeIt = it->second;
  }

  auto insertedIt = __modifiers.insert(beforeIt, modifier);
  __modifierToIterator.emplace(std::move(modifier), std::move(insertedIt));

  return true;
}

bool DCModifierList::DeleteModifier(DCModifier modifier) 
{
  if (!modifier) {
    return false;
  }

  auto mtiIt = __modifierToIterator.find(modifier);

  if (mtiIt == __modifierToIterator.end()) {
    return false;
  }

  __modifiers.erase(mtiIt->second);
  __modifierToIterator.erase(mtiIt);
  return true;
}

DCModifierList::ModifierListIter 
DCModifierList::Find(DCModifier modifier) 
{
  auto mtiIt = __modifierToIterator.find(modifier);

  if (mtiIt == __modifierToIterator.end()) {
    return __modifiers.end();
  }

  return mtiIt->second;
}

DCModifierList::ModifierListIter 
DCModifierList::begin() 
{
  return __modifiers.begin();
}

DCModifierList::ModifierListIter 
DCModifierList::end() 
{
  return __modifiers.end();
}
