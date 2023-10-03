#pragma once
#include "keymap.h"
#include <unordered_map>

namespace skk {

struct InputMode
{
  InputType Type;
  std::string Statusline;
  KeyFunc Default = nulcmd;
  std::unordered_map<uint8_t, KeyFunc> KeyMap;

protected:
  InputMode() {}

public:
  ~InputMode() {}
  InputMode(const InputMode&) = delete;
  InputMode& operator=(const InputMode&) = delete;
  Result input(uint8_t c)
  {
    auto found = KeyMap.find(c);
    if (found == KeyMap.end()) {
      return Default(c, false);
    } else {
      return found->second(c, false);
    }
  }
};

struct AsciiInput : InputMode
{
  AsciiInput();
};

struct ZenkakuInput : InputMode
{
  ZenkakuInput();
};

struct KanaInput : InputMode
{
  KanaInput();
};

struct CodeInput : InputMode
{
  CodeInput();
};

} // namespace
