#include "romkan.h"
#include "ctrlcode.h"
#include "skk.h"
#include "terms.h"
#include <ctype.h>
#include <sstream>
#include <string.h>

#define KANA_NN [(int)CON::NN][0]
#define KANA_XTU [(int)CON::NN][1]

extern skk::KanaTable HiraTab;
extern skk::KanaTable KataTab;

namespace skk {

KanaInput::KanaInput()
  : InputMode(InputType::Kana, "かな")
{
}

const KanaTable&
KanaInput::currentTab() const
{
  if (IsHiragana) {
    return HiraTab;
  } else {
    return KataTab;
  }
}

Result
KanaInput::putc(char8_t c)
{
  switch (c) {
    case CTRL_H:
      return { .Output = backSpace() };

    case 'l':
      // ascii mode
      return {
        .Output = { .Confirmed = flushKana() },
        .NextInputMode = InputType::Ascii,
      };

    case 'L':
      // 全角mode
      return {
        .Output = { .Confirmed = flushKana() },
        .NextInputMode = InputType::Zenkaku,
      };

    case 'q':
      // toggle: ひらがな<>カタカナ
      IsHiragana = !IsHiragana;
      return { .Output = { .Confirmed = flushKana() } };
  }

  // 母音
  if (vowel_from_char(c)) {
    return { .Output = inputKanaVowel(c) };
  }

  // 子音
  return {
    .Output = inputKanaConso(c),
  };
}

// 母音
skk::Output
KanaInput::inputKanaVowel(char c)
{
  auto vowel = vowel_from_char(c);
  assert(vowel);
  std::stringstream ss;
  if (SmallTU && vowel == Vowel::U) {
    ss << currentTab().m_table KANA_XTU;
  } else {
    ss << currentTab().m_table[(int)Kindex][(int)*vowel];
  }
  clear();
  return { .Confirmed = ss.str() };
}

// 子音
skk::Output
KanaInput::inputKanaConso(char c)
{
  char notOverwrite = 0;
  switch (c) {
    case 'k':
      Kindex = CON::K;
      break;

    case 's':
      if (Consonant.back() == 't') {
        Kindex = CON::TS;
        notOverwrite = 1;
      } else {
        Kindex = CON::S;
      }
      break;

    case 't':
      if (Consonant.back() == 'x') {
        SmallTU = 1;
        notOverwrite = 1;
      }
      Kindex = CON::T;
      break;

    case 'n':
      if (Consonant.back() == 'n') {
        Consonant.clear();
        Kindex = {};
        return { .Confirmed = currentTab().m_table KANA_NN };
      }
      Kindex = CON::N;
      break;

    case 'h':
      switch (Consonant.back()) {
        case 'c':
          notOverwrite = 1;
          Kindex = CON::CH;
          break;
        case 's':
          notOverwrite = 1;
          Kindex = CON::SH;
          break;
        case 'd':
          notOverwrite = 1;
          Kindex = CON::DH;
          break;
        case 't':
          notOverwrite = 1;
          Kindex = CON::TH;
          break;
        default:
          Kindex = CON::H;
      }
      break;
    case 'm':
      Kindex = CON::M;
      break;
    case 'r':
      Kindex = CON::R;
      break;
    case 'g':
      Kindex = CON::G;
      break;
    case 'z':
      Kindex = CON::Z;
      break;
    case 'd':
      Kindex = CON::D;
      break;
    case 'b':
      Kindex = CON::B;
      break;
    case 'p':
      Kindex = CON::P;
      break;
    case 'x':
      Kindex = CON::X;
      break;
    case 'y':
      if (Consonant.back() && Kindex < CON::Y) {
        notOverwrite = 1;
        Kindex = (CON)((int)Kindex + (int)CON::Y + 1);
      } else if (Consonant.back() == 'j') {
        notOverwrite = 1;
        Kindex = CON::JY;
      } else
        Kindex = CON::Y;
      break;
    case 'w':
      if (Consonant.back() == 'x') {
        notOverwrite = 1;
        Kindex = CON::XW;
      } else
        Kindex = CON::W;
      break;
    case 'j':
      Kindex = CON::J;
      break;
    case 'f':
      Kindex = CON::F;
      break;
    case 'v':
      Kindex = CON::V;
      break;
  }

  std::stringstream ss;
  if (!notOverwrite) {
    ss << flushLastConso(c);
  }
  Consonant.push_back(c);

  char str[] = { c, 0 };
  return {
    .Confirmed = ss.str(),
    .Unconfirmed = str,
  };
}

skk::Output
KanaInput::backSpace()
{
  skk::Output output;
  if (Consonant.size()) {
    auto con = Consonant;
    clear();
    for (auto c : con) {
      output += inputKanaConso(c);
    }
  } else {
    output.Confirmed.push_back(CTRL_H);
  }
  return output;
}

void
KanaInput::clear()
{
  Consonant.clear();
  Kindex = {};
  SmallTU = 0;
}

std::string
KanaInput::flushLastConso(char c)
{
  if (Consonant.empty()) {
    return "";
  }

  std::string out;
  if (Consonant.back() == 'n') {
    out += currentTab().m_table KANA_NN;
  } else if (Consonant.back() == c) {
    out += currentTab().m_table KANA_XTU;
  }
  Consonant.clear();

  return out;
}

} // namespace
