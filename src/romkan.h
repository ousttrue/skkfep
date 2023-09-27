#pragma once

enum SkkModes
{
  SKK_MODE,
  KANA_MODE,
  ZENEI_MODE,
  KKANA_MODE,
  KINPUT_MODE,
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

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
extern short Nconso;
extern char SmallTU;
extern short Kindex;

namespace romkan {

void
iKanaV(char c);

void
iKanaC(char c);

void
iZenAl(char c);

void
iZenEx(char c);

void
flthru(char c);

void
flushKana();

void
inputKanaVowel(char c, void (*output)(const char*));

void
inputKanaConso(char c, void (*output)(const char*), void (*flush)(int));

void
flushLastConso(char c, void (*output)(const char*), void (*flush)(int));

void
toKana(char c);

void
tglK(char c);

void
cancelConso();

void
kanaBS(char c);

void
inputZenkakuAlpha(char c, void (*output)(const char*));

void
inputZenkakuEx(char c, void (*output)(const char*));

void
codeinMsg();

void
inputCode(char);

void
cancelCode(char);

void
codein(char c);

void
enterCode(char);

void
hira2kata(char* buf);

} // namespace
