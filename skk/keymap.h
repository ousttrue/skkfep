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

//
// キーボード入力に対する副作用をあらわす
// 内部ステートの変更と、child process, std out(preedit), statusline の
// 3系統の出力がありえる。
// test しづらい。
//
// * nullcmd: 何もしない
//
// [to child process]
// * thru 素通し
// * romaji 変換
//
// [to preedit]
// * 見出し語 romaji
//
using KeyFunc = std::function<void(uint8_t, bool)>;

struct SkkResult
{
  // write child pty
  std::string Output;
  // write stdout
  PreEdit Predit;
  SkkModes Mode;
};

enum class KeymapTypes
{
  Normal,
  Selection,
  CodeInput,
  Kana,
  Zenkaku,
  KanjiInput,
  OkuriInput,
  KAlphaInput,
};

struct Keymap
{
  KeymapTypes Type;
  KeyFunc DefaultFunc = {};
  std::unordered_map<uint8_t, KeyFunc> Keymap;

  SkkResult input(uint8_t c, bool okuri)
  {
    auto found = Keymap.find(c);

    auto f = DefaultFunc;
    if (found != Keymap.end()) {
      f = found->second;
    }

    f(c, okuri);
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
