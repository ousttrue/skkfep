#pragma once
#include "conversion/dictionary.h"
#include <functional>
#include <optional>
#include <ostream>
#include <stdint.h>
#include <string>

namespace skk {
// https://ddskk.readthedocs.io/ja/latest/05_basic.html#id12
// https://github.com/uobikiemukot/yaskk
// 変換モード
enum class ConversionType
{
  // 確定入力モード
  Direct,
  // 見出し語入力モード
  Entry,
  // 送り仮名確定待ち
  Okuri,
  // 候補選択
  Selection,
};

// https://ddskk.readthedocs.io/ja/latest/05_basic.html#id8
// https://fixedpoint.jp/2010/02/19/skk-graph.html
// 入力モード
enum class InputType
{
  Ascii,
  // 仮名(ひらがなデフォルトとカタカナのテーブル切り替えができる)
  Kana,
  // 全角英語
  Zenkaku,
  // 漢字コード(skkfepの追加モード)
  // TODO: unicode 入力にする
  Code,
};

struct Output
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

  bool operator==(const Output& rhs) const
  {
    return Confirmed == rhs.Confirmed && Unconfirmed == rhs.Unconfirmed;
  }

  Output& operator+=(const Output& rhs)
  {
    Confirmed += rhs.Confirmed;
    Unconfirmed += rhs.Unconfirmed;
    return *this;
  }
};

inline std::ostream&
operator<<(std::ostream& os, const Output& rhs)
{
  os << "{" << rhs.Confirmed << ", " << rhs.Unconfirmed << "}";
  return os;
}

struct Result
{
  ::skk::Output Output;
  std::optional<ConversionType> NextConversinMode;
  std::optional<InputType> NextInputMode;
  CandidateSelector Context;

  Result& operator+=(const Result& rhs)
  {
    Output += rhs.Output;
    if (rhs.NextConversinMode) {
      NextConversinMode = rhs.NextConversinMode;
    }
    if (rhs.NextInputMode) {
      NextInputMode = rhs.NextInputMode;
    }
    return *this;
  }
};

using KeyFunc = std::function<Result(uint8_t, bool)>;
inline Result
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
  InputType Type;
  KeyFunc DefaultFunc = {};
  std::unordered_map<uint8_t, KeyFunc> Keymap;

  Result input(uint8_t c, bool okuri)
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

} // namespace
