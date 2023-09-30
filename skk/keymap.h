#pragma once
#include <functional>
#include <stdint.h>
#include <string>

enum SkkModes
{
  SKK_MODE,
  KANA_MODE,
  ZENEI_MODE,
  KKANA_MODE,
  KINPUT_MODE,
  KSELECT_MODE,
};
inline const char*
mode_string(SkkModes m)
{
  switch (m) {
    case SKK_MODE:
      return "SKK";
    case KANA_MODE:
      return "かな";
    case ZENEI_MODE:
      return "全英";
    case KKANA_MODE:
      return "カナ";
    case KINPUT_MODE:
      return "単語入力";
    case KSELECT_MODE:
      return "単語選択";
  }
}

struct PreEdit
{
  // internal raw input
  std::string Input;
  // show user interface
  std::string Display;
};

using KeyFunc = std::function<void(uint8_t)>;

struct SkkResult
{
  // write child pty
  std::string Output;
  // write stdout
  PreEdit Predit;
  SkkModes Mode;
};

struct Keymap
{
  KeyFunc DefaultFunc = {};
  std::unordered_map<uint8_t, KeyFunc> Keymap;

  SkkResult input(uint8_t c, bool okuri)
  {
    auto found = Keymap.find(c);

    auto f = &DefaultFunc;
    if (found != Keymap.end()) {
      f = &found->second;
    }

    (*f)(c);
    return {

    };
  }

  void setall(const KeyFunc& f)
  {
    DefaultFunc = f;
    Keymap.clear();
  }

  const KeyFunc& operator[](uint8_t key) const
  {
    auto found = Keymap.find(key);
    if (found != Keymap.end()) {
      return found->second;
    }
    return DefaultFunc;
  }
};
using KeymapPtr = Keymap*;
