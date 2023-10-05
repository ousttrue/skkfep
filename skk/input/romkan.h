#pragma once
#include "inputmode.h"
#include "keymap.h"
#include <assert.h>
#include <functional>
#include <optional>
#include <string>

namespace skk {

enum class Vowel
{
  A,
  I,
  U,
  E,
  O,
};
inline std::optional<Vowel>
vowel_from_char(char c)
{
  switch (c) {
    case 'a':
      return Vowel::A;
    case 'i':
      return Vowel::I;
    case 'u':
      return Vowel::U;
    case 'e':
      return Vowel::E;
    case 'o':
      return Vowel::O;
  }

  return {};
}

// 子音(Consonant)
enum class CON
{
  _0, // あ行
  K,
  S,
  T,
  N,
  H,
  M,
  R,
  G,
  Z,
  D,
  B,
  P,
  X,
  Y,
  W,
  KY,
  SY,
  TY,
  NY,
  HY,
  MY,
  RY,
  GY,
  ZY,
  DY,
  BY,
  PY,
  XY,
  CH,
  J,
  SH,
  TS,
  XW,
  F,
  V,
  DH,
  JY,
  TH,
  NN,
  COUNT,
};
struct KanaTable
{
  const char* m_table[(int)CON::COUNT][5];
};

struct KanaInput : InputMode
{
  // Hiragana or Katakana
  bool IsHiragana = true;
  std::string Consonant;
  bool SmallTU = false;
  CON Kindex = {};

  KanaInput();
  Result putc(char8_t c) override;

private:
  Output inputKanaVowel(char c);
  Output inputKanaConso(char c);
  Output backSpace();
  void clear();
  std::string flushLastConso(char c);
  std::string flushKana() { return flushLastConso('\0'); }
  const KanaTable& currentTab() const;
};

} // namespace
