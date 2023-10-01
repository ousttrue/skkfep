#include "romkan.h"
#include "skk.h"
#include "stty.h"
#include "terms.h"
#include <ctype.h>
#include <sstream>
#include <string.h>

#define KANA_NN [(int)CON::NN][0]
#define KANA_XTU [(int)CON::NN][1]

extern const char* HiraTab[][5];
extern const char* KataTab[][5];

// LastConso[0] is always '\0'
char LastConso[MAX_CONSO];
short Nconso = 0;

bool SmallTU = false;

CON Kindex = {};
static const char* (*CurrentTab)[5] = HiraTab;

namespace romkan {

bool
isHiragana()
{
  return CurrentTab == HiraTab;
}

SkkResult
iKanaV(char c, bool)
{
  return { .Output = { .Confirmed = inputKanaVowel(c) } };
}

SkkResult
iKanaC(char c, bool)
{
  return { .Output = { .Confirmed = inputKanaConso(c) } };
}

SkkResult
flthru(char c, bool)
{
  return { .Output = { .Confirmed = flushKana() } };
}

std::string
flushKana()
{
  return flushLastConso('\0');
}

std::string
inputKanaVowel(char c)
{
  auto vowel = vowel_from_char(c);
  assert(vowel);
  if (Kindex != CON::_0) { /* if preceding consonant exists */
    csrLeft(Nconso);
    erase(Nconso);
    csrLeft(Nconso);
  }
  std::stringstream ss;
  if (SmallTU && vowel == Vowel::U) {
    ss << CurrentTab KANA_XTU;
  } else {
    ss << CurrentTab[(int)Kindex][(int)*vowel];
  }
  SmallTU = 0;
  Kindex = {};
  Nconso = 0;
  return ss.str();
}

std::string
inputKanaConso(char c)
{
  char notOverwrite = 0;
  switch (c) {
    case 'k':
      Kindex = CON::K;
      break;

    case 's':
      if (LastConso[Nconso] == 't') {
        Kindex = CON::TS;
        notOverwrite = 1;
      } else {
        Kindex = CON::S;
      }
      break;

    case 't':
      if (LastConso[Nconso] == 'x') {
        SmallTU = 1;
        notOverwrite = 1;
      }
      Kindex = CON::T;
      break;

    case 'n':
      if (LastConso[Nconso] == 'n') {
        csrLeft(Nconso);
        Nconso = 0;
        Kindex = {};
        return CurrentTab KANA_NN;
      }
      Kindex = CON::N;
      break;

    case 'h':
      switch (LastConso[Nconso]) {
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
      if (LastConso[Nconso] && Kindex < CON::Y) {
        notOverwrite = 1;
        Kindex = (CON)((int)Kindex + (int)CON::Y + 1);
      } else if (LastConso[Nconso] == 'j') {
        notOverwrite = 1;
        Kindex = CON::JY;
      } else
        Kindex = CON::Y;
      break;
    case 'w':
      if (LastConso[Nconso] == 'x') {
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
  Nconso++;
  LastConso[Nconso] = c;
  terminal::write1(c);
  return ss.str();
}

std::string
flushLastConso(char c)
{
  if (Nconso == 0) {
    return "";
  }

  if (Nconso <= 2) {
    csrLeft(Nconso);
  } else {
    csrLeft(Nconso);
    erase(Nconso);
    csrLeft(Nconso);
  }

  std::string out;
  if (LastConso[Nconso] == 'n') {
    out += CurrentTab KANA_NN;
  } else if (LastConso[Nconso] == c) {
    out += CurrentTab KANA_XTU;
  }
  Nconso = 0;
  return out;
}

SkkResult
tglK(Skk* skk)
{
  auto out = flushKana();
  if (CurrentTab == HiraTab) {
    CurrentTab = KataTab;
    skk->showmode(KKANA_MODE);
  } else {
    CurrentTab = HiraTab;
    skk->showmode(KANA_MODE);
  }
  return { .Output = { .Confirmed = out } };
}

void
cancelConso()
{
  rubout(Nconso);
  Nconso = 0;
  Kindex = {};
  SmallTU = 0;
}

SkkResult
kanaBS(char c, bool)
{
  SkkResult output;
  if (Nconso) {
    int n = Nconso;
    char con[MAX_CONSO];
    for (int i = 1; i < Nconso; i++)
      con[i] = LastConso[i];
    cancelConso();
    for (int i = 1; i < n; i++)
      output += iKanaC(con[i]);
  } else {
    output.Output.Confirmed += c;
  }
  return output;
}

void
hira2kata(char* buf)
{
  int i = 0;
  while (buf[i]) {
    if (buf[0] & 0x80) {
      for (int j = 0; j <= (int)CON::NN; j++) {
        for (int k = 0; k < 5; k++) {
          if (!strncmp(&buf[i], HiraTab[j][k], 2)) {
            strncpy(&buf[i], KataTab[j][k], 2);
            goto brk1;
          }
        }
      }
    brk1:
      i += 2;
    } else
      i++;
  }
}

} // namespace
