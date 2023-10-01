#pragma once
#include <functional>
#include <optional>
#include <stdint.h>
#include <string>

enum SkkModes
{
  // ascii
  SKK_MODE,
  // かな
  KANA_MODE,
  // 全角
  ZENEI_MODE,
  // カナ
  KKANA_MODE,
  // ▽
  KINPUT_MODE,
  // ▼
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

enum class KeymapTypes
{
  // ascii
  Normal,
  // romkan
  Kana,
  // 全角
  Zenkaku,
  // 入力▽+romkan
  KanjiInput,
  // 入力*
  OkuriInput,
  // 候補選択
  Selection,
  // 入力▽+ascii
  KAlphaInput,

  CodeInput,
};

// 内部ステート(入力モード、変換モード、未確定文字列)の変更と
// 3系統の出力 pty(確定文字列 child process), std out(未確定文字列 preedit), statusline(モード表示など)
struct SkkOutput
{
  // 確定(to child process & 出力待ちflushout)
  //   母音、子音+母音
  //   漢字選択
  //   エラー、キャンセル入力
  std::string Through;
  // 未確定(to term)
  //   ローマ字未確定(子音単体)
  //   漢字未確定(未選択)
  //   送り未確定？
  std::string Predit;

  std::optional<SkkModes> NextMode;

  std::optional<KeymapTypes> NextKeymap;

  SkkOutput& operator+=(const SkkOutput& rhs)
  {
    Through += rhs.Through;
    Predit += rhs.Predit;
    return *this;
  }
};

using KeyFunc = std::function<SkkOutput(uint8_t, bool)>;

// 変換モード(直入力、▽見出し入力, *送り入力、▼Select)
// + 入力モード(ascii, ひら、カタ、全)
// = keymap
//
// input => keymap => mode => app
struct Keymap
{
  KeymapTypes Type;
  KeyFunc DefaultFunc = {};
  std::unordered_map<uint8_t, KeyFunc> Keymap;

  SkkOutput input(uint8_t c, bool okuri)
  {
    auto found = Keymap.find(c);

    auto f = DefaultFunc;
    if (found != Keymap.end()) {
      f = found->second;
    }

    return f(c, okuri);
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
