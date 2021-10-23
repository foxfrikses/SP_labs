#pragma once

#include "IDCModifier.h"

#include <unordered_map>
#include <memory>
#include <list>

class DCModifierList 
{
  using ModifierList = std::list<DCModifier>;
  using ModifierListIter = ModifierList::iterator;

  std::list<DCModifier> __modifiers;
  std::unordered_map<DCModifier, ModifierListIter> __modifierToIterator;

public:
  bool AddModifier(DCModifier modifier, DCModifier beforeModifier = nullptr);
  bool DeleteModifier(DCModifier modifier);

  ModifierListIter Find(DCModifier modifier);

  ModifierListIter begin();
  ModifierListIter end();
};
