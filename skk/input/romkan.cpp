#include "romkan.h"
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

// void
// cancelConso()
// {
//   rubout(Nconso);
//   Nconso = 0;
//   Kindex = {};
//   SmallTU = 0;
// }
//
// skk::Result
// kanaBS(char c, bool)
// {
//   skk::Result output;
//   if (Nconso) {
//     int n = Nconso;
//     char con[MAX_CONSO];
//     for (int i = 1; i < Nconso; i++)
//       con[i] = LastConso[i];
//     cancelConso();
//     for (int i = 1; i < n; i++)
//       output += iKanaC(con[i]);
//   } else {
//     output.Output.Confirmed += c;
//   }
//   return output;
// }
//
// void
// hira2kata(char* buf)
// {
//   int i = 0;
//   while (buf[i]) {
//     if (buf[0] & 0x80) {
//       for (int j = 0; j <= (int)CON::NN; j++) {
//         for (int k = 0; k < 5; k++) {
//           if (!strncmp(&buf[i], HiraTab[j][k], 2)) {
//             strncpy(&buf[i], KataTab[j][k], 2);
//             goto brk1;
//           }
//         }
//       }
//     brk1:
//       i += 2;
//     } else
//       i++;
//   }
// }

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
  if (c == 'l') {
    return {
      .Output = { .Confirmed = flushKana() },
      .NextInputMode = InputType::Ascii,
    };
  } else if (c == 'q') {
    IsHiragana = !IsHiragana;
    return { .Output = { .Confirmed = flushKana() } };
  }
  if (vowel_from_char(c)) {
    return { .Output = inputKanaVowel(c) };
  } else {
    return {
      .Output = inputKanaConso(c),
    };
  }
}

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
  SmallTU = false;
  Kindex = {};
  Consonant.clear();
  return { .Confirmed = ss.str() };
}

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
