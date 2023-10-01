#pragma once
#include "keymap.h"
#include <unordered_map>

struct SkkInputMode
{
  KeyFunc Default = nulcmd;
  std::unordered_map<uint8_t, KeyFunc> KeyMap;

  SkkResult input(uint8_t c)
  {
    auto found = KeyMap.find(c);
    if (found == KeyMap.end()) {
      return Default(c, false);
    } else {
      return found->second(c, false);
    }
  }

  static SkkInputMode hirakana();
  static SkkInputMode katakana();
  static SkkInputMode zenei();
  static SkkInputMode ascii();
};
