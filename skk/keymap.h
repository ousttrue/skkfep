#pragma once
#include <functional>
#include <optional>
#include <stdint.h>
#include <string>

// https://ddskk.readthedocs.io/ja/latest/05_basic.html#id12
// https://github.com/uobikiemukot/yaskk
// 変換モード
enum class SkkConversionModes
{
  // 確定入力モード
  Direct,
  // 見出し語入力
  Entry,
  // 送り仮名確定待ち
  Okuri,
  // 候補選択
  Selection,
};

// https://ddskk.readthedocs.io/ja/latest/05_basic.html#id8
// https://fixedpoint.jp/2010/02/19/skk-graph.html
// 入力モード
enum class SkkInputModes
{
  Ascii,
  // ひらがな
  Hirakana,
  // カタカナ
  Katakana,
  // 全角英語
  Zenei,
};

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

struct SkkOutput
{
  // 確定(to child process & 出力待ちflushout)
  //   母音、子音+母音
  //   漢字選択
  //   エラー、キャンセル入力
  std::string Confirmed;

  // 未確定(to term)
  //   ローマ字未確定(子音単体)
  //   漢字未確定(未選択)
  //   送り未確定？
  std::string Unconfirmed;

  SkkOutput& operator+=(const SkkOutput& rhs)
  {
    Confirmed += rhs.Confirmed;
    Unconfirmed += rhs.Unconfirmed;
    return *this;
  }
};

struct SkkResult
{
  SkkOutput Output;
  SkkConversionModes ConversinMode;
  SkkInputModes InputMode;

  std::optional<SkkModes> NextMode;

  std::optional<KeymapTypes> NextKeymap;

  bool RestoreKeymap = false;

  // 再入力
  // 小文字に変えて再入力
  char ReInput = 0;
  bool Okuri = false;

  SkkResult& operator+=(const SkkResult& rhs)
  {
    Output += rhs.Output;
    if (rhs.NextMode) {
      NextMode = rhs.NextMode;
    }
    if (rhs.NextKeymap) {
      NextKeymap = rhs.NextKeymap;
    }
    if (rhs.RestoreKeymap) {
      RestoreKeymap = rhs.RestoreKeymap;
    }
    if (rhs.ReInput) {
      ReInput = rhs.ReInput;
      Okuri = rhs.Okuri;
    }
    return *this;
  }
};

using KeyFunc = std::function<SkkResult(uint8_t, bool)>;
inline SkkResult
nulcmd(char, bool)
{
  return {};
}

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

  SkkResult input(uint8_t c, bool okuri)
  {
    auto found = Keymap.find(c);
    if (found == Keymap.end()) {
      return DefaultFunc(c, okuri);
    } else {
      return found->second(c, okuri);
    }
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
