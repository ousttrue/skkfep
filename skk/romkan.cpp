#include "romkan.h"
#include "connsh.h"
#include "skk.h"
#include "statusline.h"
#include "stty.h"
#include "terms.h"
#include <ctype.h>
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
void
iKanaV(char c)
{
  inputKanaVowel(c, child::writeShells);
}

void
iKanaC(char c)
{
  inputKanaConso(c, child::writeShells, child::flushOut);
}

void
flthru(char c)
{
  flushKana();
  child::thru(c);
}

void
flushKana()
{
  flushLastConso('\0', child::writeShells, child::flushOut);
}

void
inputKanaVowel(char c, const OutFunc& output)
{
  auto vowel = vowel_from_char(c);
  if (Kindex != CON::_0) { /* if preceding consonant exists */
    csrLeft(Nconso);
    erase(Nconso);
    csrLeft(Nconso);
  }
  if (SmallTU && vowel == Vowel::U) {
    output(CurrentTab KANA_XTU);
  } else {
    output(CurrentTab[(int)Kindex][(int)vowel]);
  }
  SmallTU = 0;
  Kindex = {};
  Nconso = 0;
}

void
inputKanaConso(char c, const OutFunc& output, void (*flush)(int))

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
        output(CurrentTab KANA_NN);
        Nconso = 0;
        Kindex = {};
        return;
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
  if (!notOverwrite) {
    flushLastConso(c, output, flush);
  }
  Nconso++;
  LastConso[Nconso] = c;
  terminal::write1(c);
}

void
flushLastConso(char c, const OutFunc& output, void (*flush)(int))
{
  if (Nconso == 0) {
    return;
  }

  if (Nconso <= 2) {
    csrLeft(Nconso);
  } else {
    csrLeft(Nconso);
    erase(Nconso);
    csrLeft(Nconso);
  }

  if (LastConso[Nconso] == 'n') {
    output(CurrentTab KANA_NN);
    if (flush != NULL)
      flush(2);
  } else if (LastConso[Nconso] == c) {
    output(CurrentTab KANA_XTU);
    if (flush != NULL)
      flush(2);
  }
  Nconso = 0;
}

void
tglK(char c)
{
  flushKana();
  if (CurrentTab == HiraTab) {
    CurrentTab = KataTab;
    g_skk.showmode(KKANA_MODE);
  } else {
    CurrentTab = HiraTab;
    g_skk.showmode(KANA_MODE);
  }
}

void
cancelConso()
{
  rubout(Nconso);
  Nconso = 0;
  Kindex = {};
  SmallTU = 0;
}

void
kanaBS(char c)
{
  int i, n;
  char con[MAX_CONSO];

  if (Nconso) {
    n = Nconso;
    for (i = 1; i < Nconso; i++)
      con[i] = LastConso[i];
    cancelConso();
    for (i = 1; i < n; i++)
      iKanaC(con[i]);
  } else {
    child::thru(c);
  }
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

void
toggleKana()
{
  if (CurrentTab == HiraTab)
    g_skk.showmode(KANA_MODE);
  else
    g_skk.showmode(KKANA_MODE);
}

} // namespace
