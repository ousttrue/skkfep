#pragma once
#include "keymap.h"
#include <assert.h>
#include <functional>
#include <optional>
#include <string>

struct Skk;

// 母音
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
};

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
extern short Nconso;
extern bool SmallTU;
extern CON Kindex;

namespace romkan {

bool
isHiragana();

// 母音
skk::Result
iKanaV(char c, bool o = {});

// 子音
skk::Result
iKanaC(char c, bool o = {});

skk::Result
flthru(char c, bool);

std::string
flushKana();

// 母音
std::string
inputKanaVowel(char c);

// 子音
std::string
inputKanaConso(char c);

std::string
flushLastConso(char c);

skk::Result
tglK(Skk* skk);

void
cancelConso();

skk::Result
kanaBS(char c, bool o = {});

void
hira2kata(char* buf);

} // namespace
