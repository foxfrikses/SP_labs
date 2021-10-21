#include "IDCModifierList.h"

void DCModifierList::AddModifier(DCModifier modifier, DCModifier beforeModifier) 
{
  if (modifier == nullptr) {
    return;
  }
  if (__modifierToIterator.find(modifier) != __modifierToIterator.end()) {
    return;
  }

  auto beforeIt = __modifiers.end();

  if (auto it = __modifierToIterator.find(beforeModifier);
      it != __modifierToIterator.end()) {
    beforeIt = it->second;
  }

  auto insertedIt = __modifiers.insert(beforeIt, modifier);
  __modifierToIterator.emplace(std::move(modifier), std::move(insertedIt));
}

void DCModifierList::DeleteModifier(DCModifier modifier) 
{
  if (!modifier) {
    return;
  }

  auto mtiIt = __modifierToIterator.find(modifier);

  if (mtiIt == __modifierToIterator.end()) {
    return;
  }

  __modifiers.erase(mtiIt->second);
  __modifierToIterator.erase(mtiIt);
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
