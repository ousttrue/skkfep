#pragma once
#include <assert.h>
#include <functional>
#include <string>

// 母音
enum class Vowel
{
  A,
  I,
  U,
  E,
  O,
};
inline Vowel
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
  assert(false);
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
};

using OutFunc = std::function<void(std::string_view s)>;

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
extern short Nconso;
extern bool SmallTU;
extern CON Kindex;

namespace romkan {

// 母音
void
iKanaV(char c);

// 子音
void
iKanaC(char c);

void
flthru(char c);

void
flushKana();

// 母音
void
inputKanaVowel(char c, const OutFunc& output);

// 子音
void
inputKanaConso(char c, const OutFunc& output, void (*flush)(int));

void
flushLastConso(char c, const OutFunc& output, void (*flush)(int));

void
tglK(char c);

void
cancelConso();

void
kanaBS(char c);

void
hira2kata(char* buf);

void
toggleKana();

} // namespace
